#include "_internal_codegen.h"
#include <parser.h>
#include <lexer.h>
#include <codegen.h>
#include <stdlib.h>
#include <string.h>
//rm - means from memory(variable) to r8 LEGACY
instruction_t generate_mov_r8m(dynamic_array_t vars,register_8_bit_t reg, token_t var){
	static uint8_t bytes[] = {
		0x8A, // opcode
		0b01000101, //modr/m
		0 //placeholder
	};
	
	//dynamic data
	instruction_t inst = (instruction_t){.bytes = malloc(3), .instruction_size = 3};
	memcpy(inst.bytes, bytes, 3);
	inst.bytes[1] |= reg << 3;
	inst.bytes[2] = define_stack_offset(vars, var);
	return inst;
}

//r64b - means immb to 64-bit register
instruction_t generate_mov_r64b(instruction_t* output, register_64_bit_t reg, uint8_t* value){
	static uint8_t bytes[] = {
		0x48, //switching into x64 mode
		0xB8, //opcode
		0,0,0,0,0,0,0,0 //placeholder for value
	};

	//static data
	instruction_t* inst = output;
	if(output == NULL) inst = &((instruction_t){.bytes = malloc(10), .instruction_size = 0});
	memcpy(&inst->bytes[inst->instruction_size], bytes, 2);

	//dynamic data
	inst->bytes[inst->instruction_size + 1] += reg;
	inst->instruction_size += 2;

	memcpy(&inst->bytes[inst->instruction_size], value, 8);
	inst->instruction_size += 8;
	return *inst;
}

//r64m means memory address to 64-bit register
instruction_t generate_lea_r64m(dynamic_array_t vars, instruction_t* output, register_64_bit_t reg, token_t var){
	static uint8_t bytes[] = {
		0x48, //x64
		0x8D, //opcode
		0b1000101, //modr/m
		0 //placeholder
	};

	//static 
	instruction_t* inst = output;
	if(output == NULL) inst = &((instruction_t){.bytes = malloc(4), .instruction_size = 0});
	memcpy(&inst->bytes[inst->instruction_size], bytes, 3);

	//dynamic
	inst->instruction_size += 2;
	inst->bytes[inst->instruction_size] |= reg << 3;
	inst->bytes[inst->instruction_size + 1] = define_stack_offset(vars, var);
	inst->instruction_size += 2;
	
	return *inst;
}

instruction_t generate_syscall(instruction_t* output){
	static uint8_t bytes[] = {
		0x0F,
		0x05
	};

	instruction_t* inst = output;
	if(output == NULL) inst = &((instruction_t){.bytes = malloc(2), .instruction_size = 0});

	memcpy(inst->bytes + inst->instruction_size, bytes, 2);
	inst->instruction_size += 2;
	return *inst;
}

//r64r64 means from 64bit register to 64bit register
instruction_t generate_mov_r64r64(instruction_t* output,register_64_bit_t src, register_64_bit_t dest){
	static uint8_t bytes[3] = { 
		0x48, 
		0x89, //opcode
		0, //place holder for modrm
	};

	instruction_t* inst = output;
	if(output == NULL) inst = &((instruction_t){.bytes = malloc(3), .instruction_size = 0});

	memcpy(inst->bytes + inst->instruction_size, bytes, 3);
	inst->instruction_size += 3;
	inst->bytes[inst->instruction_size - 1] = (0b11000000 | (src << 3)) | dest;
	return *inst;
}