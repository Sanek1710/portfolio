#ifndef _DEF_DEF_MRKSTACK_H_
#define _DEF_DEF_MRKSTACK_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct DEF_MRK
{
	char *name;
	unsigned short point;
}DEF_MRK;

typedef struct DEF_MARK_STACK
{
	int count;
	int max_count;
	DEF_MRK *list;
}DEF_MARK_STACK;

DEF_MRK  def_mrk_new(const char *name, unsigned short point);
void def_mrk_delete(DEF_MRK *def_mrk);

DEF_MARK_STACK def_mark_stack_new();
int  def_mark_stack_push(DEF_MARK_STACK *mark_st, const char *name, unsigned short point);
int  def_mark_stack_pop(DEF_MARK_STACK *mark_st, int idx);
int  def_mark_stack_fnd(DEF_MARK_STACK *mark_st, const char *name);
void def_mark_stack_clear(DEF_MARK_STACK *mark_st);
void def_mark_stack_delete(DEF_MARK_STACK *mark_st);

#endif // !_DEF_MRKSTACK_H_
