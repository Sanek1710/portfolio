#ifndef _COMP_H_
#define _COMP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "strstack.h"

extern void yyerror(char *s);
extern void yyset_in (FILE *  _in_str );
extern int  yyget_lineno();
extern int  yylex(void);
extern int  yyparse(void);

FILE * tmp_file;

STACK MARK_FRST;
STACK MARK_SCND;

STR_STACK VARS;

const char * CUR_REG();


typedef enum TERM_TYPE
{
	t_num,
	t_var
}TERM_TYPE;

void print_vars();
void LOAD_VAR(const char *str);

void PUSH(const char *arg);
void POP (const char *arg);
void MOV (const char *str, TERM_TYPE type);
void OP(int op_id);
void UNOOP(int op_id);
void PREF(int op_id, const char *var);
void POST(int op_id, const char *var);
void RESET_EXP();

void PUSH_MARK(STACK *st);
int  POP_MARK (STACK *st);
int  LAST_MARK();

#endif