%include "rw32-2020.inc"

section .data


section .text
_main:
    push ebp
    mov ebp, esp
    
    call ReadUInt16  ;  ax = ???????? ????????b = ?????
    mov bl, 58       ;  bl =          00111010b = 58
                     ; -bl =11000110b 
    add al,bl        ;  al=           00011011b
    sub ah, bl       ;  ah= 00000011b
    
    call WriteUInt16 ; ax = 795

    pop ebp
    ret