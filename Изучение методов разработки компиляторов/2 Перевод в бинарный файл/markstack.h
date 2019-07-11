#ifndef _MRKSTACK_H_
#define _MRKSTACK_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct MRK
{
	char *name;
	unsigned short point;
	fpos_t fpos;
}MRK;

typedef struct MARK_STACK
{
	int count;
	int max_count;
	MRK *list;
}MARK_STACK;

MRK  mrk_new(const char *name, unsigned short point, fpos_t fpos);
void mrk_delete(MRK *mrk);

MARK_STACK mark_stack_new();
int  mark_stack_push(MARK_STACK *mark_st, const char *name, unsigned short point, fpos_t fpos);
int  mark_stack_pop(MARK_STACK *mark_st, int idx);
int  mark_stack_fnd(MARK_STACK *mark_st, const char *name);
void mark_stack_clear(MARK_STACK *mark_st);
void mark_stack_delete(MARK_STACK *mark_st);

#endif // !_MRKSTACK_H_
