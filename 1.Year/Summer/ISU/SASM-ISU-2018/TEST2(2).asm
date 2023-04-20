%include "rw32-2020.inc"

section .data
    pole dw 1,2,3,4,5,6
    hladam dd 5

section .text
_main:
    push ebp
    mov ebp, esp
    
    push pole
    push dword 6
    push dword [hladam]
    
    
    call _task22@12
    
    
    movsx ebx, word [eax]
    
    mov eax, ebx
    call WriteInt16
    
    pop ebp
    ret
    
    
_task22@12:
    push ebp
    mov ebp, esp
    push ecx
    push ebx
    push edx
    
    mov edx, [ebp + 16]
    mov ecx, dword [ebp + 12]
    movsx ebx, word [ebp + 8]
    
    cmp ecx, 0
    jbe rozne
    
    cmp edx, 0
    je rozne
    
    for:
    movsx eax, word [edx + (2 * ecx  - 2)]
    cmp ebx, eax
    je rovnake
    
    loop for
    jmp rozne
    
    rovnake:
    mov ebx, edx
    mov eax, 2
    mul ecx
    sub eax, 2
    add eax, ebx
    ;mov eax, [edx + (4 * ecx - 4)]
    jmp end
    
    rozne:
    mov eax, 0
    
    end:
    pop edx
    pop ebx
    pop ecx
    pop ebp
    ret 12
