#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (unlink(argv[1]) == 0)
		return 0;
	else
		return 1; // Error trying to delete file.
}
