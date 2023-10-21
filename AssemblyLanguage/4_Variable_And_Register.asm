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

    ; rdx의 내용을 rax에다가 집어넣는다.
    
    ; 메모리 <-> 레지스터
    ; mov rax, a ; a라는 바구니의 "주소값"을 rax에 복사
    ; mov rax, [a] ; a라는 바구니 안에 있는 값을 rax에다 복사
    mov al, [a]
    
    mov [a], byte 0x55
    mov [a], word 0x6666
    mov [a], cl
    
    xor rax, rax
    ret
    
    ; 변수의 선언 및 사용
    ; 변수는 데이터를 저장하는 바구니와 같다고 생각하면 된다. [ ]
    ; 바구니를 사용하기 위해서 바구니를 사용할 것이다라는 선언이 필요 (이름과 크기를 지정)
    
    ; 메모리에는 구분할 수 있도록 주소(번지수)가 있다.
    
    ; 초기화 된 데이터
    ; [변수이름] [크기] [초기값]
    ; [크기] db(1) dw(2) dd(4) dq(8)
section .data
    a db 0x11   ; [0x11]
    b dw 0x2222
    c dd 0x33333333
    d dq 0x4444444444444444
    
    ; 초기화되지 않은 데이터
    ; [변수이름] [크기] [개수]
    ; [크기] resb(1) resw(2) resd(4) resq(8)

section .bss
    e resb 10