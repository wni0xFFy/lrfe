#include <syntax.h>
#include <parser.h>
#include <lexer.h>
#include <stdio.h>
#include <coreutils.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char* literal;
	int literal_size;
	char* name;
	int name_size;
} variable_t;

typedef struct{
	char* msg;
	int code;
} error_t;

error_t is_var(node_t node, dynamic_array_t* vars);
error_t is_print(node_t node, dynamic_array_t* vars);
variable_t* check_var(dynamic_array_t vars, token_t var);

int syntax_checker(dynamic_array_t* _nodes){
	dynamic_array_t vars = {.data = malloc(sizeof(variable_t) * 32), .size = 0};

	node_t* nodes = _nodes->data;
	error_t (*checkers[]) (node_t node, dynamic_array_t* vars) = {
		is_var,	
		is_print,
	};

   for(node_t* node = nodes; node < nodes + _nodes->size ; node++){
   		for(unsigned long i = 0; i < sizeof(checkers) / sizeof(checkers[0]); i++){
   			error_t err = checkers[i](*node, &vars);
   			if(err.code == 2) fprintf(stdout, "%s\n", err.msg);
   		}
	}
	return 1;
}

error_t is_var(node_t node, dynamic_array_t* vars){
	variable_t* variables = vars->data;
	if(node.token.type != KEYWORD_VAR) 
		return (error_t){.code = 0, .msg = 0};

	if(node.args_count == 2){
		if(node.args[0].type == VARIABLE && node.args[1].type == NUM_LITERAL) 
			if(check_var(*vars, node.args[0]) != NULL)
				return (error_t){.code = 2, .msg = "variable already exists"};

			variables[vars->size] = (variable_t){
				.name = node.args[0].str_view, 
				.name_size = node.args[0].str_view_lenght,
				.literal = node.args[1].str_view,
				.literal_size = node.args[1].str_view_lenght,
			};
			vars->size++;

			return (error_t){.code = 1, .msg = 0};
	}

	return (error_t){.code = 2, .msg = "invalid 'var' using"};
}

error_t is_print(node_t node, dynamic_array_t* vars){
	if(node.token.type != KEYWORD_PRINT) 
		return (error_t){.code = 0, .msg = 0};

	if(node.args_count == 1){
		if(node.args[0].type == VARIABLE) {
			variable_t* var = check_var(*vars, node.args[0]);
			if(var != NULL) return (error_t){.code = 1, .msg = 0};
			else return (error_t){.code = 2, .msg = "unknown variable."};
		}
	}

	return (error_t){.code = 2, .msg = "invalid 'print' using"};
 }

variable_t* check_var(dynamic_array_t vars, token_t var){
	variable_t* v = vars.data;
	for(int i = 0; i < vars.size; i++){
		if(memcmp(v[i].name, var.str_view, var.str_view_lenght) == 0){
			return v + i;
		}
	}

	return NULL;
}