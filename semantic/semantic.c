#include <semantic.h>
#include <coreutils.h>
#include <parser.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct{
    node_t node;
    dynamic_array_t scope; //with variable_t
} scoped_node_t;

typedef struct{
    char* name;
    int name_lenght;
} variable_t;

int semantic_check_node(node_t node, dynamic_array_t* vars);
variable_t* check_var(dynamic_array_t vars, token_t var);
int create_symbol_table(dynamic_array_t* scoped_nodes, node_t node, scoped_node_t* parent);
uint8_t push_var(dynamic_array_t* vars, token_t var);
scoped_node_t push_scoped_node(dynamic_array_t* scoped_nodes, node_t node, dynamic_array_t* scope);

int semantic_check(node_t* root){
    int err_cnt = 0;

    dynamic_array_t vars = create_array(256, sizeof(scoped_node_t));

    create_symbol_table(&vars, *root, NULL); 
    scoped_node_t* scps = vars.data;

    for(int i = 1; i < vars.size; i++){
        if(semantic_check_node(scps[i].node, &scps[i].scope)) err_cnt++;
    }

    delete_array(vars);
    return err_cnt;
}

int semantic_check_node(node_t node, dynamic_array_t* vars){
    int errcunt = 0;
    for(int i = 0; i < node.args_count; i++){
        if(node.args[i].type == VARIABLE){
            if(check_var(*vars, node.args[i]) == NULL){
                fprintf(stdout, "unkonw var\n");
                errcunt += 1;
            }
        }
    }
    return errcunt;
}

int create_symbol_table(dynamic_array_t* scoped_nodes, node_t node, scoped_node_t* parent){
    dynamic_array_t local_scope = {0};
    if(parent == NULL) local_scope = create_array(16, sizeof(variable_t));
    if(parent != NULL){
        local_scope = create_array(parent->scope.size, sizeof(variable_t));
        memcpy(local_scope.data, parent->scope.data, parent->scope.size * sizeof(variable_t));
        local_scope.size = parent->scope.size;
    }

    for(int k = 0; k < node.child_count; k++){
        if(node.child[k].token.type == KEYWORD_VAR){
            push_var(&local_scope, node.child[k].args[0]);
        }   
    }

    scoped_node_t scoped_node = push_scoped_node(scoped_nodes, node, &local_scope);

    for(int k = 0; k < node.child_count; k++){
        create_symbol_table(scoped_nodes, node.child[k], &scoped_node);
    }

    return 0;
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
	vars->size += 1;
	return 0;
}

scoped_node_t push_scoped_node(dynamic_array_t* scoped_nodes, node_t node, dynamic_array_t* scope){
	check_array_capacity(scoped_nodes, sizeof(scoped_node_t));
	scoped_node_t* nodes = scoped_nodes->data;

	nodes[scoped_nodes->size] = (scoped_node_t){
		.node =  node,
		.scope = *scope,
	};
	scoped_nodes->size += 1;

	return (scoped_node_t){
		.node =  node,
		.scope = *scope,
	};
}