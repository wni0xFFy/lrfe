#include <lexer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <coreutils.h>
#include <ctype.h>
#include <stdint.h>

const int MINIMAL_TOKENS_COUNT = 16;

token_type id(char* str, unsigned long len);
inline uint8_t is_numeric_literal(char* str, unsigned long len);
inline void push_token(dynamic_array_t* arr, token_type type, char* token_str, int token_lenght);

dynamic_array_t* tokens(char* input, unsigned long file_size){
	dynamic_array_t* arr = malloc(sizeof(dynamic_array_t));
	*arr = create_array(MINIMAL_TOKENS_COUNT, sizeof(token_t));

	char* last_token_start = input;

	for(unsigned long i = 0; i < file_size; i++){
		if(input[i] == '\n' || input[i] == ' '){
			token_type tk = id(last_token_start, (input + i) - last_token_start);
			
			if(check_array_capacity(arr, sizeof(token_t))) return NULL;

			push_token(arr, tk, last_token_start, (input + i) - last_token_start);

			while(i < file_size && input[i] == ' ') i++;
			if(input[i] == '\n'){
				push_token(arr, NEWLINE, NULL, 0);
			}
			while(i < file_size && (input[i] == '\n' || input[i] == ' ')) i++;
			
			last_token_start = input + i;
		}
	}

	return arr;
}


token_type id(char* str, unsigned long len){
	if(str[0] == '@') return FUNCTION;
	
	struct table{
		char* str;
		token_type tp;
	};

	struct table tokens_table[] = {
		(struct table){.str = "var", .tp = KEYWORD_VAR},
		(struct table){.str = "print", .tp = KEYWORD_PRINT},
		(struct table){.str = "if", .tp = KEYWORD_IF}, 
		(struct table){.str = "}", .tp = CLOSE_CODE_BLOCK},
		(struct table){.str = "{", .tp = OPEN_CODE_BLOCK},
		(struct table){.str = "+", .tp = OPERATOR_PLUS},
		(struct table){.str = "-", .tp = OPERATOR_MINUS},
		(struct table){.str = "==", .tp = OPERATOR_EQUAL},
	};

	for(unsigned long i = 0; i < sizeof(tokens_table) / sizeof(struct table); i++){
		if(strlen(tokens_table[i].str) != len) continue;
		if(memcmp(str, tokens_table[i].str, strlen(tokens_table[i].str)) == 0){
			return tokens_table[i].tp;
		}
	}
	
	return is_numeric_literal(str, len) ? NUM_LITERAL : VARIABLE;
}

inline uint8_t is_numeric_literal(char* str, unsigned long len){
	for(unsigned long idx = 0; idx < len; idx++){
		if(!isdigit(str[idx])) return 0;
	}
	return 1;
}


inline void push_token(dynamic_array_t* arr, token_type type, char* token_str, int token_lenght){
	((token_t*)arr->data)[arr->size] = (token_t){
		.type = type, 
		.str_view = token_str, 
		.str_view_lenght = token_lenght,
	};
	arr->size += 1;
}