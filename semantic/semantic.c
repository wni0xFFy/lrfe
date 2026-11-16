#include <semantic.h>
#include <coreutils.h>
#include <parser.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct{
    char* name;
    int name_lenght;
    int scope;
} variable_t;

int semantic_check_node(node_t node, dynamic_array_t* vars, int scope);
variable_t* check_var(dynamic_array_t vars, token_t var, int scope);
int create_symbol_table(dynamic_array_t* symbols, dynamic_array_t nds, int scope);
uint8_t push_var(dynamic_array_t* vars, token_t var, int scope);

int semantic_check(dynamic_array_t* nodes){
    dynamic_array_t vars = create_array(16, sizeof(variable_t));
    create_symbol_table(&vars, *nodes, 0); 
    int err_cnt = 0;
    node_t* nds = nodes->data;
    for(int i = 0; i < nodes->size; i++){
        if(semantic_check_node(nds[i], &vars, 0)) err_cnt++;
    }
    variable_t* vrs = vars.data;
    for(int i = 0; i < vars.size; i++){
        fprintf(stdout, "%.*s | %d\n", vrs[i].name_lenght, vrs[i].name, vrs[i].scope);
    }
    delete_array(vars);
    return err_cnt;
}

int semantic_check_node(node_t node, dynamic_array_t* vars, int scope){
    int reterro = 0;
    for(int i = 0; i < node.args_count; i++){
        if(node.args[i].type == VARIABLE){
            if(!check_var(*vars, node.args[i], scope)) reterro += 1;
        }
    }
    for(int i = 0; i < node.child_count; i++){
        reterro += semantic_check_node(node.child[i], vars, scope + 1);
    }
    return reterro;
}

int create_symbol_table(dynamic_array_t* symbols, dynamic_array_t nds, int scope){
    node_t* nodes = nds.data;
    for(int i = 0; i < nds.size; i++){
        if(nodes[i].token.type == KEYWORD_VAR){
            push_var(symbols, nodes[i].args[0], scope);
        }

        create_symbol_table(symbols, 
            (dynamic_array_t){
                .data = nodes[i].child, 
                .size = nodes[i].child_count
            }, scope + 1
        );
    }
    return 0;
}

variable_t* check_var(dynamic_array_t vars, token_t var, int scope){
	variable_t* v = vars.data;
	for(int i = 0; i < vars.size; i++){
		if(memcmp(v[i].name, var.str_view, var.str_view_lenght) == 0){
			if(scope >= v[i].scope){
                return v + i;
            }
		}
	}
	return NULL;
}

uint8_t push_var(dynamic_array_t* vars, token_t var, int scope){
	check_array_capacity(vars, sizeof(variable_t));
	variable_t* variables = vars->data;

	variables[vars->size] = (variable_t){
		.name = var.str_view, 
		.name_lenght = var.str_view_lenght,
        .scope = scope,
	};
	vars->size++;
	return 0;
}