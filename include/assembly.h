#pragma once

#include <lexer.h>
#include <parser.h>
typedef enum{
    RESERVE,
    MOV,
    COMPARE,
    JUMP_LABEL,
    CREATE_LABEL,
    _CODEBLOCK
} instruction_type_enum;

typedef struct{
    uint8_t* labels;
    uint8_t depth;
    uint8_t* sp;
} callstack_t;

uint8_t pop_stack(callstack_t* stack);
callstack_t create_stack(uint8_t depth);
void push_stack(callstack_t* stack, uint8_t label);

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

instruction* gen_inst(callstack_t* st, node_t* node);
insttemplate select_template(node_t node);
instruction* create_instruction(insttemplate template, node_t* node, callstack_t* stack);