#include <parser.h>
#include <stdio.h>
#include <coreutils.h>
#include <stdlib.h>

node_t assembly_arguments(token_t* tokens, int* current_pos, int tokens_size);
inline int is_keyword(token_t tk);

dynamic_array_t nodes(dynamic_array_t* tokens){
	token_t* tks = tokens->data;

	dynamic_array_t nodes = {0};
	nodes.capacity = 16;
	nodes.size = 0;
	nodes.data = (node_t*)malloc(sizeof(node_t) * nodes.capacity);

	for(int i = 0; i < tokens->size; i++){
		if(tks[i].type == FUNCTION || is_keyword(tks[i])){
			((node_t*)nodes.data)[nodes.size] = assembly_arguments(tks, &i, tokens->size);
			nodes.size++;
			if(nodes.size == nodes.capacity){
				check_array_capacity(&nodes, sizeof(node_t));
			}
		}
	}
	return nodes;
}

node_t assembly_arguments(token_t* tokens, int* current_pos, int tokens_size){
	node_t node = {0};

	node.token = tokens[*current_pos];
	node.args_count = 0;

	*current_pos += 1;

 	while(1){ 
 		switch(tokens[*current_pos].type){
	 		case NEWLINE:
	 			return node;
	 		break;

	 		case CLOSE_CODE_BLOCK:
	 			return node;
	 		break;

	 		case OPEN_CODE_BLOCK:
	 			//assemblying code block via recursion
	 			node.child = malloc(sizeof(node_t));
	 			node.child_count = 0;
	 			*current_pos += 1;

	 			while(tokens[*current_pos].type != CLOSE_CODE_BLOCK){
	 				if(tokens[*current_pos].type == NEWLINE) *current_pos += 1;
	 				if(tokens[*current_pos].type == CLOSE_CODE_BLOCK) break;

					node.child = realloc(node.child, (node.child_count + 1) * sizeof(node_t));
	 				node.child[node.child_count] = assembly_arguments(tokens, current_pos, tokens_size);
	 				node.child_count += 1;

	 			}
	 		break;


	 		default:
	 			//add token to current node arguments
	 			node.args = realloc(node.args, (node.args_count + 1) * sizeof(token_t));
	 			if(node.args == NULL) return node;	 	
	 			node.args[node.args_count] = tokens[*current_pos];
	 			node.args_count += 1;
			break;
 		}

 		*current_pos += 1;
	}
	return node;
}

inline int is_keyword(token_t tk){
	if(tk.type > KEYWORDS_GROUP_START && tk.type < KEYWORDS_GROUP_END) return 1;
	return 0;
}