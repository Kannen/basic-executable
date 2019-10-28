#include <asm/prctl.h>
#include <asm/unistd_64.h>
#ifndef __x86_64__
  NSX_REPORT_UNSUPORTED_PLATEFORM
#endif
.section .text
.global _start
.type _start,@function
.align 16
_start:
    .cfi_startproc
    .cfi_undefined rip
/*nullify rbp as required by doc*/
    xorq %rbp, %rbp
/*set fs:*/
//    movq $__NR_arch_prctl, %rax
//    movq $ARCH_SET_FS, %rdi
//    movq $tls_buffer, %rsi
//    syscall 
/*move argc to rax*/
    popq %rax
/*end set rax to diff (argv.end-argv.begin)*/
    sal $3, %rax
/*set argv*/
    movq %rsp, %rdi
    movq %rsp, %rsi
    addq %rax, %rsi
/*set envp*/
    movq %rsi, %rdx
    addq $8, %rdx /*a null long between argv end envp*/
    movq %rdx, %rcx
/*look for end pointer*/
    cmpq $0, (%rcx)
    je .L_start_elf_entry_built_envp
.L_start_elf_entry_increase_end_envp:
    addq $8, %rcx
    cmpq $0, (%rcx)
    jne .L_start_elf_entry_increase_end_envp
.L_start_elf_entry_built_envp:
/*set auxvt*/
    movq %rcx, %r8
    addq $8, %r8  /*a null long between envp and auxvt*/
    movq %r8, %r9
/*look for null pointer*/
    cmpq $0, (%r9)
    je .L_start_elf_entry_built_auxvt
.L_start_elf_entry_increase_end_auxvt:
    addq $8, %r9
    cmpq $0, (%r9)
    jne .L_start_elf_entry_increase_end_auxvt
.L_start_elf_entry_built_auxvt:
/*align stack*/
  and $~15, %rsp
/*push rsp to the stack*/
  pushq %rax /*add trash to keep alignment*/
  pushq %rsp /*when pushed rsp*/
  callq execution_start
/*call _exit at return*/
  movq %rax,%rdi
  movq $60, %rax
  syscall
  .cfi_endproc

/*.local _write_nsx
.type _write_nsx,@function
.align 16
_write_nsx:
  .cfi_startproc
  movq $__NR_write, %rax
  syscall
  ret
  .cfi_endproc

.global write_nsx
.type write_nsx,@function
.set write_nsx,_write_nsx*/
