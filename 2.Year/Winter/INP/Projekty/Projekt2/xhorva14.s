; Vernamova sifra na architekture DLX
; Adrian Horvath xhorva14

        .data 0x04          ; zacatek data segmentu v pameti
login:  .asciiz "xhorva14"  ; <-- nahradte vasim loginem
cipher: .space 9 ; sem ukladejte sifrovane znaky (za posledni nezapomente dat 0)

        .align 2            ; dale zarovnavej na ctverice (2^2) bajtu
laddr:  .word login         ; 4B adresa vstupniho textu (pro vypis)
caddr:  .word cipher        ; 4B adresa sifrovaneho retezce (pro vypis)

        .text 0x40          ; adresa zacatku programu v pameti
        .global main        ; 

main:   ; sem doplnte reseni Vernamovy sifry dle specifikace v zadani
	
	addi r11, r0, laddr 	;r11 = adresa laddr
	addi r20, r0, caddr 	;r20 = adresa caddr

	
	lb r17, 0(r20) 		;r17= adresa cipher 
	lb r16, 0(r11)		;r16 = adresa login




loop:
	lb r11, 0(r16)		;r11 = neparny znak
	nop
	addi r16, r16, 1	;iteracia cez vstup

	addi r19, r0, 97	;r19 = hodnota najmensiaho mozneho pismena
	sgt  r20, r11, r19	;porovnanie vstupu ci je pismeno
	bnez r20, letter1	; ak je pismeno skoci
	nop
	nop

	j end			;ak neni pismeno tak koniec
	nop
	nop
letter1:
	addi r20, r11, 8 	; r20 = neparny znak + 8 
	addi r19, r0, 123	; r19 = hodnota posledneho pismena v abecede +1
	sgt r11, r19, r20	;porovnanie ci sme napresli cez Z
	bnez r11, ok1		;ak je to ok ulozime na vystup
	nop
	nop

	subi r20, r20, 26	; ak sme presli tak odcitame 26 aby sme urobili rotaciu
ok1:	

	;ulozit hodnotu r20 na vystup
	sb 0(r17), r20
	nop
	addi r17, r17, 1
	
	;//////////////////////////////////////////////////////
	;parne cislo
	;//////////////////////////////////////////////////////


	lb r11, 0(r16)		;r11 = parny znak
	nop
	addi r16, r16, 1   	;iteracia cez vstup
	addi r19, r0, 96	;r19 = hodnota najmensiaho mozneho pismena -1
	sgt  r20, r11, r19	;porovnanie vstupu ci je pismeno
	bnez r20, letter2	; ak je pismeno skoci
	nop
	nop

	j end			;ak neni pismeno tak koniec
	nop
	nop
letter2:
	subi r20, r11, 15 	; r20 = parny znak - 15
	addi r19, r0, 96	; r19 = hodnota prveho pismena v abecede -1
	sgt r11, r20, r19 	; porovnanie ci sme nepresli cez A
	bnez r11, ok2		;ak je to ok ulozive na vystup
	nop
	nop

	addi r20, r20, 26	;pripocitame 26 aby sme urobili rotaciu

ok2:
	;ulozime hodnotu r20 na vystup
	sb 0(r17), r20
	nop
	addi r17, r17, 1
	

	j loop
	nop
	nop






end:	
	addi r17, r17, 1 ;pisanie 0 na koniec 
	addi r20, r0, 0
	nop
	sb 0(r17), r20


	addi r14, r0, caddr ; <-- pro vypis sifry nahradte laddr adresou caddr
        trap 5  ; vypis textoveho retezce (jeho adresa se ocekava v r14)
        trap 0  ; ukonceni simulace

;xhorva14-r11-r16-r17-r19-r20-r0


;h = 8
;o = -15