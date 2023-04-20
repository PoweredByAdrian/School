%include "rw32-2020.inc"

section .data
    ; zde budou vase data

section .text
_main:
    push ebp
    mov ebp, esp
    
    mov al, 32                ; al=32                                                           ;?
    mov bl, 7                ; bl=7                                                             ;? 00000111
    mov cl, 30                ; cl=30                                                        ; ? 
    mul cl        ;ax=960    ;      ah:al 0000 0011:1100 0000                                                       ;ax=240     
    add ah, bl               ;   ax=2752    ah:al 0000 1010:1100 0000    10:192                                                 ; al = 2032    ah:al = 00000111:11110000     
    mov dl, 100              ;
    div dl        ;ax=13339  ; ah:al = 0011 0100:0001 1011    52:27                                                         ;ax = 8212      ah:al = 00100000:00010100         32:20
    inc ax                   ;  ax=13340    0011 0100:0001 1100  52:28                                               ;ax = 8213 ah:al = 00100000:00010101
    add al, ah    ;al=80     ;                                                            ;al = 53    ah:al = 00100000:00110101
    sub al, bl               ;  80-7  73                                                               ;al = 46       
    cmp al, 74                ;  22                                                   ;?   46
    JAE hop1                 ;
    add al, -128                ; -76                                                                     ;?  ;-128
    hop1:                    ;
        ;al = -55
        call WriteInt8

    pop ebp
    ret