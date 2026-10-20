#include <stdio.h>

typedef enum{
	FUNCTION,
	IDENTIFIER,
} token_type;

int tokens(FILE* f, long file_size, token_type* output);