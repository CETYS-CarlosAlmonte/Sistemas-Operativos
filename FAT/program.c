#include <stdlib.h>
#include <stdio.h>

#define SIZE_BLOCK 16

char usable_space[256]; // Where data will be stored.
int file_start_points[sizeof(usable_space) / SIZE_BLOCK]; // List of files REPRESENTED BY START POINT.
int indexes[sizeof(usable_space) / SIZE_BLOCK];

// Functions
int write(int nbytes); // To set indexes as occupated and their path.
void remove_file(int index); // Receives the start point of the fi;e to be removed.

int main(void)
{
	printf("Block size: %d\nDisk capacity: %d\nMax no. of files: %d\n", SIZE_BLOCK, sizeof(usable_space), sizeof(file_start_points)/sizeof(int));

	// Mark all indexes to emtpy (-2).
	int i; // "i" will start at 1, to maintain the idea of FAT32's structure. Being index #0 for the names list.
	for (i = 1; i < sizeof(indexes)/sizeof(int); i++)
		indexes[i] = -2;

	// Tests:
	write(20); // It requires index "1" and "2" so next file should start at "3" (if "1" it's not removed!).
	int last_write = write(32); // Ocupates "3 & 4" indexes.
	printf("%d\n", last_write); // If prints "3" then is the allocation is correct at this point.
	remove_file(2); // 2nd file, it's on index #2 at the "file_start_points" list.
	write(64);
	last_write = write(14); // Should ocupate #7.
	printf("%d\n", last_write);

	return 0;
}

int write(int nbytes)
{
	int start = 1, nindexes = sizeof(indexes)/sizeof(int);

	while (indexes[start] != -2 && start < nindexes)
		start++;

	// At this point there's at least 1 free block (start point). Then we'll look for the requested space (nbytes).
	if (start < nindexes)
	{
		int i, available_nbytes = (int)SIZE_BLOCK;
		char path[nindexes]; // To avoid using linked-list, like an struct with pointers.
		for (i = 1; i < nindexes; i++)
		{
			if (available_nbytes >= nbytes)
				break;
			else if (indexes[i] == -2 && i != start) // We already have the start point, we want the rest of the path.
			{
				available_nbytes += (int)SIZE_BLOCK;
				path[i] = 1; // Just to indicate that it's part of the path.
			}
		}

		if (available_nbytes >= nbytes)
		{
			int i, next = start; // "next" stores the index of the index[i] predecesor.
			for (i = 1; i < nindexes; i++)
			{
				if (path[i] == 1)
				{
 					indexes[next] = i;
					next = i;
				}
			}
			indexes[next] = -1; // End of the block.

			// Give the start file index the 1st index available in the "file_start_points".
			for (i = 1; i < nindexes; i++)
			{
				if (file_start_points[i] == 0)
				{
					file_start_points[i] = start; // Sets the file start point for the requested "file".
					break;
				}
			}
		}
		else
			start = -1;
	}
	else
		start = -1;

	return start; // Just to return a value that can be compared to an error, being -1 "failure", and something > -1 "success".
}

void remove_file(int index)
{
	int next = file_start_points[index];
	while (indexes[next] != -1)
	{
		//printf("R: %d\n", next);
		int remove = next;
		next = indexes[next];
		indexes[remove] = -2; // Available block index.
	}

	file_start_points[index] = 0; // Sets the start point as available.
}
