//Documentation: psABI-x86_64, 3.4.1 Process Initialization/Initial Stack and Register State
//c++ _start.S -static -fno-exceptions -fno-rtti -nostdlib -fno-threadsafe-statics
#pragma once

extern "C"{
using init_f_type  = void(*)(void);
extern init_f_type __preinit_array_start[];
extern init_f_type __preinit_array_end[];
extern init_f_type __init_array_start[];
extern init_f_type __init_array_end[];
extern init_f_type __fini_array_start[];
extern init_f_type __fini_array_end[];
}

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
}

extern "C"
[[noreturn]] void execution_start [[gnu::used]]
 (char** argv_beg,char** argv_end,char** envp_beg,char** envp_end, sys::auxv_t* auxv_beg,sys::auxv_t* auxv_end);
