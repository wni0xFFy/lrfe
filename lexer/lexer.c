#include <lexer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
				/* !!!!!!check 13 and 55 lines and create model for case for thinkpad(i wanna fucking write fucking w2bt but i can't without server)*/
int isSpecialSymbol(char symbol);
token_type identificate_token(char* symbol, unsigned long lenght);

int tokens(FILE* f, long file_size, token_type* output){
	char* buffer = malloc(1024);
	int tokens_count  = 0;

	/*
		 rewrite it into the model "FUNCTION INPUT INPUT OUTPUT"(see test.lr)
		 and 1 line is ONE and ONLY ONE operation like '+ 2 2 x' is x = 2 + 2 bc + is function and 
		 x is the last argument and if function HAS output last arg is output
	 */
	
	while(file_size > 0){
		int readed = fread(buffer, 1, 1024, f); //segment error
		for(int i = 0; i < readed; i++){
			if(buffer[i] == '\n'){}
		}

		file_size -= readed;
	}
	memset(buffer, 0, 1024);
	free(buffer);
	return tokens_count;
}


int isSpecialSymbol(char symbol){
	char special_symbols[] = {
		' ',
		';',
		'(',
		')',
		'{',
		'}',
		'\n',
	};

	for(unsigned long i = 0; i < sizeof(special_symbols); i++) 
		if(symbol == special_symbols[i]) return 1;
	
	return 0;
}

token_type identificate_token(char* symbol, unsigned long lenght){
	typedef struct{
		token_type type;
		char* str;
	} type_string;
	//rewrite it for fucntion + argument only
	type_string map[] = {
		(type_string){.type = IF, .str = "if"},
		(type_string){.type = NEXT_OP, .str = ";"},
		(type_string){.type = ELSE, .str = "else"},
		(type_string){.type = LET, .str = "let"},
		(type_string){.type = CLOSE_FIGURE_BRACKET, .str = "}"},
		(type_string){.type = OPEN_FIGURE_BRACKET, .str = "{"},
		(type_string){.type = CLOSE_BRACKET, .str = ")"},
		(type_string){.type = OPEN_BRACKET, .str = "("},
	};
	for(unsigned long i = 0; i <  sizeof(map) / sizeof(type_string); i++){
		if(lenght < strlen(map[i].str)) continue;
		if(memcmp(map[i].str, symbol, lenght) == 0) return map[i].type; 
	}

	return IDENTIFIER;
}