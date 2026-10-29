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

	node.token = tokens[*current_pos];
	node.args = malloc(512);
	node.child = malloc(sizeof(node_t) * 16);
	node.child->args = malloc(sizeof(token_t) * 32);
	node.child->args_count = 0;
	node.childs = 0;
	*current_pos += 1;

	node_t* child = node.child;

 	while(1){
 		switch(tokens[*current_pos].type){
	 		case NEWLINE:
	 			return node;
	 		break;

	 		case OPEN_CODE_BLOCK:
			 	*current_pos += 1;	
				while(*current_pos < tokens_size){
					if(tokens[*current_pos].type == CLOSE_CODE_BLOCK){fprintf(stdout, "im not human\n");break;}
					if(tokens[*current_pos].type == NEWLINE){
						child++;
						child->args = malloc(sizeof(token_t) * 32);;
						child->args_count = 0;
						node.childs++;
					}
					else if(tokens[*current_pos].type == FUNCTION || tokens[*current_pos].type >= KEYWORD_VAR){
					  	child[node.childs].token = tokens[*current_pos];
					}		
				 	else {
				 		int count = child->args_count;
				 		token_t* ch = child->args;
				 		token_t tk = tokens[*current_pos];

				 	 	ch[count] = tk;

				 	 	child->args_count += 1;
				 	 }
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
