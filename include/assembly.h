#pragma once

#include <lexer.h>
#include <parser.h>
typedef enum{
    RESERVE,
    MOV,
    COMPARE,
    JUMP_LABEL,
    CREATE_LABEL,
} instruction_type_enum;

typedef struct{ 
    instruction_type_enum type;
    int args_count;
}instruction_type;

typedef struct{
    instruction_type work;
    token_t args[16];
} instruction;

typedef struct{
    instruction_type* type[16];
    token_type token;
    int instructions_count;
} insttemplate;

instruction* gen_inst(node_t* node);
insttemplate select_template(node_t node);
instruction* create_instruction(insttemplate template, node_t* node);