#include "_internal_codegen.h"
#include <codegen.h>
#include <coreutils.h>
#include <parser.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t give_adresses(dynamic_array_t* vars, dynamic_array_t nodes, uint8_t last_address_offset){
	node_t* nds = nodes.data;

	for(int i = 0; i < nodes.size; i++){
		if(nds[i].token.type == KEYWORD_VAR){
			variable_t var = (variable_t){
				.name = nds[i].args[0].str_view,
				.name_size = nds[i].args[0].str_view_lenght,
				.address_offset = last_address_offset - 1,
			};
			last_address_offset -= 1;
			if(push_variable(vars, var) == 1){
				fprintf(stdout,"damn wha?");
				return 1;
			}
		}
		if(nds[i].child_count > 0){
			dynamic_array_t childs = (dynamic_array_t){
				.data = nds[i].child,
				.size = nds[i].child_count,
			};
			last_address_offset = give_adresses(vars, childs, last_address_offset);
		}
	}
	return last_address_offset;
}
