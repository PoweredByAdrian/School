%include "rw32-2020.inc"

section .data
    ; zde budou vase data

section .text
_main:
    push ebp
    mov ebp, esp
    
    call ReadUInt32_Silent ;p1
    mov ecx, eax
    call ReadUInt32_Silent ;p2
    mov ebx, eax
    call ReadUInt32_Silent ;p3
    
    push eax
    push ebx
    push ecx

    call _maxindex
    add esp, 3 * 4
    
    call WriteUInt32
    
    call _func
    
    pop ebp
    ret
    
_maxindex:
    push ebp
    mov ebp, esp
    
    push ebx
    push ecx
    push edx
    
    mov ebx, [ebp + (1+1) * 4] ;p1
    mov ecx, [ebp + (1+2) * 4] ;p2
    mov edx, [ebp + (1+3) * 4] ;p3
    
    pop edx
    pop ecx
    pop ebx
    
    pop ebp
    ret
    
    
    
_func:
    push ebp
    mov ebp, esp
    
    sub esp, 2*4 ;vytvorenie 2 premenne
    push edx
    
    mov [ebp - (1*4)], dword 42  ;priradenie 42 do prvej premennej
    
    mov eax, [ebp - (1 * 4)] ;priradenie 42 do eax
    call WriteInt32
    
    mov esp, ebp  ;zmazanie premennych
    pop ebp
    ret
