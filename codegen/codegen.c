#include <coreutils.h>
#include <codegen.h>
#include <lexer.h>
#include <parser.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
	char* name;
	int name_size;
	uint8_t address_offset; //offset relatively RBP
}  variable_t;

dynamic_array_t give_adresses(dynamic_array_t nodes);
uint8_t*  generate_var_creation(dynamic_array_t vars, node_t node);
uint8_t define_offset(dynamic_array_t vars, token_t var);
uint8_t str_to_num(char* str, int str_len);

uint16_t gen_node(dynamic_array_t nodes){
	dynamic_array_t arr = give_adresses(nodes);

	variable_t* vars = arr.data;
	//fprintf(stdout, "s : %d\n", ar);
	for(int i = 0; i < arr.size; i++){
		fprintf(stdout, "0x%x\n", vars[i].address_offset);
	}
	node_t* nds = nodes.data;
	for(int i = 0; i < nodes.size; i++){
		switch(nds[i].token.type){
			case KEYWORD_VAR:
				uint8_t* c = generate_var_creation(arr, nds[i]);
				for(int i = 0; i < 4; i++) fprintf(stdout, "%x ", c[i]);
				fprintf(stdout, "\n");
		}
	}
	return 0;
}

dynamic_array_t give_adresses(dynamic_array_t nodes){
	node_t* nds = nodes.data;

	uint8_t last_address_offset = 0;
	variable_t* vars = malloc(sizeof(variable_t) * 16);
	dynamic_array_t arr = (dynamic_array_t){
		.data = vars,
		.size = 0,
		.capacity = 16
	};

	for(int i = 0; i < nodes.size; i++){
		if(nds[i].token.type == KEYWORD_VAR){
			if(arr.size >= arr.capacity){
				arr.capacity += 16;
				vars = realloc(vars, sizeof(variable_t) * arr.capacity);
			}

			vars[arr.size] = (variable_t){
				.name = nds[i].args[0].str_view,
				.name_size = nds[i].args[0].str_view_lenght,
				.address_offset = last_address_offset - 1,
			};
			arr.size += 1;
			last_address_offset -= 1;
		}
	}
	return arr;
}

uint8_t* generate_var_creation(dynamic_array_t vars, node_t node){
	uint8_t num = str_to_num(node.args[1].str_view, node.args[1].str_view_lenght);
	uint8_t modrm = 0b01000101;
	uint8_t offset = define_offset(vars, node.args[0]);

	uint8_t* command= malloc(4);
	command[0] = 0xC6; 
	command[1] =  modrm; 
	command[2] = offset; 
	command[3] = num;
	return command;
}

uint8_t str_to_num(char* str, int str_len){
	char* num_str = malloc(str_len + 1);
	memcpy(num_str, str, str_len);
	num_str[str_len] = '\0';

	return atoi(num_str);
}

uint8_t define_offset(dynamic_array_t vars, token_t var){
	variable_t* vrs = vars.data;
	for(int i = 0; i < vars.size; i++){
		if(vrs[i].name_size == var.str_view_lenght){
			if(memcmp(vrs[i].name, var.str_view, vrs[i].name_size) == 0){
				return vrs[i].address_offset;
			}
		}
	}
}