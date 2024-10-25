%include "io64.inc"

section .text
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
   
    ; 8 bit = 1 byte
    ; 16 bit = 2 byte = 1 word
    ; 32 bit = 4 byte = 2 word = 1 dword (double-word)
    ; 64 bit = 8 byte = 4 word = 1 qword (quad-word)
    
    ; mov reg1, cst
    ; mov reg1, reg2
    ; 뒤에 있는 내용을 앞에 있는 내용으로 이동한다는 의미이다.
    
    ; ex)
    mov eax, 0x1234
    ; 0x1234 를 eax에다가 이동 시킨다.
    
    mov rbx, 0x12345678
    ; 0x12345678 를 rbx에다가 이동 시킨다.
    
    mov cl, 0xff
    ; 0xff 를 cl에다가 이동시킨다.
    
    mov al, 0x00
    ; 0x00을 al에다가 집어넣는다면?
    ; rax의 내용은 0x1234에서 0x1200으로 변경된다
    ; 이유는 16진수 0x1234를 이진법으로 변환한 후에 al부분인 뒤의 1바이트를 00으로 밀어버리면 0x1200이 된다.    
    mov rax, rdx
    ; rdx의 내용을 rax에다가 집어넣는다.
    
    xor rax, rax
    ret
    
; section .data
