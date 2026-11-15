#include "_internal_syntax.h"
#include <syntax.h>
#include <parser.h>
#include <lexer.h>
#include <coreutils.h>
#include <stdio.h>
#include <stdlib.h>

void check_syntax_node(int* current_line, int* errors_count, node_t node, dynamic_array_t* vars);
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
	
	int rules_count = 0;
	node_rule_t* rule = select_rule(node.token, &rules_count);
	error_t err = check_node(rule, rules_count, node, vars);

	if(err.code == 2){
			fprintf(stdout, "line %d : %s\n", *current_line ,err.msg);
			free(err.msg);
			*errors_count += 1;
	}

	for(int k = 0; k < node.child_count; k++){
		*current_line += 1;
		check_syntax_node(current_line, errors_count, node.child[k], vars);
	}
}

error_t check_node(node_rule_t* rules, int rules_lenght,node_t node, dynamic_array_t* vars){
	char* expected_token = 0;
	int is_correct = 0;
	node_rule_t curr_rule =  {0};
	for(int rule = 0; rule < rules_lenght; rule++){
		curr_rule = rules[rule];
		for(int i = 0; i < curr_rule.args_count; i++){
			if(node.args[i].type != curr_rule.rules[i].tok){
				expected_token = token_to_string(curr_rule.rules[i].tok);
				is_correct = 0;
				break;
			}

			if(curr_rule.rules[i].checker(node.args[i], vars)){is_correct = 0;}
			is_correct = 1;
		}	

		if(rule + 1 == rules_lenght && is_correct == 0){
			char* error_msg = malloc(1024);
			snprintf(error_msg, 1024, "invalid '%.*s' using. Expected %s", node.token.str_view_lenght, node.token.str_view, expected_token);
			return (error_t){.code = 2, .msg = error_msg};
		}
		if(is_correct){return (error_t){.code = 1, .msg = 0};}
	}
	return (error_t){.code = 1, .msg = 0};
}