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
        attr->kind = "CONST";
        attr->text = text;
      break;
      case ID:
        attr->kind = "VAR";
        attr->text = text;
      break;
      case INPUT:
        attr->kind = "INPUT";
      break;
      case PRINT:
        attr->kind = "PRINT";
      break;
      case ASSIG:
        attr->kind = "ASSIGN";
      break;
      case SUM:
        attr->kind = "PLUS";
      break;
      case REST:
        attr->kind = "MINUS";
      break;
      case MULT:
        attr->kind = "TIMES";
      break;
      case IF:
        attr->kind = "IF";
      break;
      case WHILE:
        attr->kind = "WHILE";
      break;
      case EMPTY:
        attr->kind = "STACK";
        attr->text = "EMPTY";
      break;
      case PUSH:
        attr->kind = "STACK";
        attr->text = "PUSH";
      break;
      case POP:
        attr->kind = "STACK";
        attr->text = "POP";
      break;
      case SIZE:
        attr->kind = "STACK";
        attr->text = "SIZE";
      break;
      case AND:
        attr->kind = "AND";
      break;
      case OR:
        attr->kind = "OR";
      break;
      case NOT:
        attr->kind = "NOT";
      break;
      case EQ:
        attr->kind = "EQ";
      break;
      case GT:
        attr->kind = "GT";
      break;
      case LT:
        attr->kind = "LT";
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


string parseBExpr(AST *a){
  string salida = "";

  while (a!= NULL){

  }
  return salida;


}

void parseCommands(AST *a) {
  while(a!=NULL) {
    string tipo = a->kind;
    if(tipo == "INPUT") {
        cout << "Input" << " ";
        file << "Input" << " ";
        file << child(a,0)->text << endl;
    }
    else if(tipo == "PRINT"){
        cout << "Input" << " ";
        file << "Print" << " ";
        file << child(a,0)->text << endl;
    }
    else if (tipo == "IF") {
        string s = parseBExpr(child(a,0));
        file << "Cond" << " (" << s << ") ";

        file << " (";
        parseCommands(child(a,2));
        file << ") ";

        if (child(a,2) != NULL) {
          file << " (";
          parseCommands(child(a,2));
          file << ") ";
        }
        file << endl;
    }
    else if(tipo == "WHILE") {

    }
    else if (tipo == "STACK") {

    }
    else if (tipo == "ASSIG") {

    }
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
