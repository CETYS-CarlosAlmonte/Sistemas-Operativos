#include <stdio.h>
#include <stdlib.h>

unsigned char memory[8192];

typedef struct node {
	struct node *next;
	unsigned size;
} node;

static node *base;

void *firstfit_malloc(size_t size)
{
	void *p;

	if (!(base) || base && base->size == 0 && base->next && ((unsigned)base->next - ((unsigned)base + sizeof(node))) == size)
	{
		base = (node*) &memory[0];
		base->size = size;
		p = (void*) ((unsigned)base + sizeof(node));
	}
	else
	{
		node *tmp = base;
		int replace = 0;
		while (tmp->next)
		{
			if (tmp->size == 0 && ((unsigned)tmp->next - ((unsigned)tmp + sizeof(node)) == size))
			{
				replace = 1;
				break;
			}
			tmp = tmp->next;
		}
		if (tmp->size == 0 && ((unsigned)tmp + sizeof(node)) < ((unsigned)base + sizeof(memory)))
			replace = 1;
		if (replace)
		{
			tmp->size = size;
			p = (void*) ((unsigned)tmp + sizeof(node));
		}
		else
		{
			unsigned newnode = ((unsigned)tmp + sizeof(node) + tmp->size);
			if ((newnode + sizeof(node) + size - 1) < (unsigned)base + sizeof(memory))
			{
				tmp->next = (node*) newnode;
				tmp->next->size = size;
				p = (void*) ((unsigned)tmp->next + sizeof(node));
			}
			else // Doesn't fit anywhere.
				p = NULL;
		}
	}

	return p;
}

void myfree(void *p)
{
	node *tmp = base;
	while (tmp != p)
	{
		tmp = (node*) ((unsigned)tmp + sizeof(node));
		if (tmp != p)
		{
			tmp = (node*) ((unsigned)tmp - sizeof(node));
			tmp = tmp->next;
		}
	}
	tmp = (node*) ((unsigned)tmp - sizeof(node));
	tmp->size = 0;
}

int main()
{
	

	return 0;
}
