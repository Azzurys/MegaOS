bits 64


; push 64 bit GPRs
%macro pusha64 0
push rax
push rbx
push rcx
push rdx
push rbp
push rdi
push rsi
push r8
push r9
push r10
push r11
push r12
push r13
push r14
push r15
%endmacro

; pop 64 bit GPRs
%macro popa64 0
pop r15
pop r14
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8
pop rsi
pop rdi
pop rbp
pop rdx
pop rcx
pop rbx
pop rax
%endmacro

; pushes interrupt number
; then jumps to the common code
%macro make_isr_err 1
isr_handler%+%1:
    push qword %1
    jmp handle_isr_common
%endmacro

; same as above but since not all
; interrupts produce an error code,
; we add the dummy error code 0
%macro make_isr_noerr 1
isr_handler%+%1:
    push qword 0
    push qword %1
    jmp handle_isr_common
%endmacro


section .text
; defined in interrupts.hpp
extern cpu_exception_handler

; common code that basically
; calls the generic handler
handle_isr_common:
    ; sysv x86-64 abi requires the Direction Flags
    ; to be cleared upon function entry
    ; see 6.2.2 about stack alignment in the Intel SDM volume 1
    cld

    pusha64

    ; pass the pushed registers to the handler
    mov rdi, rsp
    call cpu_exception_handler

    popa64

    ; restore the stack pointer as we pushed
    ; the interrupt number and the error code
    add rsp, 16

    ; return from interrupt,
    ; also re-enable interrupts since it restores RFLAGS
    iretq


make_isr_noerr 0
make_isr_noerr 1
make_isr_noerr 2
make_isr_noerr 3
make_isr_noerr 4
make_isr_noerr 5
make_isr_noerr 6
make_isr_noerr 7
make_isr_err   8
make_isr_noerr 9
make_isr_err   10
make_isr_err   11
make_isr_err   12
make_isr_err   13
make_isr_err   14
make_isr_noerr 15
make_isr_noerr 16
make_isr_err   17
make_isr_noerr 18
make_isr_noerr 19
make_isr_noerr 20
make_isr_err   21
make_isr_noerr 22
make_isr_noerr 23
make_isr_noerr 24
make_isr_noerr 25
make_isr_noerr 26
make_isr_noerr 27
make_isr_noerr 28
make_isr_noerr 29
make_isr_err   30
make_isr_noerr 31


section .data
; declare our array of isr handler using nasm macros
global handlers
handlers:
%assign i 0
%rep 32
    dq isr_handler%+i
%assign i i+1
%endrep




