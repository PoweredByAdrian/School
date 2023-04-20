%include "rw32-2020.inc"

section .data
    pole dd 1,2,3,4,5,6
    hladam dd 2
section .text
_main:
    push ebp
    mov ebp, esp
    
    mov eax, pole
    mov ebx, [hladam]
    mov ecx, 6
    
    call _task21
    
    call WriteInt32
    pop ebp
    ret
    
_task21:
    push ebp
    mov ebp, esp
    
    
    for:

    cmp ebx, [eax + (4 * ecx - 4)]
    je rovnake
    
    loop for
    jmp rozne
    
    rovnake:
    mov eax, 1
    jmp end
    
    rozne:
    mov eax, 0
    
    end:
    pop ebp
    ret