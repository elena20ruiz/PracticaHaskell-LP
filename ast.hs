
------------------------------------------------------------------------------
------DATA DECLARATIONS (2.1)

type Ident = String
data Type = V | S deriving Eq


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
--------READ (3)
