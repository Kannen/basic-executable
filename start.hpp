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
enum class auxilary_entry_type:unsigned long{
   end_of_vector = AT_NULL,
   to_ignore = AT_IGNORE,
   program_file_descriptor = AT_EXECFD,
   program_headers_begin = AT_PHDR,
   program_header_entry_size = AT_PHENT,
   program_headers_count = AT_PHNUM,
   page_size = AT_PAGESZ,
   interpreter_base_address = AT_BASE,
   _unused_flags = AT_FLAGS,
   program_entry_point = AT_ENTRY,
   not_an_elf_program = AT_NOTELF,
   real_user_id = AT_UID,
   effective_user_id = AT_EUID,
   real_group_id = AT_GID,
   effective_group_id = AT_EGID,
   plateform_name = AT_PLATFORM,
   hardware_capability = AT_HWCAP,
   clock_frequency = AT_CLKTCK,
   is_setuid_like = AT_SECURE,
   real_plateform_name = AT_BASE_PLATFORM,
   random_bytes_address = AT_RANDOM,
   hardware_capability2 = AT_HWCAP2,
   program_file_name = AT_EXECFN,
   vdso_ehdr = AT_SYSINFO_EHDR
   };
struct auxv_t{
  auxilary_entry_type type;
  union{
    long value;
    void* pointer;
    void (*function)();
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
    return aux->type==auxilary_entry_type::end_of_vector;
    }
  friend bool operator==(auxv_t_sentinel_t,auxv_t* aux){
    return aux->type==auxilary_entry_type::end_of_vector;
    }
  friend bool operator!=(auxv_t* aux,auxv_t_sentinel_t){
    return aux->type!=auxilary_entry_type::end_of_vector;
    }
  friend bool operator!=(auxv_t_sentinel_t,auxv_t* aux){
    return aux->type!=auxilary_entry_type::end_of_vector;
    }
  };
inline constexpr auto auxv_t_sentinel = auxv_t_sentinel_t{};

inline auxv_t* 
get_auxv_t(char ** envp_end){//envp_end==envp_sentinel
  return reinterpret_cast<auxv_t*>(envp_end+1);
  }
}

[[noreturn]] void start (int argc,char* argv[]);
