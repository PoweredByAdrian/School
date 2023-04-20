;Pivní pøíklad: První pivo (Polotmavý démon) stojí 25 Kè, druhé pivo (Polièka) stojí 20 Kè, tøetí pivo (Páter) stojí 21 Kè. Va¹ím úkolem je udìlat pivní kalkulaèku. Výsledná cena bude v registru EAX. Ceny jednotlivých piv ulo¾te do pole o velikosti 32 bitù, poèty vypitých piv pak do pole o velikosti 16 bitù. Pozice v poli vypitých piv bude odpovídat typu piva v prvním poli.
;Autor: T. Goldmann

%include "rw32-2017.inc"

section .data
    pivo_cena dw 25,20,21
    pocet_vypitych dw 0,2,0
    str_beer db "Celkem k zaplaceni: ",10,0

section .text
main:
    mov ebp, esp
    
    ;v CX bude suma
    mov cx,0
    
    mov ax,[pivo_cena]
    mov bx, [pocet_vypitych]
    mul bx
    add cx,ax
    
    mov ax,[pivo_cena+2]
    mov bx, [pocet_vypitych+2]
    mul bx
    add cx,ax
    
    mov ax,[pivo_cena+4]
    mov bx, [pocet_vypitych+4]
    mul bx
    add cx,ax
    
    
    mov esi, str_beer
    call WriteString
    
    mov ax, cx
    call WriteUInt16

    xor eax, eax
    ret