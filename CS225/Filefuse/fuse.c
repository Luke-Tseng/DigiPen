#include "fuse.h"
#include <stdio.h>	/* fopen/fclose/fread/fwrite */
#include <stdlib.h> /* malloc/free */
#include <string.h> /* strlen */

#define xDEBUG

int fuse(char const **filenames, int num_files, char const *output)
{
	const char *name = filenames[0];
	int BUFFER_SIZE = 1 << 16;
	char *buffer;
	FILE *stream = fopen(output, "w");
	FILE *current;
	int i = 0;
	for (; i < num_files; ++i)
	{
		name = filenames[i];
		current = fopen(name, "r");
		fwrite(name, 1, strlen(name) + 1, stream); /* writing file name */
		fseek(current, 0, SEEK_END);			   /* finding file size */
		BUFFER_SIZE = ftell(current);
		buffer = calloc(BUFFER_SIZE, sizeof(char));
		fseek(current, 0, SEEK_SET);
		fwrite(&BUFFER_SIZE, 1, 4, stream); /* writing file size */
		while (BUFFER_SIZE > 0)				/* looping so it can work with any type of file */
		{
			if (BUFFER_SIZE >= 8)
			{
				fread(buffer, 8, 1, current); /* writing all file data into the fuse file */
				fwrite(buffer, 1, 8, stream);
			}
			else
			{
				fread(buffer, BUFFER_SIZE, 1, current);
				fwrite(buffer, 1, BUFFER_SIZE, stream);
			}
			BUFFER_SIZE -= 8;
		}
		free(buffer);
		fclose(current);
	}
	fclose(stream);
	return 0;
}

int unfuse(char const *filename)
{
	FILE *stream = fopen(filename, "r");
	FILE *current;
	char letter = ' ';
	char *name = "out.txt";
	char *buffer;
	int size = 0;
	int i = 0;
	while (!feof(stream))
	{
		name = calloc(256, sizeof(char));
		if (letter != ' ')
		{
			name[0] = letter;
			i = 1;
		}
		while (letter != 0) /* getting name of file */
		{
			fread(&letter, 1, 1, stream);
			name[i] = letter;
			++i;
		}
		fread(&size, 1, 4, stream); /* reads size of file to make */
		current = fopen(name, "w");
		buffer = calloc(8, sizeof(char));
		while (size > 0) /* keeps reading and writing while size is above 0 */
		{
			if (size >= 8)
			{
				fread(buffer, 1, 8, stream);
				fwrite(buffer, 1, 8, current);
			}
			else
			{
				fread(buffer, 1, size, stream);
				fwrite(buffer, 1, size, current);
			}
			size -= 8;
		}
		fread(&letter, 1, 1, stream); /* puts new line char into letter */
		free(buffer);
		buffer = NULL;
		free(name);
		name = NULL;
		fclose(current);
	}
	fclose(stream);
	return 0;
}