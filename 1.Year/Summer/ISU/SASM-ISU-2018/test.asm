%include "rw32-2020.inc"

section .data
    a db -128
    b dd -20
    c dw -30
    d dw -32760
    e dw -32767
    
    q dd 0
    r dd 0

section .text
_main:

    
    
    push ebp
    mov ebp, esp
    xor eax, eax
    xor ebx, ebx
    xor edx, edx
    xor ecx, ecx
                    
    mov ax, 10
    mov bx, word[d]
    imul bx
    
    shl edx, 16
    mov dx, ax
    
    mov ax, word [e]
    cwde                                                
    add edx, eax
    sub edx, 115
    mov ebx, edx
    
    
    
    
    mov eax, dword [b]
    movsx ecx, word [c]
    imul ecx
    
    

    movsx ecx, byte [a]
    add eax, ecx
    
    adc edx, 0
    sub eax, 25
    
    sbb edx, 0
    idiv ebx
    
    mov [q], eax
    mov [r], edx
    
    pop ebp
    ret