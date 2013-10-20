#include <stdlib.h>
#include <stdio.h>

static char memory[8];
static char bitmap;

int first_empty_slot(char *bm);

int main(void)
{
	bitmap = 0b00000000;

	// Arbitrary fill some slots of memory:
	memory[0] = 1;
	memory[1] = 1;
	memory[5] = 1;
	memory[7] = 1;

	// Setting the bitmap according to the memory slots that are used.
	int i;
	for (i = 0; i < sizeof(memory); i++) {
		if (memory[i])
			bitmap = bitmap | 0b00000001 << i;
	}

	int emptyslot = first_empty_slot(&bitmap);

	printf("The 1st available location of memory is at index #%d\n", emptyslot);

	return 0;
}

int first_empty_slot(char *bm)
{
	int i, empty = -1;
	for (i = 0; i < 8; i++) {
		char c = *bm & (0b00000001 << i);
		if (!c) {
			empty = i; // Index found.
			break;
		}
	}

	return empty;
}
