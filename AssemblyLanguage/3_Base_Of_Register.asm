%include "io64.inc"

section .text
global CMAIN
CMAIN:
   
    ; 8 bit = 1 byte
    ; 16 bit = 2 byte = 1 word
    ; 32 bit = 4 byte = 2 word = 1 dword (double-word)
    ; 64 bit = 8 byte = 4 word = 1 qword (quad-word)
    
    PRINT_STRING msg
    
    xor rax, rax
    ret
    
; section .data
