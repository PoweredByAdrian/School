%include "rw32-2020.inc"

extern _qsort

section .data
    ; zde budou vase data

section .text
task31:
    push ebp
    mov ebp, esp
    cmp esi, 0
    jne ok
    mov eax, 0
    cmp eax, 1    
    je end_zf
    
    ok:
    cmp ecx, 0
    jb okok
    mov eax, 0
    cmp eax, 1
    jmp end_zf
    
    okok:
    push dword [comp]
    push dword 32 ;size
    push ecx    ;count
    push esi    ;ptr
    call _qsort
    
    
    end:
    xor eax, eax
    end_zf:
    pop ebp
    ret

comp:
    push ebp
    mov ebp, esp
    
    fld dword[ebp + 8];ptrA
    fld dword [ebp + 12];ptrB
    
    fcomip
    je rovnake
    
    ja Bvacsie
    
    ;A vacsie
    mov eax, -1
    jmp konec
    rovnake:
    mov eax, 0
    jmp konec
    Bvacsie:
    mov eax, 1
    
    konec:
    fstp st0
    mov esp, ebp    
    pop ebp
    ret
_main:
    push ebp
    mov ebp, esp
    
    
    mov edi, 2
    mov esi, 3
    mov ecx, 4
    call task31

    pop ebp
    ret