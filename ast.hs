
------------------------------------------------------------------------------
------DATA DECLARATIONS (2.1)

type Ident = [Char]
type Tipus = (Simple | Stack) deriving Eq

data Command a =
      Assign Ident (NExpr a)
      |Input Ident
      |Print Ident
      |OpStack a
      |Seq [Command a]
      |Cond (BExpr a) (Command a) (Command a |)
      |Loop (BExpr a) (Command a)
      |Empty
      deriving Read

data OpStack a = PUSH Ident (NExpr a)
                |POP Ident Ident
                |EMPTY Ident
                |SIZE Ident
                deriving Read

data BExpr a = AND (BExpr a) (BExpr a)
              | OR (BExpr a) (BExpr a)
              | NOT (BExpr a)
              | Gt (NExpr a) (NExpr a)
              | Eq (NExpr a) (NExpr a)
              | Lt (NExpr a) (NExpr a)
              deriving Read

data NExpr a = Var Ident
              | Const a
              | Plus (NExpr a) (NExpr a)
              | Minus (NExpr a) (NExpr a)
              | Times (NExpr a) (NExpr a)
              deriving Read
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--INSTANCES SHOW (2.2)


instance Show a => Show (Command a) where
        show(Assign x y) = concat[" ",x," ",":=",(show y)," "]
        show(Input x) = concat[" INPUT ",x," "]
        show(Print x) = concat[" PRINT ",x," "]
        show(OpStack o) = show o
        show(Seq la) = concat[" ",concat[show x | x <- la]," " ]
        show(Cond c a a1) = if (a1 != []) concat[" ","IF",show c,"THEN",show a, "ELSE", show a1,"END"," "]
                            else concat[" ","IF",show c,"THEN",show a,"END"," "]
        show(Loop c a) = concat[" ","WHILE",show c,"DO",show a,"END"," "]

instance Show a => Show (NExpr a) where
        show(Var a) = concat[" ",a," "]
        show(Const a) = concat[" ",show a," "]
        show(Plus a b) = concat[" ",show a,"+",show b," "]
        show(Minus a b) = concat[" ",show a,"-",show b," "]
        show(Times a b) = concat[" ",show a,"*",show b, " "]


instance Show a => Show (OpStack a) where
        show(PUSH a b) = concat[" ","PUSH",show a, show b," "]
        show(POP a b) =  concat[" ","PUSH",show a, show b," "]
        show(EMPTY a) = concat[" ","EMPTY",show a," "]
        show(SIZE a) = concat[" ","SIZE",show a," "]

instance Show a => Show (BExpr a) where
        show(AND a b) = concat[" ",show a,"AND",show b," "]
        show(OR a b) = concat[" ",show a,"AND",show b," "]
        show(NOT a) = concat[" ","NOT",show a," "]
        show(Gt a b) = concat[" ",show a,">",show b," "]
        show(Eq a b) = concat[" ",show a,"=",show b," "]
        show(Lt a b) = concat[" ",show a,"<",show b," "]

-------------------------------------------------------------------------------
--------INTERPRET (4)

--------1. DATA STACK PER A DATA TABLE
data Stack a = [a]

stack :: Stack a
stack = Stack []

push :: a -> Stack a -> Stack a
push a (Stack s) = Stack (a:s)

top :: Stack a -> a
top (Stack s) = s!!0

pop :: Stack a -> Stack a
pop (Stack (a:s)) = Stack s

size :: Stack a -> Int
size (Stack s) = length s

empty :: Stack a -> Bool
empty (Stack []) = True
empty (Stack a) = False

--------2. Composicio SymTable--------------------------------------------------
--------------------------------------------------------------------------------
data FilaTable a = (Ident Tipus | Empty) deriving Show
data SymTable a = FilaTable [a] deriving Show


--Funcions simples de Fila Table------------------------------------------------
filaTable:: FilaTable a
filaTable = Empty

getMaybeVar:: (FilaTable a ) -> Ident -> Maybe (a)
getMaybeVar Empty _ = Nothing
getMaybeVar (i t) v = if (i == val) then Just t
                                    else Nothing

getVar:: (FilaTable a) -> Ident
getVar (i t) = i
getVar Empty = ""

setVarFila :: FilaTable a -> Ident -> Tipus -> FilaTable a
setVarFila Empty val tip = Just (val tip)
setVarFila (i t) val tip = if (t == tip) then (val tip) else []
--------------------------------------------------------------------------------

--Funcions per al SymTable------------------------------------------------------
symTable:: SymTable a
symTable = FilaTable []

setVar :: SymTable a -> Ident -> Tipus -> Maybe (SymTable a)
setVar (FilaTable []) val t = Just (FilaTable (val t))
setVar (FilaTable a) val t
                        | fila == [] = Just (FilaTable (val t):a)
                        | otherwise = if ((getMaybeVar fila val) == Just(val t)) then Just (modifyFileTable a (val t))
                                                                                 else Nothing

                        where fila = filter (\x-> getVar x == val) a )

getVar :: SymTable a -> Ident -> Maybe a
getVar (FilaTable []) _ = Nothing
getVar (FilaTable lt) val = head([(getMaybeVar x val)| x<-lt, getVar x == val])


modifyFileTable::SymTable a -> Ident -> Tipus -> SymTable a
modifyFileTable a val t = (filter (\x-> getVar x /= val) a)++(val t))
--------------------------------------------------------------------------------



---4.2 Evaluable Class --------------------------------------------------------

class Evaluable e where
  eval::(Num a, Ord a) => (Ident -> Maybe a) -> (e a) -> (Either String a)
  typeCheck:: (Ident->String) -> (e a) -> Bool

---------Per a NExpr

data NExpr a = Var Ident
              | Const a
              | Plus (NExpr a) (NExpr a)
              | Minus (NExpr a) (NExpr a)
              | Times (NExpr a) (NExpr a)



instance Evaluable NExpr where
  typeCheck (Var Ident) =
  typeCheck Empty = --Error

  eval (getVar (FilaTable a) Ident) (Functor Ident)
                                                  | m == Nothing =
                                                  | otherwise ---Error
                                                  where m = (getVar (FilaTable a) Ident)

--------------------------------------------------------------------------------

----4.3 Interpetar--------------------------------------------------------------

interpretCommand::(Num a, Ord a) => SymTable a->[a]->Command a-> ((Either String [a]),SymTable a, [a])

interpretCommand (FilaTable a) ast
