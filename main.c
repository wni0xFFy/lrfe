#include <stdio.h>
#include <lexer.h>
#include <stdlib.h>
int main(){
	token_type* t = malloc(100);
	FILE* f = fopen("test.lr", "r");
	fseek(f, 0, SEEK_END);
	int s = ftell(f);
	rewind(f);
	tokens(f, s, t);
}