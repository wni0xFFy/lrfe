#include <syntax.h>
#include <parser.h>
#include <lexer.h>
#include <stdio.h>
#include <coreutils.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char* name;
	int name_size;
} variable_t;

typedef struct{
	char* msg;
	int code;
} error_t;

/* 
		template : {KEYWORD_VAR, NUM_LITERAL, STRING}
		arguments count : 1
		and it will works like
		CHECK ARGUMENTS_COUNT 
			IF ERROR:
				CHECK OTHER(OFFSET ARGUMENT TO ARGUMENTS_COUNT)
			OTHERWISE:
				RETURN SUCCESS
	
*/

error_t is_var(node_t node, dynamic_array_t* vars);
error_t is_print(node_t node, dynamic_array_t* vars);
error_t is_if(node_t node, dynamic_array_t* vars);
variable_t* check_var(dynamic_array_t vars, token_t var);
void check_syntax_node(int* current_line, int* errors_count, node_t node, dynamic_array_t* vars);

int syntax_checker(dynamic_array_t* _nodes){
	int errors_count = 0;
	dynamic_array_t vars = {.data = malloc(sizeof(variable_t) * 32), .size = 0};

	node_t* nodes = _nodes->data;

    for(int i = 1; i <= _nodes->size; i++){
   		check_syntax_node(&i, &errors_count, nodes[i - 1], &vars);
	}
	delete_array(vars);
	return errors_count;
}

void check_syntax_node(int* current_line, int* errors_count, node_t node, dynamic_array_t* vars){
	if(node.flags != 0){
		fprintf(stdout, "line %d : unclosed block\n", *current_line);
		*errors_count += 1;
	}
	error_t (*checkers[]) (node_t node, dynamic_array_t* vars) = {
		is_var,	
		is_print,
		is_if,
	};
	for(unsigned long j = 0; j < sizeof(checkers) / sizeof(checkers[0]); j++){
		error_t err = checkers[j](node, vars);
		if(err.code == 2){
				fprintf(stdout, "line %d : %s\n", *current_line ,err.msg);
				*errors_count += 1;
		}
   	}
	for(int k = 0; k < node.child_count; k++){
			*current_line += 1;
			check_syntax_node(current_line, errors_count, node.child[k], vars);
	}
}

error_t is_var(node_t node, dynamic_array_t* vars){
	variable_t* variables = vars->data;

	if(node.token.type != KEYWORD_VAR) 
		return (error_t){.code = 0, .msg = 0};

	if(node.args_count == 2){
		if(node.args[0].type == VARIABLE && node.args[1].type == NUM_LITERAL) {
			if(check_var(*vars, node.args[0]) != NULL){
				return (error_t){.code = 2, .msg = "variable already exists"};
			}
			
			check_array_capacity(vars, sizeof(variable_t));
			variables = vars->data;

			variables[vars->size] = (variable_t){
				.name = node.args[0].str_view, 
				.name_size = node.args[0].str_view_lenght,
			};
			vars->size++;

			return (error_t){.code = 1, .msg = 0};
		}
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

error_t is_if(node_t node, dynamic_array_t* vars){
	if(node.token.type != KEYWORD_IF) {
		return (error_t){.code = 0, .msg = 0}; 
	}

	//checking for avoid segfault below
	if(node.args_count != 3) {
		return (error_t){.code = 2, .msg = "invalid 'if' using"};
	}

	//checking is argument variable and does that var exists
	if(node.args[0].type == VARIABLE){
		variable_t* var = check_var(*vars, node.args[0]);
		if(var == NULL) return (error_t){.code = 2, .msg = "unknown variable."};
	}
	else {
		return (error_t){.code = 2, .msg = "invalid 'if' using"};
	}

	if(node.args[2].type == VARIABLE){
		variable_t* var = check_var(*vars, node.args[2]);
		if(var == NULL) return (error_t){.code = 2, .msg = "unknown variable."};
	}
	else {
		return (error_t){.code = 2, .msg = "invalid 'if' using"};
	}

	if(node.args[1].type != OPERATOR_EQUAL || node.child_count < 1){
		return (error_t){.code = 2, .msg = "invalid 'if' using"};
	}

	return (error_t){.code = 1, .msg = 0};
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