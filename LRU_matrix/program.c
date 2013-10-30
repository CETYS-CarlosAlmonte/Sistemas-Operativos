#include <stdlib.h>
#include <stdio.h>

#define N 4
static char matrix[N];

void shift_matrix(int*);
void print_matrix(void);

int main(void)
{
        int access[10] = {0, 1, 2, 3, 2, 1, 0, 3, 2, 3};

	matrix[2] = 'A';
        int i;
        for (i = 0; i < 10; i++) {
                shift_matrix(&access[i]);
		print_matrix();
	}


        return 0;
}

void shift_matrix(int *i)
{
	matrix[*i] = 0b00001111; // Sets all the row[i] bits to '1'.
	char complement = 0b00000001 << ((N-1) - *i); // The '1' will ensure that in the 'i' position from left to right, there will be a zero.

	int k;
	for (k = 0; k < N; k++) {
		matrix[k] = matrix[k] ^ complement;
		char tmp = matrix[k] ^ complement; // One more XOR to check which value is the one that has the lowest value, in order to identify which one has the zore (objective).
		if (tmp < matrix[k])
			matrix[k] = tmp;
	}
}

void print_matrix()
{
	int i;
	for (i = 0; i < N; i++)
		printf("Row #%d: %d\n", i, (int)matrix[i]);
}
