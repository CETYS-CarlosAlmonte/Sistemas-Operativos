#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (!argv[1])
	{
		printf("No source supplied.\n");
		return 1;
	}
	else if (!argv[2])
	{
		printf("No destination was supplied.\n");
		return 1;
	}
	else
	{
		// Building new file location string + original filename.
		char new_location[strlen(argv[1])+strlen(argv[2])+1];
		strcpy(new_location, argv[2]);
		strcat(new_location, "/");
		strcat(new_location, argv[1]);

		int source = open(argv[1], O_RDONLY);
		open(new_location, O_CREAT);
		int target = open(new_location, O_WRONLY);
		int received;
		char buffer[256];

		while ((received = read(source, buffer, sizeof(buffer))) > 0)
			write(target, buffer, received);

		unlink(argv[1]);


		return 0;
	}
}
