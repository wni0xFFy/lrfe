#include <coreutils.h>
#include <codegen.h>
#include "_internal_codegen.h"
#include <lexer.h>
#include <parser.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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

uint16_t gen_node(dynamic_array_t nodes){
	dynamic_array_t vars = give_adresses(nodes);

	node_t* nds = nodes.data;
	for(int i = 0; i < nodes.size; i++){
		switch(nds[i].token.type){
			case KEYWORD_VAR:
				instruction_t add_instruction = generate_var_creation(vars, nds[i]);
				for(int i = 0; i < 4; i++) fprintf(stdout, "%x ", add_instruction.bytes[i]);
				fprintf(stdout, "\n");
				break;
			case KEYWORD_IF:
				generate_if(vars, nds[i]);
				break;
			default:
				fprintf(stdout, "Node can't be compilated\n");
		}
	}
	return 0;
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
	
	instruction_t a = generate_mov_r8m(vars, AH, node.args[0]);
	for(int i = 0; i < a.instruction_size; i++) fprintf(stdout, "%x ", a.bytes[i]);
	fprintf(stdout, "\n");
	fprintf(stdout, "NEXT REGISTER\n");
	a = generate_mov_r8m(vars, AL, node.args[2]);
	for(int i = 0; i < a.instruction_size; i++) fprintf(stdout, "%x ", a.bytes[i]);
	fprintf(stdout, "\n");
	/*
		---if part should be like this--
		push rax 

		mov ah, [rbp - offset]
		mov al, [rbp - offset-2]

		cmp ah, al
		pop rax
		jne brk
		--PART WITH CHILD NODES--
		brk:
	*/
	return (instruction_t){.bytes = 0, .instruction_size = 0};
}

//rm - means from memory(variable) to r8
instruction_t generate_mov_r8m(dynamic_array_t vars,register_8_bit_t reg, token_t var){
	uint8_t* bytes = malloc(3);
	bytes[0] = 0x8A; // opcode
	bytes[1] = (0b01000101) | (reg << 3);
	bytes[2] = define_stack_offset(vars, var);
	return (instruction_t){.bytes = bytes, .instruction_size = 3};
}