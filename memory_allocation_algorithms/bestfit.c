#include <stdio.h>
#include <stdlib.h>

unsigned char memory[8192];

typedef struct node {
	struct node *next;
	unsigned size; // Amount of free memory.
} node;

static node *freelist;

void *bestfit_malloc(size_t size)
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
		node *smallest = tmp, *previous;
		unsigned realsize; // Includes the size of the struct (which is usable).
		while (tmp->next)
		{
			realsize = tmp->size + sizeof(node);
			if (tmp->size < smallest->size && realsize >= size)
			{
				previous = smallest;
				smallest = tmp;
			}
			tmp = tmp->next;
		}
		realsize = smallest->size + sizeof(node); // Reusing the variable for the same idea.
		if (realsize >= size) // In case there's only 1 free block.
		{
			if (previous)
				previous->next = smallest->next;
			else if (smallest->next == smallest)
				freelist = (node*) ((unsigned)smallest + size); // Rellocate start of the list.
			p = (void*) smallest;
		}
		else
			p = NULL;
	}

	return p;
}

int main()
{
	long *l = bestfit_malloc(sizeof(long));
	printf("L: %d\n", (unsigned)l);
	printf("B: %d\n", (unsigned)freelist);

	return 0;
}
