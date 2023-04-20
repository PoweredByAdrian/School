%include "rw32-2020.inc"

section .data
    a dd 1
    b dw 2
    c dw 3
    pd dd 4

section .text
; int sum4_cdecl(int a, char b, short c, int *pd){
;       return a + b + c + *pd;
; }

; pd ... ebp + 20
; c ... ebp + 16
; b ... ebp + 12
; a ... ebp + 8

sum4_cdecl:
    enter 0,0
    
    mov edi,[ebp+8]
    movsx eax,byte [ebp+12]
    add edi,eax
    movsx eax,word [ebp+14]
    add edi,eax
    mov eax,[ebp+16]
    add edi,[eax]
    
    mov eax, edi
    
    leave
    ret

_main:
    push ebp
    mov ebp, esp
    
    push pd
    push word [c]
    push word [b]
    push dword [a]
    call sum4_cdecl
    add esp,12
    call WriteInt32

    pop ebp
    ret