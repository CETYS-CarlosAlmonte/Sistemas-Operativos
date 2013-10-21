#include <stdlib.h>
#include <stdio.h>

unsigned char memory[8192];

typedef struct node {
	struct node *next;
	unsigned size;
} node;

static node *base;
static node *last;

void *nextfit_malloc(size_t size)
{
	void *p;

	if (!(base))
	{
		base = (node*) &memory[0];
		base->size = size;
		last = base;
		base->next = base; // Circular
		p = (void*) ((unsigned)base + sizeof(node));
	}
	else
	{
		node *tmp = last;
		char replace = 0;
		while (tmp->next)
		{
			if (tmp->size == 0 && ((unsigned)tmp->next - ((unsigned)tmp + sizeof(node)) == size))
			{
				replace = 1;
				break;
			}
			tmp = tmp->next;
			if (tmp == last)
				break; // Looked all throught the circular list.
		}
		/*if (tmp->size == 0 && ((unsigned)tmp + sizeof(node) + size) < ((unsigned)base + sizeof(memory)))
			replace = 1;*/
		if (replace)
		{
			tmp->size = size;
			p = (void*) ((unsigned)tmp + sizeof(node));
		}
		else
		{
			node *newnode = (node*) ((unsigned)tmp + sizeof(node) + tmp->size);
			if (((unsigned)newnode + sizeof(node) + size - 1) <= (unsigned)base + sizeof(memory))
			{
				newnode->size = size;
				newnode->next = base;
				tmp->next = newnode;
				last = newnode;
				p = (void*) ((unsigned)newnode + sizeof(node));
			}
			else // Doesn't fit anywhere.
				p = NULL;
		}
	}

	return p;
}

int main(void)
{
	long *l = nextfit_malloc(sizeof(long));
	printf("B: %d\n", (unsigned)base);
	printf("L1: %d\n", (unsigned)l);
	int *i = nextfit_malloc(sizeof(int));
	printf("I1: %d\n", (unsigned)i);
	long *m = nextfit_malloc(sizeof(long));
	printf("L2: %d\n", (unsigned)m);

	printf("B: %d\n", (unsigned)base->next->next->next);

	return 0;
}
