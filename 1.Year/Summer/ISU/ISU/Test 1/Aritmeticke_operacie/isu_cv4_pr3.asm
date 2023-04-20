;Pivn� p��klad: Prvn� pivo (Polotmav� d�mon) stoj� 25 K�, druh� pivo (Poli�ka) stoj� 20 K�, t�et� pivo (P�ter) stoj� 21 K�. Va��m �kolem je ud�lat pivn� kalkula�ku. V�sledn� cena bude v registru EAX. Ceny jednotliv�ch piv ulo�te do pole o velikosti 32 bit�, po�ty vypit�ch piv pak do pole o velikosti 16 bit�. Pozice v poli vypit�ch piv bude odpov�dat typu piva v prvn�m poli.
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