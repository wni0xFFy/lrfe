#include "_internal_codegen.h"
#include <parser.h>
#include <lexer.h>
#include <codegen.h>
#include <stdlib.h>
#include <string.h>
//rm - means from memory(variable) to r8
instruction_t generate_mov_r8m(dynamic_array_t vars,register_8_bit_t reg, token_t var){
	uint8_t* bytes = malloc(3);
	bytes[0] = 0x8A; // opcode
	bytes[1] = (0b01000101) | (reg << 3);
	bytes[2] = define_stack_offset(vars, var);
	return (instruction_t){.bytes = bytes, .instruction_size = 3};
}

//r64b - means immb to 64-bit register
instruction_t generate_mov_r64b(instruction_t* output, register_64_bit_t reg, uint8_t value){
	if(output != NULL){
		output->bytes[output->instruction_size] = 0x48;
		output->bytes[output->instruction_size + 1] = 0xB8 + reg;
		output->bytes[output->instruction_size + 2] = value;
        memset(output->bytes + 3, 0, 7);
		output->instruction_size += 10;
	}
	uint8_t* bytes = calloc(10, 1);
	bytes[0] = 0x48;
	bytes[1] = 0xB8 + reg;
	bytes[2] = value;
    memset(bytes + 3, 0, 7);
	return (instruction_t){
		.bytes = bytes,
		.instruction_size = 10,
	};
}

//r64m means memory address to 64-bit register
instruction_t generate_lea_r64m(dynamic_array_t vars, instruction_t* output, register_64_bit_t reg, token_t var){
	if(output != NULL){
		output->bytes[output->instruction_size] = 0x48;
		output->bytes[output->instruction_size + 1] = 0x8D;
		output->bytes[output->instruction_size + 2] = 0b01000101 | (reg << 3);
		output->bytes[output->instruction_size + 3] = define_stack_offset(vars, var);
		output->instruction_size += 4;
	}
	uint8_t* bytes = malloc(4);
	bytes[0] = 0x48;
	bytes[1] = 0x8D;
	bytes[2] =  0b01000101 | (reg << 3);
	bytes[3] = define_stack_offset(vars, var);
	return (instruction_t){
		.bytes = bytes,
		.instruction_size = 4,
	};
}

instruction_t generate_syscall(instruction_t* output){
	if(output != NULL){
		output->bytes[output->instruction_size] = 0x0F;
		output->bytes[output->instruction_size + 1] = 0x05;
		output->instruction_size += 2;
	}
	uint8_t* bytes = malloc(2);
	bytes[0] = 0x0F;
	bytes[1] = 0x05;
	return (instruction_t){
		.bytes = bytes,
		.instruction_size = 2,
	};
}

//r64r64 means from 64bit register to 64bit register
instruction_t generate_mov_r64r64(instruction_t* output,register_64_bit_t src, register_64_bit_t dest){
	if(output != NULL){
		output->bytes[output->instruction_size] = 0x48;
		output->bytes[output->instruction_size + 1] = 0x8C;
		output->bytes[output->instruction_size + 1] = (0b11000000 | (src << 3)) | dest;
		output->instruction_size += 3;
	}
	uint8_t* bytes = malloc(3);
	bytes[0] = 0x48;
	bytes[1] = 0x89; //opcode
	bytes[2] = (0b11000000 | (src << 3)) | dest;
	return (instruction_t){
		.bytes = bytes,
		.instruction_size = 3,
	};
}