#include <coreutils.h>
#include <codegen.h>
#include "_internal_codegen.h"
#include <lexer.h>
#include <parser.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	uint8_t* bytes;
	uint8_t instruction_size;
} instruction_t;

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

instruction_t generate_var_creation(dynamic_array_t vars, node_t node);
instruction_t generate_if(dynamic_array_t vars, node_t node);
instruction_t generate_mov_r8m(dynamic_array_t vars,register_8_bit_t reg, token_t var);

uint8_t gen_node(dynamic_array_t instructions, dynamic_array_t vars, node_t node){
	instruction_t inst = {0};
	switch(node.token.type){
		case KEYWORD_VAR:
			inst = generate_var_creation(vars, node);
			for(int i = 0; i < 4; i++) fprintf(stdout, "%x ", inst.bytes[i]);
			fprintf(stdout, "\n");
			break;
		case KEYWORD_IF:
			inst = generate_if(vars, node);
			break;
		default:
			fprintf(stdout, "Node can't be compilated\n");
	}
	return inst.instruction_size;
}

instruction_t generate_var_creation(dynamic_array_t vars, node_t node){
	uint8_t* command = malloc(4);
	command[0] = 0xC6; 
	command[1] = 0b01000101; 
	command[2] = define_stack_offset(vars, node.args[0]);;
	command[3] = str_to_num(node.args[1].str_view, node.args[1].str_view_lenght);
	return (instruction_t){.bytes = command, .instruction_size = 4};
}


instruction_t generate_if(dynamic_array_t vars, node_t node){
	instruction_t  generated_if = generate_mov_r8m(vars, AL, node.args[0]);
	instruction_t tmp_instruction = generate_mov_r8m(vars, AH, node.args[2]);

	generated_if.bytes = realloc(generated_if.bytes, generated_if.instruction_size + tmp_instruction.instruction_size + 4);
	memcpy(generated_if.bytes + generated_if.instruction_size, tmp_instruction.bytes, tmp_instruction.instruction_size);
	generated_if.instruction_size += tmp_instruction.instruction_size;

	generated_if.bytes[generated_if.instruction_size] = 0x38;
	generated_if.bytes[generated_if.instruction_size + 1] = 0xE0;
	generated_if.bytes[generated_if.instruction_size + 2] = 0x75; //JNE opcode
	generated_if.bytes[generated_if.instruction_size + 3] = 0xFF; //placeholder for JNE
	generated_if.instruction_size += 4;

	/*
		---if part should be like this--
		push rax 

		mov ah, [rbp - offset]
		mov al, [rbp - offset-2]
		cmp ah, al # 0x38 0b11100000 

		pop rax
		jne brk
		--PART WITH CHILD NODES--
		brk:
	*/
	for(int i = 0; i < generated_if.instruction_size; i++){
		fprintf(stdout, "%x ", generated_if.bytes[i]);
	}
	return generated_if;
}

//rm - means from memory(variable) to r8
instruction_t generate_mov_r8m(dynamic_array_t vars,register_8_bit_t reg, token_t var){
	uint8_t* bytes = malloc(3);
	bytes[0] = 0x8A; // opcode
	bytes[1] = (0b01000101) | (reg << 3);
	bytes[2] = define_stack_offset(vars, var);
	return (instruction_t){.bytes = bytes, .instruction_size = 3};
}