#pragma once
#include <coreutils.h>
#include <stdint.h>
#include <lexer.h>

typedef struct{
	char* name;
	int name_size;
	uint8_t address_offset; //offset relatively RBP
}  variable_t;

dynamic_array_t give_adresses(dynamic_array_t nodes);
uint8_t str_to_num(char* str, int str_len);
uint8_t define_stack_offset(dynamic_array_t vars, token_t var);