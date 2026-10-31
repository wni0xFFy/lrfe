#include <stdio.h>
#include <lexer.h>
#include <stdlib.h>
#include <parser.h>
#include <syntax.h>

void view_tokens(dynamic_array_t tokens);

int main(){
	FILE* f = fopen("test.lr", "rb");
	fseek(f, 0, SEEK_END);
	int s = ftell(f) + 1;
	rewind(f);
	char* buffer = malloc(s + 256);
	fread(buffer, 1, s, f);
	buffer[s - 1] = '\n';

	dynamic_array_t c = tokens(buffer, s);
	view_tokens(c);
	dynamic_array_t n = nodes(&c);
	node_t* nds = n.data;
	for(int i = 0; i < n.size; i++){
		if(nds[i].childs > 0){
			fprintf(stdout, "CHILDS %d", nds[i].childs);
			// for(int j = 0; j < nds[i].childs; j++){
			// 	node_t child = nds[i].child[j];
			// 	fprintf(stdout, "child : %.*s", child.token.str_view_lenght, child.token.str_view);
			// }
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
			default:
				fprintf(stdout, "UNEXPECTED TOKEN ");
				break;
		}
	}
}