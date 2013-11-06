#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd = open(argv[1], O_RDONLY);
	if (fd != -1)
	{
		char buffer[256];
		open("newfile.txt", O_CREAT);
		int fd_n = open("newfile.txt", O_WRONLY);
		int received;
		while ((received = read(fd, buffer, sizeof(buffer))) > 0)
			write(fd_n, buffer, received);
	}
	else
		printf("File %s does not exists.\n", argv[1]);


	return 0;
}
