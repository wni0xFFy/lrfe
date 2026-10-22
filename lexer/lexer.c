#include <lexer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// function ALWAYS starts with @ 

token_type id(char* str, int len);
int len(char* str);

token_array_t tokens(char* input, long file_size){
	token_array_t arr = {0};
	arr.size = 0;
	arr.tokens = malloc(128 * sizeof(token_t));
	char* last_token_start = input;

	for(int i = 0; i < file_size; i++){
		if(input[i] == '\n' || input[i] == ' '){
			token_type tk = id(last_token_start, (input + i) - last_token_start);
			arr.tokens[arr.size] = (token_t){
				.type = tk, 
				.str_view = last_token_start, 
				.str_view_lenght = (input + i) - last_token_start
			};
			
			arr.size++;

			while(input[i] == '\n' || input[i] == ' ') i++;
			last_token_start = input + i;
		}
	}

	return arr;
}


token_type id(char* str, int len){
	if(str[0] == '@') return FUNCTION;

	struct table{
		char* str;
		token_type tp;
	};

	struct table keywords_table[] = {
		(struct table){.str = "var", .tp = KEYWORD_VAR},
	};

	for(unsigned long i = 0; i < sizeof(keywords_table) / sizeof(struct table); i++){
		if(memcmp(str, keywords_table[i].str, strlen(keywords_table[i].str)) == 0) return keywords_table[i].tp;
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

