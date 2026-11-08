#include "_internal_codegen.h"
#include <codegen.h>
#include <coreutils.h>
#include <parser.h>
#include <stdint.h>
#include <stdlib.h>

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
