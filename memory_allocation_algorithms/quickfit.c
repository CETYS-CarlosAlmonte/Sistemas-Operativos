#include <stdio.h>
#include <stdlib.h>

unsigned char memory[8192];
#define BLOCK_SIZE 512

typedef struct node {
	struct node *next;
	unsigned size; // Amount of free memory.
} node;

static node *freelist;

void *quickfit_malloc(size_t size)
{
	void *p;

	if (!(freelist))
	{
		// Set memory block of 512 bytes.
		freelist = (node*) &memory[0];
		int i;
		node *tmp = freelist;
		for (i = 1; i < sizeof(memory)/BLOCK_SIZE; i++) {
			tmp->next = (node*) &memory[i*BLOCK_SIZE];
			tmp = tmp->next;
		}
		if (size <= (BLOCK_SIZE-sizeof(node))) {
			p = (void*) ((unsigned)freelist + sizeof(node));
			freelist->size = size;
		}
		else
			p = NULL;
	}
	else
	{
		node *tmp = freelist;
		while (tmp->size && tmp->next)
			tmp = tmp->next;

		if (!tmp->size && size <= (BLOCK_SIZE-sizeof(node))) // Found available block. And requested memory fits in block.
		{
			p = (void*) ((unsigned)tmp + sizeof(node));
			tmp->size = size;
		}
		else
			p = NULL;
	}

	return p;
}

int main()
{
	long *l = quickfit_malloc(sizeof(long));
	printf("L: %d\n", (unsigned)l);
	long long *ll = quickfit_malloc(sizeof(long long));
	printf("LL: %d\n", (unsigned)ll);

	return 0;
}
