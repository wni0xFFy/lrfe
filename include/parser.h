#pragma once

#include <lexer.h>

typedef struct {
	token_t token;
	token_t* args;
	int args_count;
	token_t output;
} node_t;

dynamic_array_t nodes(dynamic_array_t* tokens); 
int check_keyword(dynamic_array_t arr, int keyword_index);
