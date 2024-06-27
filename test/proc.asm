jmp main

main:
    call proc
    mov rbx, 10
    halt

proc:
    mov rax, 2
    ret