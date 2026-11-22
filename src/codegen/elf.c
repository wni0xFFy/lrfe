#include <stdint.h>
#include <coreutils.h>
#include <codegen.h>
#include <stdlib.h>
#include <string.h>

//check docs on https://en.wikipedia.org/wiki/Executable_and_Linkable_Format
const uint8_t BASE_ELF_SHIT[] = {
    0x7f, 'E', 'L', 'F', 0x2, 0x1, 0x1, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, //e_ident
    0x2,0x0,//e_type
    0x3E,0x0,//e_machine
    0x1,0x0,0x0,0x0,//e_version
    0x78, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, //e_entry(virtual address) 
    0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0, //header table offset e_phoff
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0, //e_shoff
    0x0,0x0,0x0,0x0, //e_flags
    0x40,0x0, //e_ehsize
    0x38,0x0, //e_phentsize
    0x1,0x0,//e_phnum
    0x40,0x0, //e_shentsize
    0x0,0x0, //e_shnum
    0x0,0x0, //e_shstrndx 64
    //PROGRAMM HEADER
    0x1,0x0,0x0,0x0,//p_type
    0x5,0x0,0x0,0x0,//p_flags
    0,0,0,0,0,0,0,0, //p_offset
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,//p_vaddr
    0,0,0,0,0,0,0,0,//p_paddr
    0x78,0,0,0,0,0,0,0,//p_filesz 96
    0x78,0,0,0,0,0,0,0,//p_memsz 104
    0,0,0,0,0,0,0,0,//p_align
};

dynamic_array_t create_elf_bytes(dynamic_array_t instructions){
    instruction_t* instrs = instructions.data;
    int executable_size = 0;
    for(int i = 0; i < instructions.size; i++) executable_size += instrs[i].instruction_size;

    uint8_t* output = malloc(120 + executable_size);
    memcpy(output, BASE_ELF_SHIT, 120);
    output[96] += executable_size;
    output[104] += executable_size;
    output += 120;
    for(int i = 0; i < instructions.size; i++){
        memcpy(output, instrs[i].bytes, instrs[i].instruction_size);
        output += instrs[i].instruction_size;
    }

    output -= executable_size + 120;
    return (dynamic_array_t){
        .data = output,
        .size = 120 + executable_size,
    };
}