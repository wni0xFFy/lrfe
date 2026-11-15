#include "_internal_syntax.h"
#include <stdlib.h>
#include <string.h>

node_rule_t* select_rule(token_t token, int* rules_lenght){
	static const token_type var_token_rules[] = {
		VARIABLE, NUM_LITERAL
	};

	static const token_type other_var_token_rules[] = {
		VARIABLE, VARIABLE
	};

	static const token_type avrrules[] = {
		KEYWORD_IF, VARIABLE
	};

	static const token_type print_token_rules[] = {
		VARIABLE
	};

	static const token_type if_token_rules[] = {
		VARIABLE, OPERATOR_EQUAL, VARIABLE
	};

	static node_rule_t rules[] = {
		(node_rule_t){.token = KEYWORD_VAR, .args_count = 2, .rules = &var_token_rules[0]},
		(node_rule_t){.token = KEYWORD_VAR, .args_count = 2, .rules = &other_var_token_rules[0]},
		(node_rule_t){.token = KEYWORD_VAR, .args_count = 2, .rules = &avrrules[0]},
		(node_rule_t){.token = KEYWORD_PRINT,.args_count = 1, .rules = &print_token_rules[0]},
		(node_rule_t){.token = KEYWORD_IF,.args_count = 3, .rules = &if_token_rules[0]},
	};

    //if you're here for add syntax rule DO NOT TOUCH ANYTHING BELOW THIS COMMENTARY


    /* 
        if you aren't.
        this code block searchs match
        argument's token type with token in the table
        and after this find the end of this token's rules
    */

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
    return NULL;
}