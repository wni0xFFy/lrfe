#pragma once

#include <lexer.h>

//struct node_t;

typedef struct node_t{
	token_t token;
	token_t* args;
	int args_count;

	int child_count;
	struct node_t* child;

	uint8_t flags;
} node_t;

dynamic_array_t nodes(dynamic_array_t* tokens); 