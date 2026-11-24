# Parser(AST builder) module

# Input

contains main function, which needs dynamic array of tokens. 

# Output

root node of source code AST

# Structures 

```C
typedef struct node_t{
	token_t token;
	token_t* args;
	int args_count;

	int child_count;
	struct node_t* child;

	uint8_t flags;
} node_t;
```

node_t is node structure which's used by any functions(public and private) in this module.

# Public Functions

```C
    node_t nodes(dynamic_array_t* tokens); 
```

The main function returns tree root.

# Parser Schema

![[shema.svg]]