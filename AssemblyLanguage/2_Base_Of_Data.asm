%include "io64.inc"

section .text
global CMAIN
CMAIN:
    ;write your code here
    
    ; 메모를 남길 수 있는 기능 = 주석
    
    ; 10진수 (0 1 2 3 4 5 6 7 8 9)
    
    ; 2진수 (0 1)
    ; 0 1 10 
    ; 0b0 0b1 0b10 0b11 0b100
    
    ; 16진수 (0 1 2 3 4 5 6 7 8 9 A B C D E F)
    ; 0 1 2 3 4 5 6 7 8 9 A B C D E F
    ; 0x00 
    
    PRINT_STRING msg
    
    xor rax, rax
    ret
    
section .data
    msg db 'Hello World', 0x00