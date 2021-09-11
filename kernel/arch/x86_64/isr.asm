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
%macro make_exception_handler_err 1
exception_handler%+%1:
    push qword %1
    jmp handle_isr_common
%endmacro

; same as above but since not all
; interrupts produce an error code,
; we add the dummy error code 0
%macro make_exception_handler_noerr 1
exception_handler%+%1:
    push qword 0
    push qword %1
    jmp handle_isr_common
%endmacro


%macro make_irq_handler 1
interrupt_handler%+%1:
    push qword 0
    push qword %1
    jmp handle_irq_common
%endmacro


section .text
; defined in interrupts.hpp
extern cpu_exception_handler
extern irq_interrupt_handler

; common code that basically
; calls the cpu exception handler
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


; same as above but calls the irq handler
handle_irq_common:
    cld
    pusha64
    mov rdi, rsp
    call irq_interrupt_handler
    popa64
    add rsp, 16
    iretq


make_exception_handler_noerr 0
make_exception_handler_noerr 1
make_exception_handler_noerr 2
make_exception_handler_noerr 3
make_exception_handler_noerr 4
make_exception_handler_noerr 5
make_exception_handler_noerr 6
make_exception_handler_noerr 7
make_exception_handler_err   8
make_exception_handler_noerr 9
make_exception_handler_err   10
make_exception_handler_err   11
make_exception_handler_err   12
make_exception_handler_err   13
make_exception_handler_err   14
make_exception_handler_noerr 15
make_exception_handler_noerr 16
make_exception_handler_err   17
make_exception_handler_noerr 18
make_exception_handler_noerr 19
make_exception_handler_noerr 20
make_exception_handler_err   21
make_exception_handler_noerr 22
make_exception_handler_noerr 23
make_exception_handler_noerr 24
make_exception_handler_noerr 25
make_exception_handler_noerr 26
make_exception_handler_noerr 27
make_exception_handler_noerr 28
make_exception_handler_noerr 29
make_exception_handler_err   30
make_exception_handler_noerr 31


