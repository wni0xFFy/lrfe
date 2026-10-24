#pragma once
#include <coreutils.h>
typedef enum{
	FUNCTION,
	IDENTIFIER,
	VARIABLE,
	KEYWORD_VAR,
} token_type;

typedef struct{
	token_type type; 
	char* str_view;
	int str_view_lenght;
} token_t;

dynamic_array_t tokens(char* input, long file_size);