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
/*nullify rbp as required by doc*/
//at function intry rbp is pushed on the stack, it indicates caller base frame address
    xorq %rbp, %rbp
/*register function in rdx...*/
    //movq %rdx, %r10 
    //=>rdx is a pointer to near the top none executable memory...
    //=>probably linux does not respect System V Application Binary Interface
/*move argc to the first arg*/
    movq (%rsp),%rdi
/*move *argv start at %8(rsp) */
    movq %rsp, %rsi
    addq $8,%rsi
    callq execution_start//rsp is already well aligned 
    hlt//in case execution_start would return (illegal instruction in user mode)
    .cfi_endproc
