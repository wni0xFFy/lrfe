#pragma once
#include <syntax.h>
#include <lexer.h>
#include <coreutils.h>

typedef struct{
	char* name;
	int name_size;
} variable_t;

typedef struct{
	char* msg;
	int code;
} error_t;

typedef struct{
	token_type tok;
	uint8_t (*checker) (token_t token, dynamic_array_t* vars);
} token_rule_t;

typedef struct{
	const token_rule_t* rules;
	int args_count;
	token_type token;
} node_rule_t;

uint8_t push_var(dynamic_array_t* vars, token_t var);
node_rule_t* select_rule(token_t token, int* rules_lenght);