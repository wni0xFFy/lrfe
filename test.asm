# x86_64 asm/GAS

.intel_syntax noprefix
.global _start

_start:
    mov rbp, rsp
    mov byte ptr [rbp - 0x1], 0
    mov byte ptr [rbp - 0x2], 48

    mov al, [rbp - 0x1]
    mov ah, [rbp - 0x2]

    cmp al, 0
    jne end
        mov rax, 1
        mov rdi, 1
        lea rsi, [rbp - 0x2]
        mov rdx, 1
        syscall
    end:

    mov rax, 60
    mov rdx, 0
    syscall