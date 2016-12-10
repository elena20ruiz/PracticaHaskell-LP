/*
 * A n t l r  T r a n s l a t i o n  H e a d e r
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 *
 *   antlr -gt interpreter.g
 *
 */

#define ANTLR_VERSION	13333
#include "pcctscfg.h"
#include "pccts_stdio.h"

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
#define GENAST

#include "ast.h"

#define zzSET_SIZE 4
#include "antlr.h"
#include "tokens.h"
#include "dlgdef.h"
#include "mode.h"

/* MR23 In order to remove calls to PURIFY use the antlr -nopurify option */

#ifndef PCCTS_PURIFY
#define PCCTS_PURIFY(r,s) memset((char *) &(r),'\0',(s));
#endif

#include "ast.c"
zzASTgvars

ANTLR_INFO

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

void
#ifdef __USE_PROTOS
program(AST**_root)
#else
program(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (setwd1[LA(1)]&0x1) ) {
      instruction(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  (*_root)=createASTlist(_sibling);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x2);
  }
}

void
#ifdef __USE_PROTOS
instruction(AST**_root)
#else
instruction(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (setwd1[LA(1)]&0x4) ) {
    inputprint(zzSTR); zzlink(_root, &_sibling, &_tail);
  }
  else {
    if ( (LA(1)==ID) ) {
      action(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==IF) ) {
        ifthen(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {
        if ( (setwd1[LA(1)]&0x8) ) {
          expression(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {
          if ( (LA(1)==WHILE) ) {
            loop(zzSTR); zzlink(_root, &_sibling, &_tail);
          }
          else {
            if ( (setwd1[LA(1)]&0x10) ) {
              objectaction(zzSTR); zzlink(_root, &_sibling, &_tail);
            }
            else {zzFAIL(1,zzerr1,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
        }
      }
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x20);
  }
}

void
#ifdef __USE_PROTOS
loop(AST**_root)
#else
loop(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(WHILE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  multcondition(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(DO);  zzCONSUME;
  program(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(END);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x40);
  }
}

void
#ifdef __USE_PROTOS
inputprint(AST**_root)
#else
inputprint(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==INPUT) ) {
      zzmatch(INPUT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==PRINT) ) {
        zzmatch(PRINT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {zzFAIL(1,zzerr2,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x80);
  }
}

void
#ifdef __USE_PROTOS
action(AST**_root)
#else
action(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(ASSIG); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  expression(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x1);
  }
}

void
#ifdef __USE_PROTOS
ifthen(AST**_root)
#else
ifthen(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(IF); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  multcondition(zzSTR); zzlink(_root, &_sibling, &_tail);
  thenthen(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==ELSE) ) {
      elseelse(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==END) ) {
      }
      else {zzFAIL(1,zzerr3,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzmatch(END);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x2);
  }
}

void
#ifdef __USE_PROTOS
thenthen(AST**_root)
#else
thenthen(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(THEN);  zzCONSUME;
  program(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x4);
  }
}

void
#ifdef __USE_PROTOS
elseelse(AST**_root)
#else
elseelse(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(ELSE);  zzCONSUME;
  program(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x8);
  }
}

void
#ifdef __USE_PROTOS
onecondition(AST**_root)
#else
onecondition(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  expression(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==EQ) ) {
      zzmatch(EQ); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==LT) ) {
        zzmatch(LT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {
        if ( (LA(1)==GT) ) {
          zzmatch(GT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {zzFAIL(1,zzerr4,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
    zzEXIT(zztasp2);
    }
  }
  expression(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x10);
  }
}

void
#ifdef __USE_PROTOS
multcondition(AST**_root)
#else
multcondition(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==NOT) ) {
      zzmatch(NOT); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (setwd2[LA(1)]&0x20) ) {
      }
      else {zzFAIL(1,zzerr5,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  onecondition(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (setwd2[LA(1)]&0x40) ) {
      {
        zzBLOCK(zztasp3);
        zzMake0;
        {
        {
          zzBLOCK(zztasp4);
          zzMake0;
          {
          if ( (LA(1)==AND) ) {
            zzmatch(AND); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {
            if ( (LA(1)==OR) ) {
              zzmatch(OR); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
            }
            else {zzFAIL(1,zzerr6,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
          zzEXIT(zztasp4);
          }
        }
        multcondition(zzSTR); zzlink(_root, &_sibling, &_tail);
        zzEXIT(zztasp3);
        }
      }
    }
    else {
      if ( (setwd2[LA(1)]&0x80) ) {
      }
      else {zzFAIL(1,zzerr7,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x1);
  }
}

void
#ifdef __USE_PROTOS
expression(AST**_root)
#else
expression(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  var(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (setwd3[LA(1)]&0x2) ) {
      {
        zzBLOCK(zztasp3);
        zzMake0;
        {
        if ( (LA(1)==SUM) ) {
          zzmatch(SUM); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==REST) ) {
            zzmatch(REST); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {
            if ( (LA(1)==MULT) ) {
              zzmatch(MULT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
            }
            else {zzFAIL(1,zzerr8,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
        }
        zzEXIT(zztasp3);
        }
      }
      expression(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (setwd3[LA(1)]&0x4) ) {
      }
      else {zzFAIL(1,zzerr9,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x8);
  }
}

void
#ifdef __USE_PROTOS
var(AST**_root)
#else
var(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==ID) ) {
    zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  }
  else {
    if ( (LA(1)==NUM) ) {
      zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {zzFAIL(1,zzerr10,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x10);
  }
}

void
#ifdef __USE_PROTOS
objectaction(AST**_root)
#else
objectaction(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==SIZE) ) {
    zzmatch(SIZE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  }
  else {
    if ( (LA(1)==EMPTY) ) {
      zzmatch(EMPTY); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==POP) ) {
        zzmatch(POP); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
        zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {
        if ( (LA(1)==PUSH) ) {
          zzmatch(PUSH); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
          var(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {zzFAIL(1,zzerr11,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x20);
  }
}
