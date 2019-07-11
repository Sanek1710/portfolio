.DATA
    y: .dw 0x00
    c: .dw 0x00
    b: .dw 0x00
    x: .dw 0x00
    .mem:
.CODE
    mov  eax, [y]
    mov  ebx, 1
    add  eax, ebx
    mov  [y], eax
    mov  eax, [c]
    mov  ebx, [c]
    mov  [.mem+00h], ebx
    mov  [.mem+04h], eax
    mov  eax, 1
    sub  ebx, eax
    mov  [c], ebx
    mov  ebx, [.mem+04h]
    mov  eax, [b]
    mov  [.mem+04h], ebx
    mov  ebx, [c]
    add  eax, ebx
    mov  ebx, 5
    mov  [.mem+08h], eax
    mov  eax, [c]
    mov  [.mem+0Ch], eax
    mov  [.mem+10h], ebx
    mov  ebx, 1
    add  eax, ebx
    mov  [c], eax
    mov  eax, [.mem+10h]
    mov  ebx, [.mem+0Ch]
    mlt  ebx, eax
    mov  eax, [.mem+08h]
    sub  eax, ebx
    mov  ebx, [.mem+04h]
    mlt  ebx, eax
    mov  eax, [x]
    div  ebx, eax
    mov  eax, 9
    div  ebx, eax
    mov  eax, [.mem+00h]
    add  eax, ebx
    mov  [c], eax
    mov  eax, [c]
    mov  ebx, [b]
    cmp  eax, ebx
    jle  .+0Ah
    mov  eax, 1
    jmp  .+08h
    mov  eax, 0
    tst  eax
    je   _P000
    mov  eax, [c]
    call print
    jmp  _P001
_P000:
    mov  eax, [b]
    call print
_P001:
    ret  
