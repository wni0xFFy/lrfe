#include <lexer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <coreutils.h>
#include <ctype.h>
#include <stdint.h>

const int MINIMAL_TOKENS_COUNT = 16;

token_type identify_token_type(token_t* tok, char* str, unsigned long len);
inline uint8_t is_numeric_literal(char* str, unsigned long len);
inline uint8_t push_token(dynamic_array_t* arr, token_type type, char* token_str, int token_lenght, uint8_t value);
void ffsymbol(char** str, unsigned int* len);

dynamic_array_t* tokens(char* input, unsigned long file_size){
	dynamic_array_t* arr = malloc(sizeof(dynamic_array_t));
	*arr = create_array(MINIMAL_TOKENS_COUNT, sizeof(token_t));

	ffsymbol(&input, (unsigned int*)&file_size);

	char* last_token_start = input;

	for(unsigned long i = 0; i < file_size; i++){
		if(input[i] == '\n' || input[i] == ' '){
			token_t tok = {0};
			token_type tk = identify_token_type(&tok, last_token_start, (input + i) - last_token_start);
			
			if(push_token(arr, tk, last_token_start, (input + i) - last_token_start, tok.value)){
					delete_array(*arr);
					free(arr);
					return NULL;
			}

			while(i < file_size && input[i] == ' ') i++;
			if(input[i] == '\n'){
				if(push_token(arr, NEWLINE, NULL, 0, tok.value)){
					delete_array(*arr);
					free(arr);
					return NULL;
				}
			}
			while(i < file_size && (input[i] == '\n' || input[i] == ' ')) i++;
			
			last_token_start = input + i;
		}
	}

	return arr;
}


token_type identify_token_type(token_t* tok, char* str, unsigned long len){
	if(str[0] == '@') return FUNCTION;
	
	struct table{
		char* str;
		token_type tp;
	};

	struct table tokens_table[] = {
		(struct table){.str = "let", .tp = KEYWORD_VAR},
		(struct table){.str = "print", .tp = KEYWORD_PRINT},
		(struct table){.str = "if", .tp = KEYWORD_IF}, 
		(struct table){.str = "}", .tp = CLOSE_CODE_BLOCK},
		(struct table){.str = "{", .tp = OPEN_CODE_BLOCK},
		(struct table){.str = "+", .tp = OPERATOR_PLUS},
		(struct table){.str = "-", .tp = OPERATOR_MINUS},
		(struct table){.str = "<", .tp = OPERATOR_FEWER},
		(struct table){.str = ">", .tp = OPERATOR_GREATER},
		(struct table){.str = "==", .tp = OPERATOR_EQUAL},
	};

	for(unsigned long i = 0; i < sizeof(tokens_table) / sizeof(struct table); i++){
		if(strlen(tokens_table[i].str) != len) continue;
		if(memcmp(str, tokens_table[i].str, strlen(tokens_table[i].str)) == 0){
			fprintf(stdout, "%s\n", tokens_table[i].str);
			return tokens_table[i].tp;
		}
	}
	tok->value = is_numeric_literal(str, len);
	return tok->value ? NUM_LITERAL : VARIABLE;
}
inline uint8_t is_numeric_literal(char* str, unsigned long len){
	uint8_t num = 0;
	int digit = 0;
	for(unsigned long idx = 0; idx < len; idx++){
		digit = isdigit(str[idx]);
		if(digit == 0) return 0;
		else num *=	10; num += str[idx] - '0';
	}
	return num;
}


inline uint8_t push_token(dynamic_array_t* arr, token_type type, char* token_str, int token_lenght, uint8_t value){
	if(check_array_capacity(arr, sizeof(token_t))) return 1; 
	((token_t*)arr->data)[arr->size] = (token_t){
		.value = value,
		.type = type, 
		.str_view = token_str, 
		.str_view_lenght = token_lenght,
	};
	arr->size += 1;
	return 0;
}

void ffsymbol(char** str, unsigned int* len){
	char* end = *str + *len;
	while(*str < end && (**str == '\n' || **str == ' ')){
		*str += 1; 
		*len -= 1;
	}
}