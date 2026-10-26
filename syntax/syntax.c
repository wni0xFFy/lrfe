#include <syntax.h>
#include <parser.h>
#include <lexer.h>
#include <stdio.h>
#include <coreutils.h>
#include <stdint.h>

typedef struct{
	char* msg;
	int code;
} error_t;

error_t is_var(node_t node);
error_t is_print(node_t node);

int syntax_checker(dynamic_array_t* _nodes){
	node_t* nodes = _nodes->data;
	error_t (*checkers[]) (node_t node) = {
		is_var,	
		is_print,
	};
	
   for(node_t* node = nodes; node < nodes + _nodes->size ; node++){
   		for(unsigned long i = 0; i < sizeof(checkers) / sizeof(checkers[0]); i++){
   			error_t err = checkers[i](*node);
   			if(err.code == 2) fprintf(stdout, "%s\n", err.msg);
   		}
	}
	return 1;
}

error_t is_var(node_t node){
	if(node.token.type != KEYWORD_VAR) 
		return (error_t){.code = 0, .msg = 0};

	if(node.args_count == 2){
		if(node.args[0].type == VARIABLE && node.args[1].type == NUM_LITERAL) 
			return (error_t){.code = 1, .msg = 0};
	}

	return (error_t){.code = 2, .msg = "invalid 'var' using"};
}

error_t is_print(node_t node){
	if(node.token.type != KEYWORD_PRINT) 
		return (error_t){.code = 0, .msg = 0};

	if(node.args_count == 1){
		if(node.args[0].type == VARIABLE) 
			return (error_t){.code = 1, .msg = 0};
	}

	return (error_t){.code = 2, .msg = "invalid 'print' using"};
 }