#include "markstack.h"

MARK_STACK
mark_stack_new()
{
	MARK_STACK mark_st;

	mark_st.count = 0;
	mark_st.max_count = 32;
	mark_st.list = (MRK*)malloc(mark_st.max_count * sizeof(MRK));

	return mark_st;
}

MRK
mrk_new(const char *name, unsigned short point, fpos_t fpos)
{
	MRK mrk;

	mrk.name = (char *)malloc(strlen(name) + 1);
	strcpy(mrk.name, name);
	mrk.point = point;
	mrk.fpos = fpos;

	return mrk;
}

void
mrk_delete(MRK *mrk)
{
	free(mrk->name);
	mrk->name = NULL;
}

int
mark_stack_push(MARK_STACK *mark_st, const char *name, unsigned short point, fpos_t fpos)
{
	mark_st->list[mark_st->count] = mrk_new(name, point, fpos);
	mark_st->count++;

	if (mark_st->max_count == mark_st->count)
	{
		mark_st->max_count += 32;
		mark_st->list = (MRK*)realloc(mark_st->list, mark_st->max_count * sizeof(MRK));
	}
}


int
mark_stack_pop(MARK_STACK *mark_st, int idx)
{
	if (mark_st->count == 0 || idx < 0 || idx >= mark_st->count) return 0;
	mark_st->count--;

	mrk_delete(&mark_st->list[idx]);

	while (idx < mark_st->count)
	{
		mark_st->list[idx] = mark_st->list[idx + 1];
		idx++;
	}

	return mark_st->count;
}

int
mark_stack_fnd(MARK_STACK *mark_st, const char *name)
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
mark_stack_clear(MARK_STACK *mark_st)
{
	while (mark_st->count--)
	{
		mrk_delete(&mark_st->list[mark_st->count]);
	}
}

void
mark_stack_delete(MARK_STACK *mark_st)
{
	mark_stack_clear(mark_st);
	free(mark_st->list);
	mark_st->max_count = 0;
}
