#include <assembly.h>
#include <parser.h>
#include <lexer.h>
#include <coreutils.h>
#include <stdlib.h>
#include <stdio.h>

dynamic_array_t gen_instructions(node_t* root){

}

instruction* gen_inst(node_t* node){
   // instruction* inst = malloc(2 * sizeof(instruction));
    insttemplate tmp = select_template(*node);
    instruction* inst = create_instruction(tmp, node);
    fprintf(stdout, "%d", inst[1].work.args_count);
    return NULL;
}

insttemplate select_template(node_t node){
    static const instruction_type aviable_inst[] = {
        (instruction_type){.args_count = 2, .type = MOV},
        (instruction_type){.args_count = 0, .type = RESERVE},
        (instruction_type){.args_count = 3, .type = COMPARE},
        (instruction_type){.args_count = 1, .type = JUMP_LABEL},
        (instruction_type){.args_count = 1, .type = CREATE_LABEL},
    };

    static const insttemplate templates[] = {
        (insttemplate){
            .type = {
                &aviable_inst[1], &aviable_inst[0]
             }, 
            .instructions_count = 2, 
            .token = KEYWORD_VAR
        },
        (insttemplate){
            .type = {
                &aviable_inst[2], &aviable_inst[3]
            }, 
            .instructions_count = 2, 
            .token = KEYWORD_IF,
        },
    };

    for(unsigned long i = 0; i < sizeof(templates) / sizeof(insttemplate); i++){
        if(node.token.type == templates[i].token) return templates[i];
    } 
    return (insttemplate){0};
}

instruction* create_instruction(insttemplate template, node_t* node){
    /*
        create stack in which we will push current label, 
        needed to return/skip codeblock
    */

    instruction* instructions = malloc(template.instructions_count * sizeof(instruction));
    for(int i = 0; i < template.instructions_count; i++){
        instructions[i].work.args_count = template.type[i]->args_count;
        instructions[i].work.type = template.type[i]->type;
        switch(instructions[i].work.type){
            case JUMP_LABEL: {instructions[i].args[0].str_view = create_unique_label(); continue;}
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