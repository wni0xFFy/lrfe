#include <assembly.h>
#include <parser.h>
#include <lexer.h>
#include <coreutils.h>
#include <stdlib.h>
#include <stdio.h>

int create_unique_label();

dynamic_array_t gen_instructions(node_t* root){
    return (dynamic_array_t){0};
}

instruction* gen_inst(callstack_t* stack, node_t* node){
   // instruction* inst = malloc(2 * sizeof(instruction));
    insttemplate tmp = select_template(*node);
    instruction* inst = create_instruction(tmp, node, stack);
    fprintf(stdout, "%d", inst[1].work.args_count);
    return NULL;
}

insttemplate select_template(node_t node){
    static instruction_type aviable_inst[] = {
        (instruction_type){.args_count = 2, .type = MOV},
        (instruction_type){.args_count = 0, .type = RESERVE},
        (instruction_type){.args_count = 3, .type = COMPARE},
        (instruction_type){.args_count = 1, .type = JUMP_LABEL},
        (instruction_type){.args_count = 1, .type = CREATE_LABEL},
        (instruction_type){.args_count = 0, .type = _CODEBLOCK},
    };

    static insttemplate templates[] = {
        (insttemplate){
            .type = {
                &aviable_inst[1], 
                &aviable_inst[0]
             }, 
            .instructions_count = 2, 
            .token = KEYWORD_VAR
        },
        (insttemplate){
            .type = {
                &aviable_inst[2], 
                &aviable_inst[3], 
                &aviable_inst[5], 
                &aviable_inst[4]
            }, 
            .instructions_count = 4, 
            .token = KEYWORD_IF,
        },
    };

    for(unsigned long i = 0; i < sizeof(templates) / sizeof(insttemplate); i++){
        if(node.token.type == templates[i].token) return templates[i];
    } 
    return (insttemplate){0};
}

instruction* create_instruction(insttemplate template, node_t* node, callstack_t* stack){
    instruction* instructions = malloc(template.instructions_count * sizeof(instruction));
    for(int i = 0; i < template.instructions_count; i++){
        instructions[i].work.args_count = template.type[i]->args_count;
        instructions[i].work.type = template.type[i]->type;

        switch(instructions[i].work.type){
            case JUMP_LABEL: {
                instructions[i].args[0].str_view_lenght = create_unique_label(); 
                push_stack(stack, instructions[i].args[0].str_view_lenght); 
                fprintf(stdout, ".jp %d\n", instructions[i].args[0].str_view_lenght);
                continue;
            }

            case _CODEBLOCK: {
                fprintf(stdout, "CODEBLLOOOCK\n");
                continue;
            }

            case CREATE_LABEL: {
                instructions[i].args[0].str_view_lenght = pop_stack(stack); 
                fprintf(stdout, ".label %d\n", instructions[i].args[0].str_view_lenght);
                continue;
            }

            default: fprintf(stdout, "h\n");
        }

        for(int k = 0; k < instructions[i].work.args_count; k++){instructions[i].args[k] = node->child[k].token;}
    }
    return instructions;
}

int create_unique_label(){
    static int counter = 0;
    counter++;
    return counter;
}


//              ------stack function------
callstack_t create_stack(uint8_t depth){
    uint8_t* labels = malloc(depth);
    return (callstack_t){.labels = labels, .sp = labels - 1, .depth = depth};
}
//              ------stack function------
void push_stack(callstack_t* stack, uint8_t label){
    stack->sp += 1;
    if(stack->sp - stack->labels == stack->depth) return; 
    *(stack->sp) = label;
}
//              ------stack function------
uint8_t pop_stack(callstack_t* stack){
    uint8_t label = *(stack->sp);
    stack->sp -= 1;
    return label;
}