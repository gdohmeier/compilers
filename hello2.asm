; https://www.nasm.us/xdoc/2.15.05/html/nasmdoc0.html 
; https://duckduckgo.com/?q=link+nasm+object+file+mac+osx&t=osx&ia=web
; nasm -f macho64 -o hello2.o hello2.asm
; ld -o hello2 -e _main hello2.o -macosx_version_min 10.15 -static
; ld -e _main -o hello2 hello2.o -static
; ./hello2

global _main


section .text

_main:
    mov     rax, 0x2000004 ; write
    mov     rdi, 1 ; stdout
    mov     rsi, msg
    mov     rdx, msg.len
    syscall

    mov     rax, 0x2000001 ; exit
    mov     rdi, 0
    syscall


section .data

msg:    db      "Hello, world!", 10
.len:   equ     $ - msg

