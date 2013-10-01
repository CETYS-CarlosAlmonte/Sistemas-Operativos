#include <stdio.h>
#include <stdlib.h>

void RemoveComments(FILE *source, FILE *target)
{
	char buffer = fgetc(source);
	while (buffer != EOF)
	{
		if (buffer == '/')
		{
			buffer = fgetc(source);
			if (buffer == '/')
			{
				while (buffer != 10)
					buffer = fgetc(source);
			}
			else if (buffer == '*')
			{
				while (fgetc(source) != '*') { }
				buffer = fgetc(source);
				buffer = fgetc(source);
			}
			if (buffer != EOF)
				fputc(buffer, target);
		}
		else
			fputc(buffer, target);
		buffer = fgetc(source);
	}

	fclose(source);
	fclose(target);
}

int main (int argc, char **args)
{
	FILE *source, *target;
	source = fopen(args[1], "r");
	target = fopen("newfile.txt", "w+");

	RemoveComments(source, target);
}
