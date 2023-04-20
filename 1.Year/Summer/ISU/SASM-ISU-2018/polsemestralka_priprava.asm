%include "rw32-2020.inc"

section .data
    ; zde budou vase data

section .text
_main:
    push ebp
    mov ebp, esp
    
    mov al, 24                          ;?
    mov bl, 7                       ;? 00000111
    mov cl, 10                          ; ? 
    mul cl                              ;ax=240     
    add ah, bl                          ; al = 2032    ah:al = 00000111:11110000     
    mov dl, 100      
    div dl                              ;ax = 8212      ah:al = 00100000:00010100         32:20
    inc ax                              ;ax = 8213 ah:al = 00100000:00010101
    add al, ah                               ;al = 53    ah:al = 00100000:00110101
    sub al, bl                          ;al = 46       
    cmp al, 47                          ;?   46
    JAE hop1
    add al, -128                            ;?  ;-128
    hop1:
        ;al = -82
        call WriteInt8

    pop ebp
    ret