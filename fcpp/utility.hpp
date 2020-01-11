#pragma once

#include <fcpp/type_traits.hpp>
#include <utility>
#include <experimental/source_location>

namespace std{
	/// construct an object and avoid pitfoil of T(expr) invoking an
	/// explicit type convertion which may cause a reinterpret cast 
	/// followed by a const cast see [expr.type.cast]
	/// use list initialization for aggregates 
	/// and direct none list initialization for other types
	template<class T,class...Args>
	[[gnu::always_inline]]constexpr inline auto 
	construct (Args&&...args) noexcept(is_nothrow_construct_able_v<T,Args...>)
		-> T
		{
		static_assert(is_object_v<T> && !is_array_v<T> && !is_const_v<T> && !is_volatile_v<T>);
		static_assert(is_construct_able_v<T>);

		if constexpr(!is_class_v<T> && !is_union_v<T>){
			T x(forward<Args&&>(args)...);//avoid functional cast expression
			return x;
			}
		else if constexpr(is_constructible_v<T,Args...>)
			return T(forward<Args>(args)...);
		else //it is an aggregate
			return T{forward<Args>(args)...};
		}

	inline char* write_num(char*const buff,long num){
	  auto end=buff;
	  if (num==0) {*buff='0';return buff+1;}
	  if (num<0) {*end++='-';num = -num;}
	  long l=10;
	  while(l<=num && l<1'000'000'000'000'000'000l) l*=10;
	  if (l>num) l/=10;
	  while(l!=1){
	    long n = num/l;
	    *end++='0'+n;
	    num-=n*l;
	    l/=10;
	    }
	  *end++='0'+num;
	  return end;
	  }
	
	inline char* write_num(char*const buff,int num){
	  auto end=buff;
	  if (num==0) {*buff='0';return buff+1;}
	  if (num<0) {*end++='-';num = -num;}
	  int l=10;
	  while(l<=num && l<1'000'000'000) l*=10;
	  if (l>num) l/=10;
	  while(l!=1){
	    int n = num/l;
	    *end++='0'+n;
	    num-=n*l;
	    l/=10;
	    }
	  *end++='0'+num;
	  return end;
	  }
	
	inline char* write_num(char*const buff,unsigned long num){
	  auto end=buff;
	  if (num==0) {*buff='0';return buff+1;}
	  long l=10;
	  while(l<=num && l<10'000'000'000'000'000'000ul) l*=10;
	  if (l>num) l/=10;
	  while(l!=0){
	    int n = num/l;
	    *end++='0'+n;
	    num-=n*l;
	    l/=10;
	    }
	  *end++='0'+num;
	  return end;
	  }
	
	inline char* write_num(char*const buff,unsigned num){
	  auto end=buff;
	  if (num==0) {*buff='0';return buff+1;}
	  int l=10;
	  while(l<=num && l<1'000'000'000) l*=10;
	  if (l>num) l/=10;
	  while(l!=0){
	    int n = num/l;
	    *end++='0'+n;
	    num-=n*l;
	    l/=10;
	    }
	  *end++='0'+num;
	  return end;
	  }
	inline auto read_num(char* arg){
	  struct ret_t {long var;int error;};
	  int val=0;
	  if (*arg == '\0') return ret_t{0,1};
	  bool neg=false;
	  if (*arg == '-') {
	    neg=true;
	    ++arg;
	    if (*arg == '\0') return ret_t{0,1};
	    }
	  do{
	    val*=10;
	    if (*arg >= '0' && *arg<='9'){
	      val+=*arg-'0';
	      }
	    else {
	      return ret_t{0,1};
	      }
	    }while(*++arg!='\0');
	  if (neg) return ret_t{-val,0};
	  return ret_t{val,0};
	  }

	//TODO rien à faire ici:
	inline long get_processor_timestamp(){
	  register long low asm("rax");
	  register long high asm("rdx");
	  asm volatile(
	    "rdtsc\n\t"
	    :"=r"(low),"=r"(high)
	    );
	  return (high<<32) | low;
	  }
	inline long get_processor_timestamp_with_fence(){
	  //see How to Benchmark Code execution Time on IA32 and IA64 instruction set architecture  
	  register long low asm("rax");
	  register long high asm("rdx");
	  asm volatile(
	    "lfence\n\t"//serializing instruction. Shall it be mfence on AMD - see S.O.???
	    "rdtsc\n\t"
	    :"=r"(low),"=r"(high)
	    :
	    :"memory"//prevent compiler reordering/optimization of memory accesses
	    );
	  return (high<<32) | low;
	  }
	}

