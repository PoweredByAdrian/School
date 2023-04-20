%include "rw32-2020.inc"

section .data
    ; zde budou vase data

section .text
_main:
    push ebp
    mov ebp, esp

    ;int fce (int p1, int p2)
    push dword 40 ;p2
    push dword 1  ;p1
    
    call fce
    
    add esp, 2 * 4 ;odstrani paramerte (2= pocet parametrov 4= velkost (konstanta)

    pop ebp
    ret
    
fce:
    push ebp
    mov ebp, esp
    
    mov eax, [ebp + 8] ;p1
    call WriteInt32
    call WriteNewLine
    
    add eax, [ebp + 12] ; p1 + p2
    
    call WriteInt32
    
    pop ebp
    ret    