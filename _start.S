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
    .cfi_startproc
    .cfi_undefined rip
    //according to elf spec:
    //at stack_start location:
    //   (%rsp) => argc
    //   8(%rsp) => argv_beg
    //   8+argv_end => envp
    //   8+envp => auxv_t
    //   rdx => pointer to initialization function
    //          always null on linux x86 thow
    movq %rsp,%rdi
    movq %rdx,%rsi
    callq _Z5startiPPcPFvvE//rsp is already well aligned 
    hlt//in case execution_start would return (illegal instruction in user mode)
    .cfi_endproc
