#pragma once
#include <syntax.h>
#include <lexer.h>
#include <coreutils.h>


typedef struct{
	char* msg;
	int code;
} error_t;

typedef struct{
	const token_type* rules;
	int args_count;
	token_type token;
} node_rule_t;

node_rule_t* select_rule(token_t token, int* rules_lenght);