make_irq_handler 32
make_irq_handler 33
make_irq_handler 34
make_irq_handler 35
make_irq_handler 36
make_irq_handler 37
make_irq_handler 38
make_irq_handler 39
make_irq_handler 40
make_irq_handler 41
make_irq_handler 42
make_irq_handler 43
make_irq_handler 44
make_irq_handler 45
make_irq_handler 46
make_irq_handler 47
make_irq_handler 48
make_irq_handler 49
make_irq_handler 50
make_irq_handler 51
make_irq_handler 52
make_irq_handler 53
make_irq_handler 54
make_irq_handler 55
make_irq_handler 56
make_irq_handler 57
make_irq_handler 58
make_irq_handler 59
make_irq_handler 60
make_irq_handler 61
make_irq_handler 62
make_irq_handler 63
make_irq_handler 64
make_irq_handler 65
make_irq_handler 66
make_irq_handler 67
make_irq_handler 68
make_irq_handler 69
make_irq_handler 70
make_irq_handler 71
make_irq_handler 72
make_irq_handler 73
make_irq_handler 74
make_irq_handler 75
make_irq_handler 76
make_irq_handler 77
make_irq_handler 78
make_irq_handler 79
make_irq_handler 80
make_irq_handler 81
make_irq_handler 82
make_irq_handler 83
make_irq_handler 84
make_irq_handler 85
make_irq_handler 86
make_irq_handler 87
make_irq_handler 88
make_irq_handler 89
make_irq_handler 90
make_irq_handler 91
make_irq_handler 92
make_irq_handler 93
make_irq_handler 94
make_irq_handler 95
make_irq_handler 96
make_irq_handler 97
make_irq_handler 98
make_irq_handler 99
make_irq_handler 100
make_irq_handler 101
make_irq_handler 102
make_irq_handler 103
make_irq_handler 104
make_irq_handler 105
make_irq_handler 106
make_irq_handler 107
make_irq_handler 108
make_irq_handler 109
make_irq_handler 110
make_irq_handler 111
make_irq_handler 112
make_irq_handler 113
make_irq_handler 114
make_irq_handler 115
make_irq_handler 116
make_irq_handler 117
make_irq_handler 118
make_irq_handler 119
make_irq_handler 120
make_irq_handler 121
make_irq_handler 122
make_irq_handler 123
make_irq_handler 124
make_irq_handler 125
make_irq_handler 126
make_irq_handler 127
make_irq_handler 128
make_irq_handler 129
make_irq_handler 130
make_irq_handler 131
make_irq_handler 132
make_irq_handler 133
make_irq_handler 134
make_irq_handler 135
make_irq_handler 136
make_irq_handler 137
make_irq_handler 138
make_irq_handler 139
make_irq_handler 140
make_irq_handler 141
make_irq_handler 142
make_irq_handler 143
make_irq_handler 144
make_irq_handler 145
make_irq_handler 146
make_irq_handler 147
make_irq_handler 148
make_irq_handler 149
make_irq_handler 150
make_irq_handler 151
make_irq_handler 152
make_irq_handler 153
make_irq_handler 154
make_irq_handler 155
make_irq_handler 156
make_irq_handler 157
make_irq_handler 158
make_irq_handler 159
make_irq_handler 160
make_irq_handler 161
make_irq_handler 162
make_irq_handler 163
make_irq_handler 164
make_irq_handler 165
make_irq_handler 166
make_irq_handler 167
make_irq_handler 168
make_irq_handler 169
make_irq_handler 170
make_irq_handler 171
make_irq_handler 172
make_irq_handler 173
make_irq_handler 174
make_irq_handler 175
make_irq_handler 176
make_irq_handler 177
make_irq_handler 178
make_irq_handler 179
make_irq_handler 180
make_irq_handler 181
make_irq_handler 182
make_irq_handler 183
make_irq_handler 184
make_irq_handler 185
make_irq_handler 186
make_irq_handler 187
make_irq_handler 188
make_irq_handler 189
make_irq_handler 190
make_irq_handler 191
make_irq_handler 192
make_irq_handler 193
make_irq_handler 194
make_irq_handler 195
make_irq_handler 196
make_irq_handler 197
make_irq_handler 198
make_irq_handler 199
make_irq_handler 200
make_irq_handler 201
make_irq_handler 202
make_irq_handler 203
make_irq_handler 204
make_irq_handler 205
make_irq_handler 206
make_irq_handler 207
make_irq_handler 208
make_irq_handler 209
make_irq_handler 210
make_irq_handler 211
make_irq_handler 212
make_irq_handler 213
make_irq_handler 214
make_irq_handler 215
make_irq_handler 216
make_irq_handler 217
make_irq_handler 218
make_irq_handler 219
make_irq_handler 220
make_irq_handler 221
make_irq_handler 222
make_irq_handler 223
make_irq_handler 224
make_irq_handler 225
make_irq_handler 226
make_irq_handler 227
make_irq_handler 228
make_irq_handler 229
make_irq_handler 230
make_irq_handler 231
make_irq_handler 232
make_irq_handler 233
make_irq_handler 234
make_irq_handler 235
make_irq_handler 236
make_irq_handler 237
make_irq_handler 238
make_irq_handler 239
make_irq_handler 240
make_irq_handler 241
make_irq_handler 242
make_irq_handler 243
make_irq_handler 244
make_irq_handler 245
make_irq_handler 246
make_irq_handler 247
make_irq_handler 248
make_irq_handler 249
make_irq_handler 250
make_irq_handler 251
make_irq_handler 252
make_irq_handler 253
make_irq_handler 254
make_irq_handler 255



section .data

; declare our arrays of handlers using nasm macros
global exceptions_handlers
exceptions_handlers:
%assign i 0
%rep 32
    dq exception_handler%+i
%assign i i+1
%endrep

global interrupts_handlers
interrupts_handlers:
%assign i 32
%rep 223
    dq interrupt_handler%+i
%assign i i+1
%endrep




