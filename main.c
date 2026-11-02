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
	//view_tokens(c);
	dynamic_array_t n = nodes(&c);

	node_t* nds = n.data;
	//if(nds[1].child_count < 1) fprintf(stdout, "SSSSTOOOOOOOP");
	syntax_checker(&n);
}

void view_tokens(dynamic_array_t tokens){
	token_t* tks = tokens.data;
	char* text_impl[] = { 
		"PLUS",
		"MINUS",
		"EQUAL",
		"OPEN-BRACKET",
		"CLOSE-BRACKET",
		"NEWLINE\n",
		"FUNCTION",
		"NUM-LITERAL",
		"VARIABLE",
		"gs",
		"KEYWORD-VAR",
		"KEYWORD-PRINT",
		"KEYWORD-IF",
		"ge",
	};

	for(int i = 0; i < tokens.size; i++){
		fprintf(stdout, "%s ", text_impl[tks[i].type]);
	}
}