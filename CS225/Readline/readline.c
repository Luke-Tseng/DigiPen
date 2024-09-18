#include "readline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readline(FILE *stream)
{
	if (feof(stream))
	{
		return NULL;
	}
	int size = 0;
	int capacity = 2;
	char *buffer = calloc(capacity, sizeof(char));
	while (!feof(stream))
	{
		int read_size = capacity - size;
		char* result = fgets(buffer + size, read_size, stream);
		if (result == NULL)
		{
			break;
		}
		capacity *= 2;
		size = size + read_size - 1;
		char *temp = realloc(buffer, capacity * sizeof(char));
		for (int i = size; i < capacity; ++i)
		{
			*(temp + i) = 0;
		}
		buffer = temp;

		for (int i = 0; i < (int)strlen(buffer); ++i)
		{
			if (*(buffer + i) == '\n')
			{
				*(buffer + i) = '\0';
			}
		}
		if (*(buffer + size - 1) == '\0')
		{
			break;
		}
	}
	return buffer;
}
