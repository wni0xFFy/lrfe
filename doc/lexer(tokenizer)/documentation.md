# Lexer Module

# Structures 
```C
typedef enum{
	OPERATOR_PLUS,
    VARIABLE
	...
} token_type;
```
token_type is enumeration of token types. Token type is needed for distinguish tokens. This enumeration didn't work singly. Often, it's used with token_t structure

```C
typedef struct{
	token_type type; 
	char* str_view;
	int str_view_lenght;
} token_t;
```
token_t is the main structure for tokens, it contains token_type, string which contains token as it is in source file, string lenght.

# Public Functions

```C
dynamic_array_t* tokens(char* input, unsigned long file_size);
```
tokens() is the main function, which transforms raw source file into tokens.
input should be terminated with \n symbol, otherwise function will miss last token.

Return value is dynamic_array_t structure instance(check coreutils/documentation.md), which contains tokens array and its count.
