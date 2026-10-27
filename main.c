#include <stdio.h>
#include <lexer.h>
#include <stdlib.h>
#include <parser.h>
#include <syntax.h>

int main(){
	FILE* f = fopen("test.lr", "r");
	fseek(f, 0, SEEK_END);
	int s = ftell(f) + 1;
	rewind(f);
	char* buffer = malloc(s);
	fread(buffer, 1, s, f);
	buffer[s - 1] = '\n';

	dynamic_array_t c = tokens(buffer, s);
	dynamic_array_t n = nodes(&c);
	token_t* nds = c.data;
	for(int i = 0; i < c.size; i++){
		if(nds[i].type == OPEN_CODE_BLOCK) fprintf(stdout, "OPEN\n");
 		if(nds[i].type == CLOSE_CODE_BLOCK) fprintf(stdout, "CLOSE\n");
	}
	syntax_checker(&n);
}