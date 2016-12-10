#ifndef tokens_h
#define tokens_h
/* tokens.h -- List of labelled tokens and stuff
 *
 * Generated from: interpreter.g
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * ANTLR Version 1.33MR33
 */
#define zzEOF_TOKEN 1
#define SPACE 2
#define NUM 3
#define ID 4
#define ENDEF 5
#define LT 6
#define GT 7
#define EQ 8
#define ASSIG 9
#define SUM 10
#define MIN 11
#define MULT 12
#define IF 13
#define THEN 14
#define ELSE 15
#define END 16
#define WHILE 17
#define DO 18
#define PRINT 19
#define INPUT 20
#define AND 21
#define OR 22
#define NOT 23
#define PUSH 24
#define POP 25
#define EMPTY 26
#define SIZE 27
#define REST 28

#ifdef __USE_PROTOS
void program(AST**_root);
#else
extern void program();
#endif

#ifdef __USE_PROTOS
void instruction(AST**_root);
#else
extern void instruction();
#endif

#ifdef __USE_PROTOS
void loop(AST**_root);
#else
extern void loop();
#endif

#ifdef __USE_PROTOS
void inputprint(AST**_root);
#else
extern void inputprint();
#endif

#ifdef __USE_PROTOS
void action(AST**_root);
#else
extern void action();
#endif

#ifdef __USE_PROTOS
void ifthen(AST**_root);
#else
extern void ifthen();
#endif

#ifdef __USE_PROTOS
void thenthen(AST**_root);
#else
extern void thenthen();
#endif

#ifdef __USE_PROTOS
void elseelse(AST**_root);
#else
extern void elseelse();
#endif

#ifdef __USE_PROTOS
void onecondition(AST**_root);
#else
extern void onecondition();
#endif

#ifdef __USE_PROTOS
void multcondition(AST**_root);
#else
extern void multcondition();
#endif

#ifdef __USE_PROTOS
void expression(AST**_root);
#else
extern void expression();
#endif

#ifdef __USE_PROTOS
void var(AST**_root);
#else
extern void var();
#endif

#ifdef __USE_PROTOS
void objectaction(AST**_root);
#else
extern void objectaction();
#endif

#endif
extern SetWordType zzerr1[];
extern SetWordType zzerr2[];
extern SetWordType setwd1[];
extern SetWordType zzerr3[];
extern SetWordType zzerr4[];
extern SetWordType zzerr5[];
extern SetWordType zzerr6[];
extern SetWordType zzerr7[];
extern SetWordType setwd2[];
extern SetWordType zzerr8[];
extern SetWordType zzerr9[];
extern SetWordType zzerr10[];
extern SetWordType zzerr11[];
extern SetWordType setwd3[];
