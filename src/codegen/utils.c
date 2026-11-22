#include "_internal_codegen.h"
#include <string.h>
#include <stdint.h>
#include <codegen.h>
#include <coreutils.h>
#include <stdlib.h>
#include <lexer.h>
#include <stdio.h>

uint8_t str_to_num(char* str, int str_len){
	char* num_str = malloc(str_len + 1);
	memcpy(num_str, str, str_len);
	num_str[str_len] = '\0';
    uint8_t num = atoi(num_str);
    free(num_str);
	return num;
}

uint8_t define_stack_offset(dynamic_array_t vars, token_t var){
	variable_t* vrs = vars.data;
	for(int i = 0; i < vars.size; i++){
		if(vrs[i].name_size == var.str_view_lenght && memcmp(vrs[i].name, var.str_view, vrs[i].name_size) == 0)
			return vrs[i].address_offset;
	}
	
	return 0;
}

uint8_t push_instruction(dynamic_array_t* arr, instruction_t inst){
	check_array_capacity(arr, sizeof(instruction_t));
	((instruction_t*)arr->data)[arr->size] = inst;
	arr->size += 1;
	return 0;
}

uint8_t push_variable(dynamic_array_t* arr, variable_t var){
	check_array_capacity(arr, sizeof(variable_t));
	((variable_t*)arr->data)[arr->size] = var;
	arr->size += 1;
	return 0;
}