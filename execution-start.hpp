//Documentation: psABI-x86_64, 3.4.1 Process Initialization/Initial Stack and Register State
//c++ _start.S -static -fno-exceptions -fno-rtti -nostdlib -fno-threadsafe-statics
#pragma once

#include "stddef.h"
#include <asm/unistd.h>
#include <asm/prctl.h>

extern "C"{
using init_f_type  = void(*)(void);
extern init_f_type __preinit_array_start[];
extern init_f_type __preinit_array_end[];
extern init_f_type __init_array_start[];
extern init_f_type __init_array_end[];
extern init_f_type __fini_array_start[];
extern init_f_type __fini_array_end[];
}
//
namespace sys{

inline void execute_init_array(){
  init_f_type* beg = __preinit_array_start;
  init_f_type* end = __preinit_array_end;
  for(;beg!=end;++beg) (*beg)();
  beg = __init_array_start;
  end = __init_array_end;
  for(;beg!=end;++beg) (*beg)();
  }

inline void execute_fini_array(){
  auto beg = __fini_array_start;
  auto end = __fini_array_end;
  for(;beg!=end;++beg) (*beg)();
  }

struct auxv_t{
  size_t a_type;//int in the document but above it is specified as 2*8bytes
  union{
    long a_val;
    void* a_ptr;
    void (*a_fnc)();
    } a_un;
  };
#define BE_STRINGIFY(x) KAPI_STRINGIFY_(x)
#define   BE_STRINGIFY_(x) #x
inline int set_tls_buffer(void* buff){
  register int ret asm("rax")=__NR_arch_prctl;
  register int com asm("rdi")=ARCH_SET_FS;
  register void* buff_ asm("rsi") = buff;
  asm volatile(
    //"movq $" BE_STRINGIFY(__NR_arch_prctl) ", %%rax\n\t"
    //"movq $" BE_STRINGIFY(ARCH_SET_FS) ", %%rdi\n\t"
    "syscall\n"
    :"=r"(ret)
    :"r"(buff),"r"(ret),"r"(com)
    :"r11","rcx","r9","r8","r10","rdx","memory");
  return ret;
  }
#undef BE_STRINGIFY
#undef BE_STRINGIFY_
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-declarations"
extern "C"
int execution_start [[gnu::used]]
 (char** argv_beg,char** argv_end,char** envp_beg,char** envp_end, sys::auxv_t* auxv_beg,sys::auxv_t* auxv_end);
//  {
//  execute_init_array();
//  auto x=nsx_main(nsx::range_view(argv_beg,argv_end),nsx::range_view(envp_beg,envp_end),nsx::range_view(auxv_beg,auxv_end));
//  execute_fini_array();
//  return x;
//  }
#pragma GCC diagnostic pop
