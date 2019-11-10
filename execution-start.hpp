//Documentation: psABI-x86_64, 3.4.1 Process Initialization/Initial Stack and Register State
//c++ _start.S -static -fno-exceptions -fno-rtti -nostdlib -fno-threadsafe-statics
#pragma once
#include <linux/auxvec.h>

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
  size_t a_type;
  union{
    long a_val;
    void* a_ptr;
    void (*a_fnc)();
    };
  };

struct envp_sentinel_t{
  friend bool operator==(char** envp,envp_sentinel_t){
    return *envp==nullptr;
    }
  friend bool operator==(envp_sentinel_t,char** envp){
    return *envp==nullptr;
    }
  friend bool operator!=(char** envp,envp_sentinel_t){
    return *envp!=nullptr;
    }
  friend bool operator!=(envp_sentinel_t,char** envp){
    return *envp!=nullptr;
    }
  };
inline constexpr auto envp_sentinel = envp_sentinel_t{};
inline char** 
get_envp(char ** argv_end){//ie argv+argc
  return argv_end+1;
  }

struct auxv_t_sentinel_t{
  friend bool operator==(auxv_t* aux,auxv_t_sentinel_t){
    return aux->a_type==AT_NULL;
    }
  friend bool operator==(auxv_t_sentinel_t,auxv_t* aux){
    return aux->a_type==AT_NULL;
    }
  friend bool operator!=(auxv_t* aux,auxv_t_sentinel_t){
    return aux->a_type!=AT_NULL;
    }
  friend bool operator!=(auxv_t_sentinel_t,auxv_t* aux){
    return aux->a_type!=AT_NULL;
    }
  };
inline constexpr auto auxv_t_sentinel = auxv_t_sentinel_t{};

inline auxv_t* 
get_auxv_t(char ** envp_end){//envp_end==envp_sentinel
  return reinterpret_cast<auxv_t*>(envp_end+1);
  }
}

extern "C"
[[noreturn]] void execution_start [[gnu::used]]
  (int argc,char* argv[]);
 //(char** argv_beg,char** argv_end
 //,char** envp_beg,char** envp_end
 //,sys::auxv_t* auxv_beg,sys::auxv_t* auxv_end
 //,void(*atexit_rdx_function)(void));
