#include <lexer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// function ALWAYS starts with @ 

token_type id(char* str, int len);
int len(char* str);

const char* msgs[] = {
	"function ",
	"identifier ",
	"variable ",
	"keyword ",
};
int tokens(FILE* f, long file_size, token_type* output){
	char* buffer = malloc(file_size);
	int tokens_count = 0;
	while(file_size > 0){
		int lenght = fread(buffer, 1, file_size, f);
		char word[128]; word[127] = '\0';
		int wptr = 0;

		for(int i = 0; i < lenght; i++){
			if(buffer[i] == '\n' || buffer[i] == ' '){
				token_type tk = id(word, len(word));
				fprintf(stdout, "%s%s\n", msgs[tk], word);
		
				wptr = 0;
				tokens_count++;
				memset(word, 0, 127);
				while(buffer[i] == '\n' || buffer[i] == ' ') i++;
			}
			word[wptr] = buffer[i];
			wptr++;
		}

		fprintf(stdout, "%d", tokens_count);

		file_size -= lenght;
	}
	free(buffer);
	return tokens_count;
}


token_type id(char* str, int len){
	if(str[0] == '@') return FUNCTION;
	char* keywords[] = {
		"var",
	};

	for(int i = 0; i < sizeof(keywords); i++){
		if(memcmp(str, keywords[i], 3) == 0) return KEYWORD;
	}

	for(int i = 0; i < len; i++){
		if(!isdigit(str[i])) return VARIABLE;
	}

	return IDENTIFIER;
}

int len(char* str){
	int i = 0;
	while(str[i] != 0) i++;
	return i;
}

