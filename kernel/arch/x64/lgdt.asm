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

    pop rdi
    mov rax, 0x28
    push rax
    push rdi
    retfq

.reload_cs:
    ret


