;Vytvoøte pole se ètyømi 16 bitovými prvky. Následnì vypoètìte výraz: x=pole[0]^2+pole[1]+pole[2]/(-2*pole[3])
;Autor: T. Goldmann

%include "rw32-2017.inc"

section .data
    pole dw 10,20,1000,40

section .text
main:
    mov ebp, esp
    
    mov ax, [pole]
    mul ax ;pole[0]^pole[0]
    add ax,[pole+2]
    
    ;ulozeni pole[0]^pole[0]+pole[1] do BX
    mov bx,ax
    
    mov ax, [pole+6];-2*pole[3]
    mov cx,-2
    mul cx
    
    ; -2*pole[3] bude delitel, proto ukladam do cx
    mov cx, ax
    ; delenec pole[2]
    mov ax,[pole+4]
    cwd
    idiv cx
    ;vysledek bude v BX
    add bx,ax
    
    mov ax,bx
    call WriteUInt16
    

    xor eax, eax
    ret