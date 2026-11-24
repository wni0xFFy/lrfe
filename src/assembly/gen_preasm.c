#include <assembly.h>
#include <parser.h>
#include <lexer.h>
#include <coreutils.h>
#include <stdlib.h>
#include <stdio.h>

int create_unique_label();
void _push_instruction(dynamic_array_t* arr, instruction inst);

dynamic_array_t gen_instructions(node_t* root){
    return (dynamic_array_t){0};
}

instruction gen_inst(dynamic_array_t* insts, callstack_t* stack, node_t* node){
    insttemplate tmp = select_template(*node);
    return create_instruction(insts, tmp, node, stack);
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

instruction create_instruction(dynamic_array_t* arr, insttemplate template, node_t* node, callstack_t* stack){
    instruction inst = {0};
    for(int i = 0; i < template.instructions_count; i++){
        inst.work.args_count = template.type[i]->args_count;
        inst.work.type = template.type[i]->type;

        switch(inst.work.type){
            case JUMP_LABEL: {
                inst.args[0].value = create_unique_label(); 
                push_stack(stack, inst.args[0].value); 
                fprintf(stdout, ".jp %d\n", inst.args[0].value);
                break;
            }
            case COMPARE: {
                inst.args[0] = node->args[0]; inst.args[1] = node->args[2];
                fprintf(stdout,".cmp %.*s, %.*s\n", 1,inst.args[0].str_view, 1  ,inst.args[1].str_view);
                break;
            }
            case MOV: {
                inst.args[0] = node->args[0]; inst.args[1] = node->args[1];
                fprintf(stdout,".mv %.*s, %d\n", 1, inst.args[0].str_view, inst.args[1].value);
                break;
            }
            case _CODEBLOCK: {
                for(int i = 0; i < node->child_count; i++){
                    create_instruction(arr, select_template(node->child[i]), &node->child[i], stack);
                }
                continue;
            }

            case CREATE_LABEL: {
                inst.args[0].str_view_lenght = pop_stack(stack); 
                fprintf(stdout, ".label %d\n", inst.args[0].str_view_lenght);
                break;
            }

            default: fprintf(stdout, "some stuff that i can't process\n");
        }
        _push_instruction(arr, inst);
     //  fprintf(stdout, "\n");
        //for(int k = 0; k < (*output)[i].work.args_count; k++){(*output)[i].args[k] = node->child[k].token;}
    }
    return inst;
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

void _push_instruction(dynamic_array_t* arr, instruction inst){
    check_array_capacity(arr, sizeof(instruction));
    instruction* data = arr->data;
    data[arr->size] = inst;
    arr->size += 1;
}