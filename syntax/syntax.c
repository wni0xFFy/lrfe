#include <syntax.h>
#include <parser.h>
#include <lexer.h>
#include <stdio.h>
#include <coreutils.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char* name;
	int name_size;
} variable_t;

typedef struct{
	char* msg;
	int code;
} error_t;

typedef struct checking{
		token_type tok;
		uint8_t (*checker) (token_t token, dynamic_array_t* vars);
} token_rule_t;

typedef struct node_rule_t{
	token_rule_t* rules;
	int args_count;
} node_rule_t;

node_rule_t select_rule(token_t token);
uint8_t check_var(token_t var, dynamic_array_t* vars);
uint8_t check_push_var(token_t var, dynamic_array_t* vars);
uint8_t check_placeholder();
void check_syntax_node(int* current_line, int* errors_count, node_t node, dynamic_array_t* vars);
uint8_t push_var(dynamic_array_t* vars, token_t var);
error_t check_node(node_rule_t* rules, int rules_lenght,node_t node, dynamic_array_t* vars);

int syntax_checker(dynamic_array_t* _nodes){
	int errors_count = 0;
	dynamic_array_t vars = {.data = malloc(sizeof(variable_t) * 32), .size = 0};

	node_t* nodes = _nodes->data;

    for(int i = 1; i <= _nodes->size; i++){
   		check_syntax_node(&i, &errors_count, nodes[i - 1], &vars);
	}
	delete_array(vars);
	return errors_count;
}

void check_syntax_node(int* current_line, int* errors_count, node_t node, dynamic_array_t* vars){
	if(node.flags != 0){
		fprintf(stdout, "line %d : unclosed block\n", *current_line);
		*errors_count += 1;
	}

	node_rule_t rule = select_rule(node.token);
	error_t err = check_node(&rule, 1, node, vars);

	if(err.code == 2){
			fprintf(stdout, "line %d : %s\n", *current_line ,err.msg);
			*errors_count += 1;
	}
	for(int k = 0; k < node.child_count; k++){
		*current_line += 1;
		check_syntax_node(current_line, errors_count, node.child[k], vars);
	}
}

node_rule_t select_rule(token_t token){
	token_rule_t* var_token_rules = malloc(2 * sizeof(token_rule_t));
	var_token_rules[0] = (token_rule_t){.checker = check_push_var, .tok = VARIABLE};
	var_token_rules[1] = (token_rule_t){.checker = check_placeholder, .tok = NUM_LITERAL};


	token_rule_t* print_token_rules = malloc(1 * sizeof(token_rule_t));
	print_token_rules[0] = (token_rule_t){.checker = check_var, .tok = VARIABLE};

	token_rule_t* if_token_rules = malloc(3 * sizeof(token_rule_t));
	if_token_rules[0] = (token_rule_t){.checker = check_var, .tok = VARIABLE};
	if_token_rules[1] = (token_rule_t){.checker = check_placeholder, .tok = OPERATOR_EQUAL};
	if_token_rules[2] = (token_rule_t){.checker = check_var, .tok = VARIABLE};

	node_rule_t rules[] = {
		(node_rule_t){.args_count = 2, .rules = var_token_rules},
		(node_rule_t){.args_count = 1, .rules = print_token_rules},
		(node_rule_t){.args_count = 3, .rules = if_token_rules},
	};
	
	return rules[token.type - KEYWORDS_GROUP_START - 1];
}

error_t check_node(node_rule_t* rules, int rules_lenght,node_t node, dynamic_array_t* vars){
	char* expected_token = 0;

	for(int rule = 0; rule < rules_lenght; rule++){
		node_rule_t curr_rule = rules[rule];
		int is_correct = 0;
		for(int i = 0; i < curr_rule.args_count; i++){
			if(node.args[i].type != curr_rule.rules[i].tok){
				expected_token = token_to_string(curr_rule.rules[i].tok);
				is_correct = 0;
				break;
			}
			if(curr_rule.rules[i].checker(node.args[i], vars)){return (error_t){.code = 2, .msg = "invalid using"};}
			is_correct = 1;
		}

		if(rule + 1 == rules_lenght && is_correct == 0){
			char* error_msg = malloc(1024);
			snprintf(error_msg, 1024, "invalid %.*s using. Expected %s", node.token.str_view_lenght, node.token.str_view, expected_token);
			return (error_t){.code = 2, .msg = error_msg};
		}
	}

	return (error_t){.code = 1, .msg = 0};
}

uint8_t check_var(token_t var, dynamic_array_t* vars){
	variable_t* v = vars->data;
	for(int i = 0; i < vars->size; i++){
		if(memcmp(v[i].name, var.str_view, var.str_view_lenght) == 0){
			return 0;
		}
	}

	return 1;
}

uint8_t check_push_var(token_t var, dynamic_array_t* vars){
	if(check_var(var, vars) == 0) return 1;
	push_var(vars, var);
	return 0;
}

uint8_t push_var(dynamic_array_t* vars, token_t var){
	check_array_capacity(vars, sizeof(variable_t));
	variable_t* variables = vars->data;

	variables[vars->size] = (variable_t){
		.name = var.str_view, 
		.name_size = var.str_view_lenght,
	};
	vars->size++;
	return 0;
}

uint8_t check_placeholder(){
	return 0;
}