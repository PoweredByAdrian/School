%include "rw32-2018.inc"

section .data
    sMessage db "Hello World!",10,0 ;10 = novy riadok
                                    ;0 = \0

section .text
_main:
    push ebp
    mov ebp, esp
    
    mov ebx, 5
    mov edi, sMessage2
    call ReadString
    ; eax = pocet nacitanych znakov
    
    mov esi, sMessage
    call WriteString

    pop ebp
    ret