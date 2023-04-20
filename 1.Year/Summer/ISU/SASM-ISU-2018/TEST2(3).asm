%include "rw32-2020.inc"

extern _malloc
section .data
    ; zde budou vase data

section .text
_main:
    push ebp
    mov ebp, esp
    
    mov ecx, 5
    mov ebx, ecx
    cmp ecx, 0
    jbe error
    
    push ecx
    call _malloc
    add esp, 4
    
    cmp eax, 0
    jbe error
    
    
    
    mov ecx, ebx
    for:
    mov ebx, ecx
    sub ebx, 2
    mov edx, ecx
    sub edx, 1
    add ebx, edx
    mov [eax + (2 * ecx) - 2], ebx
    
    loop for
    
    jmp end
    
    call WriteUInt32
    
    error:
    mov eax, 0
    
    end:
    call WriteUInt32
    mov ebx, [eax]
    mov eax, ebx
    call WriteNewLine
    call WriteUInt32
    pop ebp
    ret
    
