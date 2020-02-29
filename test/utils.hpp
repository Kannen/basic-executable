#pragma once
#include <kernel/fs.hpp>
#include <kernel/kernel.hpp>
#include <kernel/error_messages.hpp>

template<int N>
inline void out(const char(&str)[N]){
  using namespace kernel;
  write(standard_output_file,str,N);
  }
inline void out(const char*str){
  using namespace kernel;
  kernel::write(standard_output_file,str,__builtin_strlen(str));
  }
inline void out(long i){
  using namespace kernel;
  char buffer[32];
  auto e = write_num(buffer,i);
  kernel::write(standard_output_file,buffer,e-buffer);
  }
template<int N>
inline void err(const char(&str)[N]){
  using namespace kernel;
  write(standard_error_file,str,N);
  }
inline void err(const char*str){
  using namespace kernel;
  write(standard_error_file,str,__builtin_strlen(str));
  }
inline void err(long i){
  using namespace kernel;
  char buffer[32];
  auto e = write_num(buffer,i);
  kernel::write(standard_error_file,buffer,e-buffer);
  }
template<class T>
inline auto handle_error_str(T v,const char* name){ 
  if (!v){
    err(name);
    err(error_message(v.error()));
    err("\n");
    kernel::exit(v.error());
    }
  return v.value();
  }
template<class T,int N>
inline auto handle_error(T v,const char(&name)[N]){ 
  if (!v){
    err(name);
    err(error_message(v.error()));
    err("\n");
    kernel::exit(v.error());
    }
  return v.value();
  }
inline void handle_error_str(kernel::error_code er,const char*name){ 
  if (er!=kernel::error_code::none){
    err(name);
    err(kernel::error_message(er));
    err("\n");
    kernel::exit(er);
    }
  }
template<int N>
inline void handle_error(kernel::error_code er,const char(&name)[N]){ 
  if (er!=kernel::error_code::none){
    err(name);
    err(kernel::error_message(er));
    err("\n");
    kernel::exit(er);
    }
  }
template<class T,class...Args>
inline auto handle_error_str(T v, Args... strs){ 
  if (!v){
    (err(strs),...);
    err(error_message(v.error()));
    err("\n");
    kernel::exit(v.error());
    }
  return v.value();
  }
template<class...Args>
inline auto handle_error_str(kernel::error_code v, Args... strs){ 
  if (v!=kernel::error_code::none){
    (err(strs),...);
    err(error_message(v));
    err("\n");
    kernel::exit(v);
    }
  return v;
  }
