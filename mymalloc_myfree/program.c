#include <stdio.h>
#include <stdlib.h>

unsigned char Memory[8192];

typedef struct node {
	struct node *Next;
	unsigned Size;
} Node;

static Node *Base;

void *MyMalloc(size_t size)
{
	void *p;

	if (!(Base) || Base && Base->Size == 0 && Base->Next && ((unsigned)Base->Next - ((unsigned)Base + sizeof(Node))) == size)
	{
		Base = (Node*) &Memory[0];
		Base->Size = size;
		p = (void*) ((unsigned)Base + sizeof(Node));
	}
	else
	{
		Node *tmp = Base;
		int replace = 0;
		while (tmp->Next)
		{
			if (tmp->Size == 0 && ((unsigned)tmp->Next - ((unsigned)tmp + sizeof(Node)) == size))
			{
				replace = 1;
				break;
			}
			tmp = tmp->Next;
		}
		if (tmp->Size == 0 && ((unsigned)tmp + sizeof(Node)) < ((unsigned)Base + sizeof(Memory)))
			replace = 1;
		if (replace)
		{
			tmp->Size = size;
			p = (void*) ((unsigned)tmp + sizeof(Node));
		}
		else
		{
			unsigned newnode = ((unsigned)tmp + sizeof(Node) + tmp->Size);
			if ((newnode + sizeof(Node) + size - 1) < (unsigned)Base + sizeof(Memory))
			{
				tmp->Next = (Node*) newnode;
				tmp->Next->Size = size;
				p = (void*) ((unsigned)tmp->Next + sizeof(Node));
			}
			else //Doesn't fit
				p = NULL;
		}
	}

	return p;
}

void MyFree(void *p)
{
	Node *tmp = Base;
	while (tmp != p)
	{
		tmp = (Node*) ((unsigned)tmp + sizeof(Node));
		if (tmp != p)
		{
			tmp = (Node*) ((unsigned)tmp - sizeof(Node));
			tmp = tmp->Next;
		}
	}
	tmp = (Node*) ((unsigned)tmp - sizeof(Node));
	tmp->Size = 0;
}

int main()
{
	long long *L = MyMalloc(sizeof(long long));
	int *I = MyMalloc(sizeof(int));
	*L = 3333;
	*I = 120;
	printf("%d\n%d\n", (unsigned)Base, (unsigned)L);
	MyFree(L);
	long long *Lb = MyMalloc(sizeof(long long));
	*Lb = 4124;
	printf("%d\n%d\n", (unsigned)Base, (unsigned)Lb);

	return 0;
}
