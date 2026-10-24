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

	//WHILE TOKEN NOT EQUAL FUNCTION OR KEYWORD - ADD TOKENS LIKE ARGUMENTS
	for(int i = current_pos + 1; i < tokens->size; i++){
		if(tks[i].type == FUNCTION || tks[i].type >= KEYWORD_VAR) return node;
		node.args[node.args_count] = tks[i];
		node.args_count++;
	}
	//in the end the last one word will be output
}

int check_keyword(dynamic_array_t arr, int keyword_index){
	arr.data += keyword_index;
	token_t* tokens = (token_t*)arr.data;
	/*TODO короче мы все что идет после keyword до 
	\n(aka next line) запихиваем как аргументы и то что нужно этому 
	keyword и уже масками проверяем правильность */
	switch(tokens[0].type){
		case KEYWORD_VAR:
			if(tokens[1].type == VARIABLE && tokens[2].type == IDENTIFIER) return 0;
			break;
	}

	return 1;
}