#include <stdio.h>
#include <lexer.h>
#include <stdlib.h>
#include <parser.h>
int main(){
	FILE* f = fopen("test.lr", "r");
	fseek(f, 0, SEEK_END);
	int s = ftell(f);
	rewind(f);
	char* buffer = malloc(s);
	fread(buffer, 1, s, f);

	dynamic_array_t c = tokens(buffer, s);
	token_t* tokens = c.data;
	dynamic_array_t n = nodes(&c);
	node_t* nodes = n.data;
	//for(node_t* node = n.data; node <= n.data + n.size; node++){
	for(int i = 0; i < n.size; i++){
		node_t* node = nodes + i;
		if(node->token.type == FUNCTION) fprintf(stdout, "\tfunction : \n");
		else fprintf(stdout, "\tkeyword : \n");

		for(int i = 0; i < node->args_count; i++){
			switch(node->args[i].type){
				case FUNCTION:
					fprintf(stdout, "func\n");
					break;
				case KEYWORD_VAR:
					fprintf(stdout, "create var\n");
					break;
				case IDENTIFIER:
					fprintf(stdout, "identifier\n");
					break;
				case VARIABLE:
					fprintf(stdout, "variable\n");
					break;
			}
		}
	}
}