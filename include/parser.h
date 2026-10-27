#pragma once

#include <lexer.h>

//struct node_t;

typedef struct node_t{
	token_t token;
	token_t* args;
	int args_count;
	struct node_t* child;
} node_t;

dynamic_array_t nodes(dynamic_array_t* tokens); 
int check_keyword(dynamic_array_t arr, int keyword_index);
