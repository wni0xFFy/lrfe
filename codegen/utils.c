#include "_internal_codegen.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <lexer.h>

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