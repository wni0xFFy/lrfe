#pragma once
#include <coreutils.h>

//maybe add limits(like OPERATOR_GROUP_START)

typedef enum{
	OPERATOR_PLUS,
	OPERATOR_MINUS,
	OPERATOR_EQUAL,
	OPERATOR_FEWER,
	OPERATOR_GREATER,
	//add fewer and greater

	OPEN_CODE_BLOCK,
	CLOSE_CODE_BLOCK,
	
	NEWLINE,
	FUNCTION,
	NUM_LITERAL,
	VARIABLE,

	KEYWORDS_GROUP_START,
	KEYWORD_VAR,
	KEYWORD_PRINT,
	KEYWORD_IF,
	KEYWORDS_GROUP_END,
} token_type;

typedef struct{
	token_type type; 
	char* str_view;
	int str_view_lenght;
} token_t;

dynamic_array_t* tokens(char* input, unsigned long file_size);