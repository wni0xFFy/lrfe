#include <stdio.h>
#include <lexer.h>
#include <stdlib.h>
int main(){
	token_type* t = malloc(100);
	FILE* f = fopen("test.lr", "r");
	tokens(f, 21, t);
}