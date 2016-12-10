 #header
<<
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

// struct to store information about tokens
typedef struct {
  string kind;
  string text;
} Attrib;

// function to fill token information (predeclaration)
void zzcr_attr(Attrib *attr, int type, char *text);

// fields for AST nodes
#define AST_FIELDS string kind; string text;
#include "ast.h"

// macro to create a new AST node (and function predeclaration)
#define zzcr_ast(as,attr,ttype,textt) as=createASTnode(attr,ttype,textt)
AST* createASTnode(Attrib* attr, int ttype, char *textt);
>>

<<
#include <cstdlib>
#include <cmath>


//GLOBAL
  ofstream file;

// function to fill token information
void zzcr_attr(Attrib *attr, int type, char *text) {
    attr->text = "";
    switch (type) {
      case NUM:
        attr->kind = "Const";
        attr->text = text;
      break;
      case ID:
        attr->kind = "Var";
        attr->text = text;
      break;
      case INPUT:
        attr->kind = "Input";
      break;
      case PRINT:
        attr->kind = "Print";
      break;
      case ASSIG:
        attr->kind = "Assign";
      break;
      case SUM:
        attr->kind = "Plus";
      break;
      case REST:
        attr->kind = "Minus";
      break;
      case MULT:
        attr->kind = "Times";
      break;
      case IF:
        attr->kind = "If";
      break;
      case WHILE:
        attr->kind = "While";
      break;
      case EMPTY:
        attr->kind = "Stack";
        attr->text = "Empty";
      break;
      case PUSH:
        attr->kind = "Stack";
        attr->text = "Push";
      break;
      case POP:
        attr->kind = "Stack";
        attr->text = "Pop";
      break;
      case SIZE:
        attr->kind = "Stack";
        attr->text = "Size";
      break;
      case AND:
        attr->kind = "And";
      break;
      case OR:
        attr->kind = "Or";
      break;
      case NOT:
        attr->kind = "Not";
      break;
      case EQ:
        attr->kind = "Eq";
      break;
      case GT:
        attr->kind = "Gt";
      break;
      case LT:
        attr->kind = "Lt";
      break;
      default:
        attr->kind = text;
      break;
    }
}

/// create a new "list" AST node with one element
AST* createASTlist(AST *child) {
 AST *as=new AST;
 as->kind="list";
 as->right=NULL;
 as->down=child;
 return as;
}

// function to create a new AST node
AST* createASTnode(Attrib* attr, int type, char* text) {
  AST* as = new AST;
  as->kind = attr->kind;
  as->text = attr->text;
  as->right = NULL;
  as->down = NULL;
  return as;
}

/// get nth child of a tree. Count starts at 0.
/// if no such child, returns NULL
AST* child(AST *a,int n) {
 AST *c=a->down;
 for (int i=0; c!=NULL && i<n; i++) c=c->right;
 return c;
}

/// print AST, recursively, with indentation
void ASTPrintIndent(AST *a,string s)
{
  if (a==NULL) return;

  cout<<a->kind;
  if (a->text!="") cout<<"("<<a->text<<")";
  cout<<endl;

  AST *i = a->down;
  while (i!=NULL && i->right!=NULL) {
    cout<<s+"  \\__";
    ASTPrintIndent(i,s+"  |"+string(i->kind.size()+i->text.size(),' '));
    i=i->right;
  }

  if (i!=NULL) {
      cout<<s+"  \\__";
      ASTPrintIndent(i,s+"   "+string(i->kind.size()+i->text.size(),' '));
      i=i->right;
  }
}


void parseNExpr(AST *a) {
  string tipo = a->kind;
  string op = "Plus";
  if(tipo == "Var" || tipo == "Const") {
    file << a->text;
  }
  else if (tipo == "Minus") op = "Minus";
  else if (tipo == "Times") op = "Times";

  if (tipo == "Plus" || tipo =="Times" || tipo == "Minus") {
    a = child(a,0);
    file << op <<" ";
    parseNExpr(a);
    file <<" ";
    parseNExpr(a->right);
  }
}

void parseBExpr(AST *a){
  string tipo = a->kind;
  a = child(a,0);
  if(tipo == "And") {
    file << "AND ";
    parseBExpr(a);
    file << " ";
    parseBExpr((a->right));
  }
  else if (tipo == "Or") {
   file << "OR ";
   parseBExpr(a);
   file << " ";
  parseBExpr(a->right);
  }
  else if (tipo == "Not") {
    file << "NOT ";
    parseBExpr(a);
  }
  else if (tipo == "Gt") {
    file << "Gt ";
    parseNExpr(a);
    file << " ";
    parseNExpr(a->right);
  }
  else if (tipo == "Eq") {
    file << "Eq ";
    parseNExpr(a);
    file << " ";
    parseNExpr(a->right);
  }
  else if (tipo == "Lt") {
    file << "Lt ";
    parseNExpr(a);
    file << " ";
    parseNExpr(a->right);
  }

}


