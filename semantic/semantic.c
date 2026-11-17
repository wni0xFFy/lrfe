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

int semantic_check_node(node_t node, dynamic_array_t* vars, int scope);
variable_t* check_var(dynamic_array_t vars, token_t var, int scope);
int create_symbol_table(dynamic_array_t* scoped_nodes, node_t node, scoped_node_t* parent);
uint8_t push_var(dynamic_array_t* vars, token_t var);
scoped_node_t push_scoped_node(dynamic_array_t* scoped_nodes, node_t node, dynamic_array_t* scope);

int semantic_check(dynamic_array_t* nodes){
    int err_cnt = 0;
    node_t* nds = nodes->data;
    dynamic_array_t vars = create_array(256, sizeof(scoped_node_t));
    create_symbol_table(&vars, nds[0], NULL); 

    scoped_node_t* nd12 = vars.data;
    variable_t* var = nd12[vars.size - 1].scope.data;
    for(int i = 0; i < vars.size; i++){
        scoped_node_t scoped_node = nd12[i];
        for(int k = 0; k < scoped_node.scope.size; k++){
            fprintf(stdout, "%.*s ", var[k].name_lenght, var[k].name); 
        }   
        fprintf(stdout, "\n");
    }
    for(int i = 0; i < nodes->size; i++){
        if(semantic_check_node(nds[0], &vars, 0)) err_cnt++;
    }

    variable_t* vrs = vars.data;

    delete_array(vars);
    return err_cnt;
}

int semantic_check_node(node_t node, dynamic_array_t* vars, int scope){
    if(node.child_count == 0){fprintf(stdout, "The end\n"); return 2;}
    int k = semantic_check_node(node.child[0], vars, scope + 1);
    if(k == 2){
        fprintf(stdout, "The layer before end.\n");
        return 1;
    }
    if(k == 1) return 1;
    
    return 0;
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

    scoped_node_t* nd12 = scoped_nodes->data;
    variable_t* var = nd12[scoped_nodes->size - 1].scope.data;

    for(int k = 0; k < node.child_count; k++){
        create_symbol_table(scoped_nodes, node.child[k], &scoped_node);
    }

    return 0;
}

variable_t* check_var(dynamic_array_t vars, token_t var, int scope){
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