#include <parser.h>
#include <stdio.h>
#include <coreutils.h>
#include <stdlib.h>

node_t assembly_arguments(dynamic_array_t* tokens, int current_pos);

//SEE THIS FUNCTION
dynamic_array_t nodes(dynamic_array_t* tokens){
	token_t* tks = tokens->data;

	dynamic_array_t nodes = {0};
	nodes.capacity = 16;
	nodes.size = 0;
	nodes.data = (node_t*)malloc(sizeof(node_t) * nodes.capacity);


	for(int i = 0; i < tokens->size; i++){
		if(tks[i].type == FUNCTION || tks[i].type >= KEYWORD_VAR){
			((node_t*)nodes.data)[nodes.size] = assembly_arguments(tokens, i);
			nodes.size++;
		}
	}
	return nodes;
}

node_t assembly_arguments(dynamic_array_t* tokens, int current_pos){
	token_t* tks = tokens->data;

	node_t node = {0};
	node.token = tks[current_pos];
	node.args = malloc(128);

	for(int i = current_pos + 1; i < tokens->size; i++){
		if(tks[i].type == FUNCTION || tks[i].type >= KEYWORD_VAR) return node;
		node.args[node.args_count] = tks[i];
		node.args_count++;
	}
}
