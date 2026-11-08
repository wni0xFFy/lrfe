#include <coreutils.h>
#include <codegen.h>
#include "_internal_codegen.h"
#include <lexer.h>
#include <parser.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


instruction_t generate_var_creation(dynamic_array_t vars, node_t node);
instruction_t generate_if_prologue(dynamic_array_t vars, node_t node);
instruction_t generate_print(dynamic_array_t vars, node_t node);

uint8_t generate_init(dynamic_array_t* instructions){
	push_instruction(instructions, generate_mov_r64r64(NULL, RSP, RBP));
	return 0;
}

uint8_t generate_exit(dynamic_array_t* instructions){
	push_instruction(instructions,generate_mov_r64b(NULL, RAX, 60));
	push_instruction(instructions,generate_mov_r64b(NULL, RDI, 0));
	push_instruction(instructions,generate_syscall(NULL));
	return 0;
}

uint8_t gen_node(dynamic_array_t* instructions, dynamic_array_t* vars, node_t node){
	instruction_t tmp_inst = {0};
	switch(node.token.type){
		case KEYWORD_VAR:
			tmp_inst = generate_var_creation(*vars, node);
			push_instruction(instructions, tmp_inst);
			break;
		case KEYWORD_IF:
			tmp_inst = generate_if_prologue(*vars, node);
			push_instruction(instructions, tmp_inst);
			uint8_t* ptr_to_place = &(((instruction_t*)instructions->data)[instructions->size - 1].bytes[((instruction_t*)instructions->data)[instructions->size - 1].instruction_size - 1]);
			uint8_t offset = 0;
			for(int i = 0; i < node.child_count; i++){
				offset += gen_node(instructions, vars, node.child[i]);
			}
			*ptr_to_place = offset;
			break;
		case KEYWORD_PRINT:
			tmp_inst = generate_print(*vars, node);
			push_instruction(instructions, tmp_inst);
			break;
		default:
			fprintf(stdout, "Node can't be compilated\n");
	}
	return tmp_inst.instruction_size;
}

instruction_t generate_var_creation(dynamic_array_t vars, node_t node){
	uint8_t* command = malloc(4);
	command[0] = 0xC6; 
	command[1] = 0b01000101; 
	command[2] = define_stack_offset(vars, node.args[0]);
	command[3] = str_to_num(node.args[1].str_view, node.args[1].str_view_lenght);
	return (instruction_t){.bytes = command, .instruction_size = 4};
}

instruction_t generate_if_prologue(dynamic_array_t vars, node_t node){
	instruction_t  generated_if = generate_mov_r8m(vars, AL, node.args[0]);
	instruction_t tmp_instruction = generate_mov_r8m(vars, AH, node.args[2]);

	generated_if.bytes = realloc(generated_if.bytes, generated_if.instruction_size + tmp_instruction.instruction_size + 4);
	memcpy(generated_if.bytes + generated_if.instruction_size, tmp_instruction.bytes, tmp_instruction.instruction_size);
	generated_if.instruction_size += tmp_instruction.instruction_size;

	generated_if.bytes[generated_if.instruction_size] = 0x38; //CMP opcode
	generated_if.bytes[generated_if.instruction_size + 1] = 0xE0; //modr/m for cmp(means that we compare al and ah 8 bit registers)
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
	return generated_if;
}

instruction_t generate_print(dynamic_array_t vars, node_t node){
	instruction_t inst = {
		.bytes = calloc(36, 1),
		.instruction_size = 0
	};
	generate_mov_r64b(&inst, RAX, 1);
	generate_mov_r64b(&inst, RDI, 1);
	generate_mov_r64b(&inst, RDX, node.args[0].str_view_lenght);
	generate_lea_r64m(vars, &inst, RSI, node.args[0]);
	generate_syscall(&inst);
	return inst;
}