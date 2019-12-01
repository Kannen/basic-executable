//documentation of the state at procedure start:
// -System V Application Binary Interface 
//    AMD64 Architecture Processor Supplement 
//      §3.4 Process Initialization
//_start is ld default entry point
//Documentation for function calling convention
// -System V Application Binary Interface 
//    AMD64 Architecture Processor Supplement  / and i386 
//      §3.2 Function calling sequence 
.section .text
.global _start
.type _start,@function
.align 16
_start:
    //.cfi_startproc
    //.cfi_undefined rip
    //
    //rbp already cleared by linux (see ELF_PLAT_INIT definition in arch/x86/include/asm/elf.c, used in fs/binfmt_elf.c)
    //
    //rsp already correctly aligned
    //
    //according to elf spec:
    //at the layout above the stack is:
    //   (%rsp) => argc
    //   8(%rsp) => argv_beg
    //   8+argv_end => envp
    //   8+envp => auxv_t_beg
    //   rdx => pointer to initialization function
    //          always null on linux x86 thow
    //   rsi is already null
    //   so rdx==rsi. (the 2nd argument)
    movq %rsp,%rdi
    callq _Z5startPvPFvvE//rsp is already well aligned 
    hlt//in case execution_start would return (illegal instruction in user mode)
    //.cfi_endproc
