#include <syntax.h>
#include "_internal_syntax.h"
#include <lexer.h>
#include <coreutils.h>

uint8_t push_var(dynamic_array_t* vars, token_t var){
	check_array_capacity(vars, sizeof(variable_t));
	variable_t* variables = vars->data;

	variables[vars->size] = (variable_t){
		.name = var.str_view, 
		.name_size = var.str_view_lenght,
	};
	vars->size++;
	return 0;
}