#include <stdio.h>
#include <lexer.h>
#include <stdlib.h>
#include <parser.h>
#include <syntax.h>
#include <codegen.h>

void view_tokens(dynamic_array_t tokens);

int main(){
	FILE* f = fopen("test.lr", "rb");
	fseek(f, 0, SEEK_END);
	int s = ftell(f) + 1;
	rewind(f);
	char* buffer = malloc(s + 256);
	fread(buffer, 1, s, f);
	buffer[s - 1] = '\n';

	dynamic_array_t c = tokens(buffer, s);
	dynamic_array_t n = nodes(&c);
	dynamic_array_t ins = {0};
	dynamic_array_t vrs = (dynamic_array_t){.data = malloc(1024), .capacity = 16, .size = 0};
	
	node_t* nds = n.data;
	//if(nds[1].child_count < 1) fprintf(stdout, "SSSSTOOOOOOOP");
	syntax_checker(&n);
	give_adresses(&vrs,n,0);
	generate_init(&ins);

	for(int i = 0; i < n.size; i++){
		gen_node(&ins,&vrs,nds[i]);
	}
	
	generate_exit(&ins);
	dynamic_array_t bytes = create_elf_bytes(ins);

	FILE* output = fopen("lr.elf", "wb");
	uint8_t* bts = bytes.data;
	fwrite(bts, 1, bytes.size, output);
	fclose(output);
	for(int i = 0; i < bytes.size; i++){
		fprintf(stdout,"0x%x ", bts[i]);
	}
	
	free(buffer);

	for(int i = 0; i < n.size; i++){
			free(nds[i].child);
	}

	delete_array(c);
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