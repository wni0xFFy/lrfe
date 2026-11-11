#include <stdio.h>
#include <lexer.h>
#include <stdlib.h>
#include <parser.h>
#include <syntax.h>
#include <codegen.h>

void view_tokens(dynamic_array_t tokens);
void cut_str(char* str);

int main(int argc, char* argv[]){
	if(argc < 2) {
		fprintf(stdout, "no input files\n");
		return -1;
	}
	
	FILE* f = fopen(argv[1], "rb");
	fseek(f, 0, SEEK_END);
	int s = ftell(f) + 1;
	rewind(f);
	char* buffer = malloc(s);
	fread(buffer, 1, s, f);
	buffer[s - 1] = '\n';

	dynamic_array_t* c = tokens(buffer, s);
	if(c == NULL) {fprintf(stdout,"not enough memory\n"); return 1;}
	//view_tokens(*c);
	dynamic_array_t n = nodes(c);
	dynamic_array_t ins = {0};
	dynamic_array_t vrs = (dynamic_array_t){.data = malloc(1024), .capacity = 16, .size = 0};
	
	node_t* nds = n.data;
	if(syntax_checker(&n)){
		fprintf(stdout, "Can't compile, errors.");
		return 1;
	}
	give_adresses(&vrs,n,0);
	generate_init(&ins);

	for(int i = 0; i < n.size; i++){
		gen_node(&ins,&vrs,nds[i]);
	}
	
	generate_exit(&ins);
	dynamic_array_t bytes = create_elf_bytes(ins);

	cut_str(argv[1]);
	FILE* output = fopen(argv[1], "wb");
	uint8_t* bts = bytes.data;
	fwrite(bts, 1, bytes.size, output);
	fclose(output);

	for(int i = 0; i < n.size; i++){
			free(nds[i].child);
	}
	free(buffer);
	delete_array(*c);
	delete_array(n);
	delete_array(vrs);
	delete_array(ins);
}

void view_tokens(dynamic_array_t tokens){
	token_t* tks = tokens.data;
	char* text_impl[] = { 
		"PLUS",
		"MINUS",
		"EQUAL",
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
	while(str[i] != '.'){
		i += 1;
	}
	str[i] = '\0';
}