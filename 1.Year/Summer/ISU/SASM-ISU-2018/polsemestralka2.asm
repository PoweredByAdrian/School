%include "rw32-2020.inc"

section .data
    ; zde budou vase data

section .text
_main:
    push ebp
    mov ebp, esp
   
    MOV AL,-3   ;-3
    AND AX, 255 ;255     ; AL = -3    al=11111101      0000000011111111
    OR AL,1   ;1     ; AL = -3    al=11111101
    XCHG AL, DL    ;
    ROL AX, 8       ;
    XOR AL, DL    ;
    SAR AL,8     ; AL = -1



    pop ebp
    ret