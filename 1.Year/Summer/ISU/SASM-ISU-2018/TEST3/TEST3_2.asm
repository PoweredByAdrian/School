%include "rw32-2020.inc"

section .data
    
    x dd 15.50
    y dd 15.50

section .text
                        ;std0   std1    std2    std3;
task32:
    push ebp
    mov ebp, esp
    

    
    push __float32__(2.0)
    push __float32__(7.750) ;ebp - 4
    
    mov eax, 0xffffffff
    push eax
    
    fld dword [ebp + 12]   ;y
    fld dword [ebp - 8]   ;7.750    y
    fadd                  ; y+7.75
    
    fldz                  ;0 y+7.75
    fcomip                 ; y+7.75
    ja nula
    
    fsqrt                 ;sqrt()
    fld dword [ebp + 8]    ;x    sqrt()
    fldpi                  ; pi   x   sqrt()
    fmul                  ;pi*x   sqrt()
    fld dword [ebp +12]     ; y   pi*x   sqrt
    fadd                    ; y+pi*x    sqrt()
    fcos
    fsub                   ;vrch
    fld dword [ebp + 8]     ;x   vrch
    fld dword [ebp - 4]     ;2   x     vrch
    fdiv                   ;x/2    vrch
    fld dword [ebp + 12]     ;y    x/2    vrch
    fadd                    ;y+x/2    vrch
    
    fldz                    
    fcomip
    je nula2
    
    fabs                    ;abs()    vrch
    fdiv

    jmp end
    
    nula2:
    fstp st0
    nula:

     fstp  st0
     fld dword [ebp - 12]
     

    end:
    mov esp, ebp
    pop ebp
    ret
_main:
    push ebp
    mov ebp, esp
    
    push dword [y]
    push dword [x]
    call task32
    
    
    
    xor eax, eax
    pop ebp
    ret