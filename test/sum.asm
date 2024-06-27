jmp main

main:
    mov rax, 0
    mov rcx, 10
    jmp calculate

calculate:
    cmp rcx, 0
    jz done
    add rax, rcx
    dec rcx
    jmp calculate


done:
    halt