#include "strstack.h"

STR_STACK
str_stack_new()
{
	STR_STACK str_st;

	str_st.count = 0;
	str_st.max_count = 32;
	str_st.list = (STR*)malloc(str_st.max_count * sizeof(STR));

	return str_st;
}

STR
str_new(const char *name)
{
	STR str;

	str.name = (char *)malloc(strlen(name) + 1);
	strcpy(str.name, name);

	return str;
}

void
str_delete(STR *str)
{
	free(str->name);
	str->name = NULL;
}

int
str_stack_push(STR_STACK *str_st, const char *name)
{
	str_st->list[str_st->count] = str_new(name);
	str_st->count++;

	if (str_st->max_count == str_st->count)
	{
		str_st->max_count += 32;
		str_st->list = (STR*)realloc(str_st->list, str_st->max_count * sizeof(STR));
	}
}


int
str_stack_pop(STR_STACK *str_st)
{
	if (str_st->count == 0) return 0;
	str_st->count--;
	str_delete(&str_st->list[str_st->count]);

	return str_st->count;
}

int
str_stack_fnd(STR_STACK *str_st, const char *name)
{
	for (int i = 0; i < str_st->count; i++)
	{
		if (!strcmp(str_st->list[i].name, name))
		{
			return i;
		}
	}

	return -1;
}

void
str_stack_clear(STR_STACK *str_st)
{
	while (str_st->count--)
	{
		str_delete(&str_st->list[str_st->count]);
	}
}

void
str_stack_delete(STR_STACK *str_st)
{
	str_stack_clear(str_st);
	free(str_st->list);
	str_st->max_count = 0;
}
