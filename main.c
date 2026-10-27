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
	node_t* nds = n.data;
	for(int i = 0; i < c.size; i++){
		node_t* child = nds[i].child;
		if(child != NULL){
		if(child->args_count > 0) fprintf(stdout, "CHILDD\n");
		}
	}

	syntax_checker(&n);
}