# Introduction
A lexer(other name is tokenizer) is the first part of any compiler. It breaks down source code into meaningful chunks(lexems/tokens). Anyway, you can write the basic compiler without it, but i highly recommend against this method.

# How it works? 
(the main question of the project..)
My realization works on sliding-window parsing. 
```C
    for(unsigned long i = 0; i < file_size; i++){
		if(input[i] == '\n' || input[i] == ' '){
			token_type tk = identify_token_type(last_token_start, (input + i) - last_token_start);
			
			...
			
			last_token_start = input + i;
		}
	}
```
I've cut some code, which isn't important now. Important part is working with indices. Let's break down this part:
```C
identify_token_type(last_token_start, (input + i) - last_token_start);
```
Definetly, it is function call. However, you need to understand the second argument in this call.
``` C
    (input + i) - last_token_start
```

In short, it's computing pointer to current symbol and subtract poiner to last_token_start? What is it? It's saved poiner to the first symbol of token(it's located after new line or after space)
Finally, this piece of code computes lenght of current token. Nothing more.

And now, you would like to ask a question: "Why do we add i to last_token_start on space or newline?" 
Here's the answer. 
```C
    while(i < file_size && input[i] == ' ') i++;
    if(input[i] == '\n'){
        if(push_token(arr, NEWLINE, NULL, 0)){
            delete_array(*arr);
            free(arr);
            return NULL;
        }
    }
    while(i < file_size && (input[i] == '\n' || input[i] == ' ')) i++;
    
    last_token_start = input + i;   
```
This part of code skips every space or extra lines and writes into last_token_start.. CURRENT TOKEN START! It might sound a little bit tricky.
However, after writing loop scans for next \n or space, which's mean that current token ends.