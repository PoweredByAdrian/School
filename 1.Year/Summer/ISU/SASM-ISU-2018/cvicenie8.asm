%include "rw32-2020.inc"

section .data
    ; zde budou vase data

section .text
_main:
    push ebp
    mov ebp, esp
    
    call ReadUInt32_Silent
    push eax ;m
    call ReadUInt32_Silent 
    push eax ;n
    
    call gdc
    add esp, 2*4 

    call WriteUInt32
    
    pop ebp
    ret
    
gdc:
    push ebp
    mov ebp, esp
    
    mov ebx, [ebp + 8] ;n
    mov ecx, [ebp + 12];m
    
    
    cmp ebx, 0
    je n_nula
    
    xor edx, edx
    mov eax, ecx
    div ebx
    
    push edx
    push ebx
    call gdc
    
    add esp, 8
    
    
    n_nula:
    mov eax, ecx
    
    
    pop ebp
    ret