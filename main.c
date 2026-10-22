#include <stdio.h>
#include <lexer.h>
#include <stdlib.h>
int main(){
	FILE* f = fopen("test.lr", "r");
	fseek(f, 0, SEEK_END);
	int s = ftell(f);
	rewind(f);
	char* buffer = malloc(s);
	fread(buffer, 1, s, f);

	token_array_t c = tokens(buffer, s);

	for(int i = 0; i < c.size; i++){
		switch(c.tokens[i].type){
			case FUNCTION:
				fprintf(stdout, "func\n");
				break;
			case KEYWORD_VAR:
				fprintf(stdout, "create variable\n");
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