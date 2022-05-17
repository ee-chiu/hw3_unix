%macro gensys 2
    global sys_%2:function
sys_%2:
    push r10
    mov r10, rcx
    mov rax, %1
    syscall
    pop r10
    ret
%endmacro

extern errno

    section .data
    section .text

    gensys 1, write
    gensys 13, rt_sigaction
    gensys 14, rt_sigprocmask
    gensys 15, rt_sigreturn
    gensys 34, pause
    gensys 35, nanosleep
    gensys 37, alarm
    gensys 60, exit
    gensys 127, rt_sigpending

extern getsigmask
    
    global setjmp:function
setjmp:
    lea rax, [rdi] ; rax = &jb
    mov [rax+8], rsp  ; reg[1] = rsp
    mov [rax+16], rbp ; reg[2] = rbp
    mov [rax+24], r12 ; reg[3] = r12
    mov [rax+32], r13 ; reg[4] = r13
    mov [rax+40], r14 ; reg[5] = r14
    mov [rax+48], r15 ; reg[6] = r15
    pop rcx ; rcx = return addr
    mov [rax+56], rcx ; reg[7] = return addr
    push rcx ; push return addr back
    mov [rax], rbx ; reg[0] = rbx

    lea rdi, [rax+64] ; rdi = &jb.mask 
    call getsigmask WRT ..plt ; getsigmask(&jb.mask)

    xor rax, rax ; return 0
    ret 

extern setsigmask

    global longjmp:function
longjmp:
    lea rax, [rdi] ; rax = &jb
    mov rsp, [rax+8]  ; rsp = reg[1]
    mov rbp, [rax+16] ; rbp = reg[2]
    mov r12, [rax+24] ; r12 = reg[3]
    mov r13, [rax+32] ; r13 = reg[4]
    mov r14, [rax+40] ; r14 = reg[5]
    mov r15, [rax+48] ; r15 = reg[6]
    pop rcx ; pop original return addr
    mov rcx, [rax+56] ; get saved return addr
    push rcx ; push new return addr back
    mov rbx, [rax] ; rbx = reg[0]

    lea rdi, [rax+64] ; rdi = &jb.mask
    call setsigmask WRT ..plt ; setsigmask(&jb.mask)

    mov rax, rsi ; return val
    ret