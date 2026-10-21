#include <stdio.h>

typedef enum{
	VAR,
} keyword;

typedef enum{
	FUNCTION,
	IDENTIFIER,
	VARIABLE,
	KEYWORD,
} token_type;


int tokens(FILE* f, long file_size, token_type* output);