#include <stdio.h>
#include <lexer.h>
#include <stdlib.h>
#include <parser.h>
#include <syntax.h>
#include <codegen.h>
#include <string.h>

typedef enum{
	FULL,
	TOKENS,
	TOKENS_PARSING,
	TOKENS_PARSING_SYNTAX,
} workmode_t;

workmode_t G_MODE = FULL;

void view_tokens(dynamic_array_t tokens);
void cut_str(char* str);

int main(int argc, char* argv[]){
	if(argc < 2) {
		fprintf(stdout, "no input files\n");
		return -1;
	}
	if(argc > 2){
		struct table{
			char* str;
			workmode_t mode;
		};

		struct table tbl[] = {
			(struct table){.str = "t", .mode = TOKENS},
			(struct table){.str = "n", .mode = TOKENS_PARSING},
			(struct table){.str = "s", .mode = TOKENS_PARSING_SYNTAX},
		};
		for(unsigned long i = 0; i < sizeof(tbl) / sizeof(struct table); i++){
			if(memcmp(argv[2], tbl[i].str, 1) == 0) G_MODE = tbl[i].mode;
		}
	}
	FILE* f = fopen(argv[1], "rb");
	if(f == NULL) {fprintf(stdout,"%s can't open file\n", argv[1]); return 1;}
	fseek(f, 0, SEEK_END);
	int s = ftell(f) + 1;
	rewind(f);
	char* buffer = malloc(s);
	fread(buffer, 1, s, f);
	buffer[s - 1] = '\n';

	dynamic_array_t* tkens = tokens(buffer, s);
	if(tkens == NULL){fprintf(stdout, "not enough memory\n"); return 1;}
	dynamic_array_t ndes = {};
	dynamic_array_t ins = {0};
	dynamic_array_t vrs = create_array(256, 1);

	switch(G_MODE){
		case TOKENS:
			view_tokens(*tkens);
			return 0;
		case TOKENS_PARSING:
			view_tokens(*tkens);
			ndes = nodes(tkens);
			return 0;
		case TOKENS_PARSING_SYNTAX:
			view_tokens(*tkens);
			ndes = nodes(tkens);
			return syntax_checker(&ndes);
		default:
			break;
	}

	ndes = nodes(tkens);
	if(syntax_checker(&ndes)){
		fprintf(stdout, "Can't compile, errors.");
		return 1;
	}

	give_adresses(&vrs,ndes,0);
	generate_init(&ins);

	node_t* nds = ndes.data;
	for(int i = 0; i < ndes.size; i++){
		gen_node(&ins,&vrs,nds[i]);
	}
	
	generate_exit(&ins);
	dynamic_array_t bytes = create_elf_bytes(ins);

	cut_str(argv[1]);
	FILE* output = fopen(argv[1], "wb");
	uint8_t* bts = bytes.data;
	fwrite(bts, 1, bytes.size, output);
	fclose(output);

	for(int i = 0; i < ndes.size; i++){
			free(nds[i].child);
	}
	free(buffer);
	delete_array(vrs);
	delete_array(ins);
	return 0;
}

void view_tokens(dynamic_array_t tokens){
	token_t* tks = tokens.data;
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

	for(int i = 0; i < tokens.size; i++){
		fprintf(stdout, "%s ", text_impl[tks[i].type]);
	}
}

void cut_str(char* str){
	int i = 0;
	while(str[i] != '.' && str[i] != '\0'){
		i += 1;
	}
	str[i] = '\0';
}