#include <parser.h>
#include <stdio.h>
#include <coreutils.h>
#include <stdlib.h>

node_t assembly_arguments(token_t* tokens, int* current_pos);

dynamic_array_t nodes(dynamic_array_t* tokens){
	token_t* tks = tokens->data;

	dynamic_array_t nodes = {0};
	nodes.capacity = 16;
	nodes.size = 0;
	nodes.data = (node_t*)malloc(sizeof(node_t) * nodes.capacity);
	node_t* nds = nodes.data;

	for(int i = 0; i < tokens->size; i++){
		if(tks[i].type == FUNCTION || tks[i].type >= KEYWORD_VAR){
			nds[nodes.size] = assembly_arguments(tks, &i);
			nodes.size++;
		}
	}
	return nodes;
}

node_t assembly_arguments(token_t* tokens, int* current_pos){
	node_t node = {0};
	node_t* child = malloc(sizeof(node_t));

	node.token = tokens[*current_pos];
	node.args = malloc(512);
	node.child = child;
	child->args = malloc(512);
	child->args_count = 0;
	*current_pos += 1;
	int isBlock = 0;

 	while(1){
 		switch(tokens[*current_pos].type){
	 		case NEWLINE:
	 			return node;
	 			break;

	 		case OPEN_CODE_BLOCK:
				*current_pos += 1;	
				while(tokens[*current_pos].type != CLOSE_CODE_BLOCK){
					if(tokens[*current_pos].type == FUNCTION || tokens[*current_pos].type >= KEYWORD_VAR){
						child->token = tokens[*current_pos];
						//*child = assembly_arguments(tokens, current_pos);
					}
				 	child->args[child->args_count] = tokens[*current_pos];
				 	child->args_count++;
				 	*current_pos += 1;
			 	}
	 			break;

	 		default:
	 			node.args[node.args_count] = tokens[*current_pos];
				node.args_count++;
				break;
 		}

 		*current_pos += 1;
	}

	return node;
}
