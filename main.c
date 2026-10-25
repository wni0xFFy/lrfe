#include <stdio.h>
#include <lexer.h>
#include <stdlib.h>
#include <parser.h>
#include <syntax.h>

int main(){
	FILE* f = fopen("test.lr", "r");
	fseek(f, 0, SEEK_END);
	int s = ftell(f);
	rewind(f);
	char* buffer = malloc(s);
	fread(buffer, 1, s, f);

	dynamic_array_t c = tokens(buffer, s);
	token_t* t = c.data;
	//for(int i = 0; i < c.size; i++) printf("%d\n", t[i].type);
	dynamic_array_t n = nodes(&c);
	syntax_checker(&n);
	node_t* nodes = n.data;
	// for(int i = 0; i < n.size; i++){
	// 	node_t* node = nodes + i;
	// 	if(node->token.type == FUNCTION) fprintf(stdout, "\tfunction : \n");
	// 	else fprintf(stdout, "\tkeyword : \n");

	// 	for(int i = 0; i < node->args_count; i++){
	// 		switch(node->args[i].type){
	// 			case FUNCTION:
	// 				fprintf(stdout, "func\n");
	// 				break;
	// 			case KEYWORD_VAR:
	// 				fprintf(stdout, "create var\n");
	// 				break;
	// 			case NUM_LITERAL:
	// 				fprintf(stdout, "identifier\n");
	// 				break;
	// 			case VARIABLE:
	// 				fprintf(stdout, "variable\n");
	// 				break;
	// 		}
	// 	}
	// }
}