;Vypočítejte obsah lichoběžníku
;Autor: T. Goldmann
%include "rw32-2017.inc"

section .data
    ; write your data here
    a dw 10
    b dw 15
    v dw 3
    
    str_result db "Obsah lichobezniku: ", 10,0

section .text
main:
    mov ebp, esp
    
    ; vypocet ((a+c)*v)/2
    mov ax, [a]
    add ax, [b]
    mov cx, [v]
    mul cx
    xor dx,dx
    mov cx, 2
    div cx
    
    ;vysledek bude v AX
    mov esi, str_result
    call WriteString
    
    ;vypisuje se AX
    call WriteUInt16

    ret