void parseStacks(AST *a) {
  string tipo = a->text;
  if (tipo == "Push") {
    a = child(a,0);
    string var = a->text;
    file << "PUSH " << var << " (";
    parseNExpr(a->right);
    file << ")";
  }
  else if (tipo == "Pop") {
    a = child(a,0);
    string var = a->text;
    string var2 = (a->right)->text;
    file << "POP " << var << " " << var2;
  }
  else if (tipo == "Empty") {
    string var = child(a,0)->text;
    file << "EMPTY "<< var;
  }
  else if (tipo == "Size") {
    string var = child(a,0)->text;
    file << "SIZE "<< var;
  }
}


void parseCommands(AST *a) {
  AST *aux;
  while(a!=NULL) {
    string tipo = a->kind;
    if(tipo == "Input") {
        file << "Input" << " ";
        aux = child(a,0);
        file << aux->text;
    }
    else if(tipo == "Print"){
        file << "Print" << " ";
        aux = child(a,0);
        file << aux->text;
    }
    else if (tipo == "If") {
        file << "Cond" << " (";
        aux = child(a,0);
        parseBExpr(aux);
        file << ") ";

        file << " (";
        aux = child((aux->right),0);
        parseCommands(aux);
        file << ") ";

        if (child(a,2) != NULL) {
          aux = aux->right;
          file << " (";
          parseCommands(aux);
          file << ") ";
        }
        file << endl;
    }
    else if(tipo == "While") {

      file << "Loop" << " (";
      aux = child(a,0);
      parseBExpr(aux);
      file << ") ";

      file << " (";
      aux = child(a,1);
      parseCommands(aux);
      file << ") ";


      file << endl;

    }
    else if (tipo == "Stack") {
      parseStacks(a);
    }
    else if (tipo == "Assign") {
      file << "Assign ";
      aux = child(a,0);
      string s = aux->text;
      file << s << " (";
      parseNExpr(aux->right);
      file << ") ";
    }
    else if (tipo == "list") {
        aux = child(a,0);
        file << " (";
        parseCommands(aux);
        file << ") ";
    }
    cout << endl;
    a = a->right;
    }
}



/// print AST
void ASTPrint(AST *a)
{

  AST *a2 = a;
  while (a2!=NULL) {
    cout<<" ";
    ASTPrintIndent(a2,"");
    a2=a2->right;
  }

  parseCommands(child(a,0));

}


int main() {
  AST *root = NULL;
  ANTLR(program(&root), stdin);


  file.open ("prinths.txt");
  ASTPrint(root);
  file.flush();
  file.close();
}
>>



#lexclass START
#token SPACE "[\ \n]" << zzskip();>>

#token NUM "[0-9]+"
#token ID "[A-Z]+[0-9]+"

#token ENDEF "ENDEF"

//CONDITIONS
#token LT "\<"
#token GT "\>"
#token EQ "\=\="
#token ASSIG "\:\="

//OPERATORS
#token SUM "\+"
#token MIN "\-"
#token MULT "\*"

//DEFINITIONS
#token IF "IF"
#token THEN "THEN"
#token ELSE "ELSE"
#token END "END"

#token WHILE "WHILE"
#token DO "DO"

//IO
#token PRINT "PRINT"
#token INPUT "INPUT"

//LOGICS
#token AND "AND"
#token OR "OR"
#token NOT "NOT"

//ACTIONS
#token PUSH "PUSH"
#token POP "POP"
#token EMPTY "EMPTY"
#token SIZE "SIZE"


program: (instruction)* <<#0=createASTlist(_sibling);>>;

instruction: inputprint | action | ifthen | expression | loop | objectaction;

loop: WHILE^ multcondition DO! program END!;

inputprint: (INPUT^|PRINT^) ID;

action: ID ASSIG^ expression;

ifthen: IF^ multcondition thenthen (elseelse|) END!;

thenthen: THEN! program;
elseelse: ELSE! program;

onecondition: expression (EQ^ | LT^ | GT^) expression;

multcondition: (NOT | ) onecondition (((AND^ | OR^) multcondition)| );

expression: var ((SUM^|REST^|MULT^) expression | );

var: ID | NUM;

objectaction: SIZE^ ID
	    | EMPTY^ ID
	    | POP^ ID ID
	    | PUSH^ ID var;
