; program.asm
; Simple x86-64 Linux program to print "Hello, World!"

section .data
    msg db "Hello, World!", 0xA   ; 0xA is newline
    len equ $ - msg                ; length of the string

section .text
    global _start

_start:
    ; write(1, msg, len)
    mov rax, 1      ; sys_write
    mov rdi, 1      ; file descriptor: stdout
    mov rsi, msg    ; pointer to message
    mov rdx, len    ; length of message
    syscall

    ; exit(0)
    mov rax, 60     ; sys_exit
    xor rdi, rdi    ; exit code 0
    syscall
