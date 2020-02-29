#pragma once

#include <fcpp/type_traits.hpp>
#include <fcpp/concepts.hpp>
#include <fcpp/assertion.hpp>
#include <utility>
#include <experimental/source_location>

namespace std{

template <class Derived, class From = Derived>
struct self_interface
	{
	protected:

	[[gnu::always_inline]] const Derived &
	self() const &
		{
		static_assert (std::is_base_of_v <self_interface, Derived>);
		static_assert (std::is_base_of_v <self_interface, From>);
		static_assert (std::is_base_of_v <From, Derived>);
		return static_cast <const Derived&> (*this);
		}

	[[gnu::always_inline]] Derived &
	self() &
		{
		static_assert (std::is_base_of_v <self_interface, Derived>);
		static_assert (std::is_base_of_v <self_interface, From>);
		static_assert (std::is_base_of_v <From, Derived>);
		return static_cast <Derived &> (*this);
		}
	[[gnu::always_inline]] Derived &&
	self() && 
		{
		static_assert (std::is_base_of_v <self_interface, Derived>);
		static_assert (std::is_base_of_v <self_interface, From>);
		static_assert (std::is_base_of_v <From, Derived>);
		return static_cast <Derived &&> (*this);
		}

	[[gnu::always_inline]] const Derived &&
	self() const && 
		{
		static_assert (std::is_base_of_v <self_interface, Derived>);
		static_assert (std::is_base_of_v <self_interface, From>);
		static_assert (std::is_base_of_v <From, Derived>);
		return static_cast <const Derived &&> (*this);
		}

	auto
	operator == (const self_interface &) const
		-> bool  = default;

	auto
	operator <=> (const self_interface &) const
		= default;

	};

/// construct an object and avoid pitfoil of T(expr) invoking an
/// explicit type convertion which may cause a reinterpret cast 
/// followed by a const cast see [expr.type.cast]
/// use list initialization for aggregates 
/// and direct none list initialization for other types
template<class T,class...Args>
[[gnu::always_inline]] constexpr inline auto 
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
	else //there must be some brace elisium
		return T{forward<Args>(args)...};
	}


template<size_t N, integral T>
inline constexpr auto
bitfield_clamp [[gnu::always_inline]]
	(T v) noexcept
	{
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wconversion"
	#pragma GCC diagnostic ignored "-Wpadded"
	struct {
		T value :N;
		} clamped{v};
	#pragma GCC diagnostic pop
	return clamped.value;
	}


/// static_cast between enumeration and arithmetic types
/// while ensuring convertion does not change
/// the value. (But for unscoped enum can not check the value
/// is out of bound appart underlying types bounds)
template <class T, size_t Nbits = sizeof(T) * numeric_limits<unsigned char>::digits, class U>
   requires    (is_arithmetic_v <T> || is_enum_v <T>) && (is_arithmetic_v <U> || is_enum_v <U>)
            && (Nbits <= sizeof (T) * numeric_limits <unsigned char>::digits)
[[gnu::always_inline]] inline constexpr auto 
narrow_cast (U u) noexcept
	-> T
	{ 
	constexpr auto T_Nbits = sizeof (T) * numeric_limits <unsigned char>::digits;
	static_assert (is_integral_v<T> || Nbits == T_Nbits);
	
	if constexpr (is_enum_v <U>)
		return narrow_cast<T>(static_cast<underlying_type_t<U>>(u));
	
	else if constexpr (is_enum_v<T>)
		return static_cast<T> (narrow_cast <underlying_type_t <T>> (u));
	      
	else {	
		T nv{};
		if constexpr (Nbits != T_Nbits)
			nv = bitfield_clamp <Nbits> (static_cast <T> (u));
		else
			nv = static_cast <T> (u);
	
		Assert (static_cast <U> (nv) == u);
	
		if constexpr (numeric_limits <T>::is_signed != numeric_limits<U>::is_signed){
			if constexpr (numeric_limits <U>::is_signed)
				Assert (u >= U{});
			else
				Assert (nv >= T{});
			}

		return nv;
		}
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

}

