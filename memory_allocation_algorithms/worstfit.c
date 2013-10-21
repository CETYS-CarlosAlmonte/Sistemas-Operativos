#include <stdio.h>
#include <stdlib.h>

unsigned char memory[8192];

typedef struct node {
	struct node *next;
	unsigned size; // Amount of free memory.
} node;

static node *freelist;

void *worstfit_malloc(size_t size)
{
	void *p;

	if (!(freelist))
	{
		p = (void*) &memory[0];
		freelist = (node*) ((unsigned)p + size);
		freelist->size = sizeof(memory) - size;
		freelist->next = freelist;
	}
	else
	{
		node *tmp = freelist;
		node *biggest = tmp, *previous;
		unsigned realsize; // Includes the size of the struct (which is usable).
		while (tmp->next)
		{
			realsize = tmp->size + sizeof(node);
			if (tmp->size > biggest->size && realsize >= size)
			{
				previous = biggest;
				biggest = tmp;
			}
			tmp = tmp->next;
		}
		realsize = biggest->size + sizeof(node); // Reusing the variable for the same idea.
		if (realsize >= size) // In case there's only 1 free block.
		{
			if (previous)
				previous->next = biggest->next;
			else if (biggest->next == biggest)
				freelist = (node*) ((unsigned)biggest + size); // Rellocate start of the list.
			p = (void*) biggest;
		}
		else
			p = NULL;
	}

	return p;
}

int main()
{
	long *l = worstfit_malloc(sizeof(long));
	printf("L: %d\n", (unsigned)l);
	printf("B: %d\n", (unsigned)freelist);

	return 0;
}
