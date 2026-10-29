#include <stdio.h>
#include <lexer.h>
#include <stdlib.h>
#include <parser.h>
#include <syntax.h>

void view_tokens(dynamic_array_t tokens);

int main(){
	FILE* f = fopen("test.lr", "r");
	fseek(f, 0, SEEK_END);
	int s = ftell(f) + 1;
	rewind(f);
	char* buffer = malloc(s);
	fread(buffer, 1, s, f);
	buffer[s - 1] = '\n';

	dynamic_array_t c = tokens(buffer, s);
	view_tokens(c);
	dynamic_array_t n = nodes(&c);
	node_t* nds = n.data;
	for(int i = 0; i < c.size; i++){
		node_t* child = nds[i].child;
		if(child != NULL){
			if(child->args_count > 0){
				for(int i = 0; i < child->args_count; i++){
					fprintf(stdout, "ARGUMENT\n");
				}
			}
		}
	}

	syntax_checker(&n);
}

void view_tokens(dynamic_array_t tokens){
	token_t* tks = tokens.data;
	for(int i = 0; i < tokens.size; i++){
		switch(tks[i].type){
			case NEWLINE:
				fprintf(stdout, "NEW LINE\n");
				break;
			case CLOSE_CODE_BLOCK:
				fprintf(stdout, "CLOSE ");
				break;
			case OPEN_CODE_BLOCK:
				fprintf(stdout, "OPEN ");
				break;
			case FUNCTION:
				fprintf(stdout, "FUNCTION" );
				break;
			case NUM_LITERAL:
				fprintf(stdout, "NUM ");
				break;
			case VARIABLE:
				fprintf(stdout, "VARIABLE ");
				break;
			case KEYWORD_VAR:
				fprintf(stdout, "KEYWORD-VAR ");
				break;
			case KEYWORD_PRINT:
				fprintf(stdout, "KEYWORD-PRINT ");
				break;
		}
	}
}