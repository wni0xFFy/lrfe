#include <parser.h>
#include <stdio.h>
#include <coreutils.h>
#include <stdlib.h>

node_t assembly_arguments(token_t* tokens, int* current_pos, int tokens_size);

dynamic_array_t nodes(dynamic_array_t* tokens){
	token_t* tks = tokens->data;

	dynamic_array_t nodes = {0};
	nodes.capacity = 16;
	nodes.size = 0;
	nodes.data = (node_t*)malloc(sizeof(node_t) * nodes.capacity);
	node_t* nds = nodes.data;

	for(int i = 0; i < tokens->size; i++){
		if(tks[i].type == FUNCTION || tks[i].type >= KEYWORD_VAR){
			nds[nodes.size] = assembly_arguments(tks, &i, tokens->size);
			nodes.size++;
		}
	}
	return nodes;
}

node_t assembly_arguments(token_t* tokens, int* current_pos, int tokens_size){
	node_t node = {0};
	node_t* child = malloc(sizeof(node_t));

	node.token = tokens[*current_pos];
	node.args = malloc(512);
	node.child = child;
	child->args = malloc(1024);
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
				fprintf(stderr, "%d", tokens_size);
				while(tokens[*current_pos].type != CLOSE_CODE_BLOCK && *current_pos < tokens_size - 1){ //i don't know WHY -1 but if you delete it without '}' program shutdown with bus error 
					if(tokens[*current_pos].type == NEWLINE) {*current_pos += 1; continue;} //here should be offsetting
					if(tokens[*current_pos].type == FUNCTION || tokens[*current_pos].type >= KEYWORD_VAR){
						child->token = tokens[*current_pos];
						/*
							change this logic  WHEN NEWLINE the child should be 
							saved like in node_t should be array of children(or dynamic_array_t or pointer + size)
						*/

						*current_pos += 1;
						continue;
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
