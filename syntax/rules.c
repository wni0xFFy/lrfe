#include "_internal_syntax.h"
#include <stdlib.h>
#include <string.h>

uint8_t check_var(token_t var, dynamic_array_t* vars);
uint8_t check_push_var(token_t var, dynamic_array_t* vars);
uint8_t check_placeholder();

node_rule_t* select_rule(token_t token, int* rules_lenght){
	static const token_rule_t var_token_rules[] = {
		(token_rule_t){.checker = check_push_var, .tok = VARIABLE},
		(token_rule_t){.checker = check_placeholder, .tok = NUM_LITERAL},
	};

	static const token_rule_t other_var_token_rules[] = {
		(token_rule_t){.checker = check_push_var, .tok = VARIABLE},
		(token_rule_t){.checker = check_placeholder, .tok = VARIABLE},
	};

	static const token_rule_t avrrules[] = {
		(token_rule_t){.checker = check_placeholder, .tok = KEYWORD_IF},
		(token_rule_t){.checker = check_placeholder, .tok = VARIABLE},
	};

	static const token_rule_t print_token_rules[] = {
		(token_rule_t){.checker = check_var, .tok = VARIABLE},
	};

	static const token_rule_t if_token_rules[] = {
		(token_rule_t){.checker = check_var, .tok = VARIABLE},
		(token_rule_t){.checker = check_placeholder, .tok = OPERATOR_EQUAL},
		(token_rule_t){.checker = check_var, .tok = VARIABLE},
	};

	static node_rule_t rules[] = {
		(node_rule_t){.token = KEYWORD_VAR, .args_count = 2, .rules = &var_token_rules[0]},
		(node_rule_t){.token = KEYWORD_VAR, .args_count = 2, .rules = &other_var_token_rules[0]},
		(node_rule_t){.token = KEYWORD_VAR, .args_count = 2, .rules = &avrrules[0]},
		(node_rule_t){.token = KEYWORD_PRINT,.args_count = 1, .rules = &print_token_rules[0]},
		(node_rule_t){.token = KEYWORD_IF,.args_count = 3, .rules = &if_token_rules[0]},
	};

	node_rule_t* alloc_rules = malloc(sizeof(rules));
	memcpy(alloc_rules, rules, sizeof(rules));
	for(unsigned long i = 0; i < sizeof(rules) / sizeof(node_rule_t); i++){
		if(rules[i].token == token.type){
			alloc_rules += i;
			while(rules[i].token == alloc_rules->token){
				*rules_lenght += 1;
				i++;
			}
			return alloc_rules;
		}
	}
    return alloc_rules;
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

uint8_t check_placeholder(){return 1;}