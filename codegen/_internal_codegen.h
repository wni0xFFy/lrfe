#pragma once
#include <coreutils.h>
#include <stdint.h>
#include <lexer.h>

typedef enum{
	AL,
	CL,
	DL,
	BL,
	AH,
	CH,
	DH,
	BH,
} register_8_bit_t;

typedef enum{
	AX,
	CX,
	DX,
	BX,
	SP,
	BP,
	SI,
	DI,
} register_16_bit_t;

typedef struct{
	char* name;
	int name_size;
	uint8_t address_offset; //offset relatively RBP
}  variable_t;

typedef struct{
	uint8_t* bytes;
	uint8_t instruction_size;
} instruction_t;


uint8_t give_adresses(dynamic_array_t* vars, dynamic_array_t nodes, uint8_t last_address_offset);
uint8_t str_to_num(char* str, int str_len);
uint8_t define_stack_offset(dynamic_array_t vars, token_t var);
uint8_t push_instruction(dynamic_array_t* arr, instruction_t inst);
uint8_t push_variable(dynamic_array_t* arr, variable_t var);