#include <semantic.h>
#include <coreutils.h>
#include <parser.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char* name;
    int name_lenght;
} variable_t;

int semantic_check_node(node_t node, dynamic_array_t* vars);
variable_t* check_var(dynamic_array_t vars, token_t var);
dynamic_array_t* create_symbol_table(dynamic_array_t nds);
uint8_t push_var(dynamic_array_t* vars, token_t var);

int semantic_check(dynamic_array_t* nodes){
    dynamic_array_t* vars = create_symbol_table(*nodes); 
    int err_cnt = 0;
    node_t* nds = nodes->data;
    for(int i = 0; i < nodes->size; i++){
        if(semantic_check_node(nds[i], vars)) err_cnt++;
    }
    delete_array(*vars);
    free(vars);
    return err_cnt;
}

int semantic_check_node(node_t node, dynamic_array_t* vars){
    for(int i = 0; i < node.args_count; i++){
        if(node.args[i].type == VARIABLE){
            if(!check_var(*vars, node.args[i])) return 1;
        }
    }
    return 0;
}

dynamic_array_t* create_symbol_table(dynamic_array_t nds){
    dynamic_array_t* arr = malloc(sizeof(dynamic_array_t));
    *arr = create_array(16, sizeof(variable_t));
    node_t* nodes = nds.data;
    for(int i = 0; i < nds.size; i++){
        if(nodes[i].token.type == KEYWORD_VAR){
            push_var(arr, nodes[i].args[0]);
        }
    }
    return arr;
}

variable_t* check_var(dynamic_array_t vars, token_t var){
	variable_t* v = vars.data;
	for(int i = 0; i < vars.size; i++){
		if(memcmp(v[i].name, var.str_view, var.str_view_lenght) == 0){
			return v + i;
		}
	}
	return NULL;
}

uint8_t push_var(dynamic_array_t* vars, token_t var){
	check_array_capacity(vars, sizeof(variable_t));
	variable_t* variables = vars->data;

	variables[vars->size] = (variable_t){
		.name = var.str_view, 
		.name_lenght = var.str_view_lenght,
	};
	vars->size++;
	return 0;
}