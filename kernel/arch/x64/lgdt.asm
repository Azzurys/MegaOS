bits 64

global asm_lgdt
asm_lgdt:
    lgdt [rdi]

    ; reload segment selectors
    mov ax, 0x30
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; push the segment value
    ; push the return address
    ; retfq will pop both in cs:rip
    pop rdi
    mov rax, 0x28
    push rax
    push rdi
    retfq


