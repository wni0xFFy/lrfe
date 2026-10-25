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
	node.token = tokens[*current_pos];
	node.args = malloc(512);
	*current_pos += 1;
 	while(1 ){
 		if(tokens[*current_pos].type == NEWLINE) break;
		node.args[node.args_count] = tokens[*current_pos];
		node.args_count++;
		*current_pos += 1;
	}
	return node;
}
