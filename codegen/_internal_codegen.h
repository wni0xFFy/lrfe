#pragma once
#include <coreutils.h>
#include <stdint.h>
#include <lexer.h>
#include <codegen.h>

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

typedef enum{
	RAX,
	RCX,
	RDX,
	RBX,
	RSP,
	RBP,
	RSI,
	RDI,
} register_64_bit_t;

typedef struct{
	char* name;
	int name_size;
	uint8_t address_offset; //offset relatively RBP
}  variable_t;


uint8_t give_adresses(dynamic_array_t* vars, dynamic_array_t nodes, uint8_t last_address_offset);
uint8_t str_to_num(char* str, int str_len);
uint8_t define_stack_offset(dynamic_array_t vars, token_t var);
uint8_t push_instruction(dynamic_array_t* arr, instruction_t inst);
uint8_t push_variable(dynamic_array_t* arr, variable_t var);

instruction_t generate_mov_r8m(dynamic_array_t vars,register_8_bit_t reg, token_t var);
instruction_t generate_mov_r64b(instruction_t* output, register_64_bit_t reg, uint8_t value);
instruction_t generate_lea_r64m(dynamic_array_t vars, instruction_t* output, register_64_bit_t reg, token_t var);
instruction_t generate_syscall(instruction_t* output);
instruction_t generate_mov_r64r64(instruction_t* output,register_64_bit_t src, register_64_bit_t dest);