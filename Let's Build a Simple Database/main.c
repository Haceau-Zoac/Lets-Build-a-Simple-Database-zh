#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "getline.h"

typedef struct
{
	char* buffer;
	size_t buffer_length;
	ssize_t input_length;
} InputBuffer;

InputBuffer* new_input_buffer(void);
void print_prompt(void);
void read_input(InputBuffer* input_buffer);
void close_input_buffer(InputBuffer* input_buffer);

int main(void)
{
	InputBuffer* input_buffer = new_input_buffer();
	if (input_buffer == NULL)
	{
		puts("open failed.");
		return EXIT_FAILURE;
	}

	while (true)
	{
		print_prompt();
		read_input(input_buffer);

		if (!strcmp(input_buffer->buffer, ".exit"))
		{
			close_input_buffer(input_buffer);
			exit(EXIT_SUCCESS);
		}
		else
		{
			printf("Unrecognized command '%s'.\n", input_buffer->buffer);
		}
	}

	return EXIT_SUCCESS;
}

InputBuffer* new_input_buffer(void)
{
	InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
	if (input_buffer != NULL)
	{
		input_buffer->buffer = NULL;
		input_buffer->buffer_length = 0;
		input_buffer->input_length = 0;
	}

	return input_buffer;
}

void print_prompt(void)
{
	fputs("db > ", stdout);
}

void read_input(InputBuffer* input_buffer)
{
	ssize_t bytes_read =
		getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

	if (bytes_read <= 0)
	{
		puts("Error reading input");
		exit(EXIT_FAILURE);
	}

	input_buffer->input_length = bytes_read - 1;
	input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer)
{
	free(input_buffer->buffer);
	free(input_buffer);
}