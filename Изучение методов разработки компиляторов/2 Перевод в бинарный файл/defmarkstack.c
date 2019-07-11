#include "defmarkstack.h"

DEF_MARK_STACK
def_mark_stack_new()
{
	DEF_MARK_STACK mark_st;

	mark_st.count = 0;
	mark_st.max_count = 32;
	mark_st.list = (DEF_MRK*)malloc(mark_st.max_count * sizeof(DEF_MRK));

	return mark_st;
}

DEF_MRK
def_mrk_new(const char *name, unsigned short point)
{
	DEF_MRK def_mrk;

	def_mrk.name = (char *)malloc(strlen(name) + 1);
	strcpy(def_mrk.name, name);
	def_mrk.point = point;

	return def_mrk;
}

void
def_mrk_delete(DEF_MRK *def_mrk)
{
	free(def_mrk->name);
	def_mrk->name = NULL;
}

int
def_mark_stack_push(DEF_MARK_STACK *mark_st, const char *name, unsigned short point)
{
	mark_st->list[mark_st->count] = def_mrk_new(name, point);
	mark_st->count++;

	if (mark_st->max_count == mark_st->count)
	{
		mark_st->max_count += 32;
		mark_st->list = (DEF_MRK*)realloc(mark_st->list, mark_st->max_count * sizeof(DEF_MRK));
	}
}


int
def_mark_stack_pop(DEF_MARK_STACK *mark_st, int idx)
{
	if (mark_st->count == 0 || idx < 0 || idx >= mark_st->count) return 0;
	mark_st->count--;

	def_mrk_delete(&mark_st->list[idx]);

	while (idx < mark_st->count)
	{
		mark_st->list[idx] = mark_st->list[idx + 1];
		idx++;
	}

	return mark_st->count;
}

int
def_mark_stack_fnd(DEF_MARK_STACK *mark_st, const char *name)
{
	for (int i = 0; i < mark_st->count; i++)
	{
		if (!strcmp(mark_st->list[i].name, name))
		{
			return i;
		}
	}

	return -1;
}

void
def_mark_stack_clear(DEF_MARK_STACK *mark_st)
{
	while (mark_st->count--)
	{
		def_mrk_delete(&mark_st->list[mark_st->count]);
	}
}

void
def_mark_stack_delete(DEF_MARK_STACK *mark_st)
{
	def_mark_stack_clear(mark_st);
	free(mark_st->list);
	mark_st->max_count = 0;
}
