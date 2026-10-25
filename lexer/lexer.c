#include <lexer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <coreutils.h>
#include <ctype.h>

token_type id(char* str, int len);
int len(char* str);
uint8_t is_numeric_literal(char* str, int len);

dynamic_array_t tokens(char* input, long file_size){
	dynamic_array_t arr = {0};

	arr.capacity = 16;
	arr.size = 0;
	arr.data = (token_t*)malloc(arr.capacity * sizeof(token_t));


	char* last_token_start = input;

	for(int i = 0; i < file_size; i++){
		if(input[i] == '\n' || input[i] == ' '){
			token_type tk = id(last_token_start, (input + i) - last_token_start);
			
			if(arr.size >= arr.capacity){
				token_t* tmp = realloc(arr.data, sizeof(token_t) * arr.size + 16);
				if(tmp != NULL) arr.data = tmp;
				arr.capacity = arr.size + 16;
			}

			((token_t*)arr.data)[arr.size] = (token_t){
				.type = tk, 
				.str_view = last_token_start, 
				.str_view_lenght = (input + i) - last_token_start
			};
			
			arr.size++;
			
			while(input[i] == '\n' || input[i] == ' ') i++;

			if(input[i - 1] == '\n'){
				((token_t*)arr.data)[arr.size] = (token_t){
					.type = NEWLINE, 
					.str_view = NULL, 
					.str_view_lenght = 0,
				};
				
				arr.size++;
			}
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
	return is_numeric_literal(str, len) ? NUM_LITERAL : VARIABLE;
}

int len(char* str){
	int i = 0;
	while(str[i] != 0) i++;
	return i;
}

uint8_t is_numeric_literal(char* str, int len){
	for(int i = 0; i < len; i++){
		if(!isdigit(str[i])) return 0;
	}
	return 1;
}
