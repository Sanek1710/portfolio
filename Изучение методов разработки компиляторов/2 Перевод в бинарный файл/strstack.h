#ifndef _STRSTACK_H_
#define _STRSTACK_H_

#include <string.h>
#include <stdlib.h>

typedef struct STR
{
	char *name;
}STR;

typedef struct STR_STACK
{
	int count;
	int max_count;
	STR *list;
}STR_STACK;

STR  str_new(const char *name);
void str_delete(STR *str);

STR_STACK str_stack_new();
int  str_stack_push(STR_STACK *str_st, const char *name);
int  str_stack_pop(STR_STACK *str_st);
int  str_stack_fnd(STR_STACK *str_st, const char *name);
void str_stack_clear(STR_STACK *str_st);
void str_stack_delete(STR_STACK *str_st);

#endif // !_STRSTACK_H_
