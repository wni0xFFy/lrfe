#include <lexer.h>

char* token_to_string(int token){
    char* text_impl[] = { 
		"PLUS",
		"MINUS",
		"EQUAL",
		"FEWER",
	    "OPERATOR_GREATER",
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
    return text_impl[token];
}