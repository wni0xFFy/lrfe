#pragma once

#include <coreutils.h>
#include <stdint.h>
#include <parser.h>

typedef struct{
	uint8_t* bytes;
	uint8_t instruction_size;
} instruction_t;

uint8_t gen_node(dynamic_array_t* instructions, dynamic_array_t* vars, node_t node);
uint8_t give_adresses(dynamic_array_t* vars, dynamic_array_t nodes, uint8_t last_address_offset);
uint8_t generate_init(dynamic_array_t* instructions);
uint8_t generate_exit(dynamic_array_t* instructions);
dynamic_array_t create_elf_bytes(dynamic_array_t instructions);