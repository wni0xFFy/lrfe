#include <syntax.h>
#include <parser.h>
#include <lexer.h>
#include <stdio.h>
#include <coreutils.h>
#include <stdint.h>

uint8_t is_var(node_t node);
uint8_t is_print(node_t node);

int syntax_checker(dynamic_array_t* _nodes){
	node_t* nodes = _nodes->data;

   for(node_t* node = nodes; node <= nodes + _nodes->size ; node++){
		if(is_var(*node) == 2) fprintf(stdout, "Wrong syntax in variable creation!");
		if(is_print(*node) == 2) fprintf(stdout, "Wrong syntax in print!");
	}
	return 1;
}

uint8_t is_var(node_t node){
	if(node.token.type != KEYWORD_VAR) return 0;
	if(node.args_count == 2){
		if(node.args[0].type == VARIABLE && node.args[1].type == NUM_LITERAL) return 1;
	}
	return 2;
}

uint8_t is_print(node_t node){
	if(node.token.type != KEYWORD_PRINT) return 0;
	if(node.args_count == 1){
		if(node.args[0].type == VARIABLE) return 1;
	}
	return 2;
 }