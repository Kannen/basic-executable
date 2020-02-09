/// @file
/// Provides tuple similar to standard library
#pragma once

#include <fcpp/language-support.hpp>
#include <fcpp/types.hpp>
#include <fcpp/utility.hpp>

#pragma GCC diagnostic push;
#pragma GCC diagnostic ignored "-Wmissing-braces"

namespace std{

#pragma GCC diagnostic push  
#pragma GCC diagnostic ignored "-Wpadded"
namespace _impl{
	template <class T, class...Args>
	struct tuple_elems
		{

		[[no_unique_address]] T first;
		
		[[no_unique_address]] tuple<Args...> others;

		};

	template <class T>
	struct tuple_elems<T>
		{

		[[no_unique_address]] T first;

		};
	}
	

template <class T, class...Args>
struct tuple <T, Args...>
	: _impl::tuple_elems <T, Args...>
	{
	
	static constexpr size_t size = sizeof...(Args)+1;
	
	friend constexpr auto 
	size (const tuple&) -> size_t
	    	{
	    	return sizeof...(Args)+1;
	    	}
	
	template<size_t I>
	friend constexpr decltype(auto)
	get [[gnu::always_inline]] (tuple& a)
	    {
	    static_assert(I>=0 && I<size,"Index out of bounds");
	    if constexpr (I==0)
	      return (a.first); 
	    else
	      return get <I-1> (a.others);
	    }
	
	template<size_t I>
	friend constexpr decltype(auto)
	get [[gnu::always_inline]] (const tuple& a)
	    {
	    static_assert(I>=0 && I<size,"Index out of bounds");
	    if constexpr (I==0)
	      return (a.first); 
	    else
	      return get <I-1> (a.others);
	    }
	
	template<size_t I>
	friend constexpr decltype(auto)
	get [[gnu::always_inline]] (tuple&& a)
	    {
	    static_assert(I>=0 && I<size,"Index out of bounds");
	    if constexpr (I==0)
	      return move(a).first; 
	    else
	      return get <I-1> (move(a).others);
	    }
	
	template<size_t I>
	friend constexpr decltype(auto)
	get [[gnu::always_inline]] (const tuple&& a)
	    {
	    static_assert(I>=0 && I<size, "Index out of bounds");
	    if constexpr (I==0)
	      return move(a).first; 
	    else
	      return get <I-1> (move(a).others);
	    }
	
	};


template<>
struct tuple<>
    {

    static constexpr auto
    size(){
      return 0;
      }

    };

template <class...Args>
tuple(Args...) -> tuple <Args...>;


template <class Arg0, class...Args>
constexpr auto
tuple_concat(Arg0&& arg0, Args&&...args);

constexpr auto 
tuple_concat(){
    return tuple<>{};
	}


namespace _impl
	{

	template <class T0, class...Ts>
	struct _tuple_concat
		{
		using type = _tuple_concat <T0, 
		                  typename _tuple_concat <Ts...> ::type> ::type;
		};

	template <class T1, class T2>
	struct _tuple_concat <T1, T2>
		{

		template <class, class>
		struct inds;

		template <size_t...Is, size_t...Js>
		struct inds <index_sequence<Is...>, index_sequence<Js...>>
			{
			using type = tuple <tuple_element_t <Is,T1> ..., tuple_element_t <Js,T2>...>;
			};

		using type = inds <make_index_sequence <tuple_size_v <T1>>
		                 , make_index_sequence <tuple_size_v <T2>>> ::type;

		};

	template <class T1>
	struct _tuple_concat <T1>
		{

		template <class>
		struct inds;

		template <size_t...Is>
		struct inds <index_sequence<Is...>>
			{
			using type = tuple <tuple_element_t <Is,T1> ...>;
			};

		using type = inds <make_index_sequence <tuple_size_v <T1>>> ::type;

		};


	template <size_t...Is, class Arg0, class...Args>
	constexpr auto
	_mtuple_concat(index_sequence<Is...>, Arg0&& arg0, Args&&...args){
		return typename _tuple_concat <remove_cvref_t<Arg0>, remove_cvref_t<Args>...> ::type
				{get<Is> (forward<Arg0>(arg0))..., tuple_concat (forward<Args>(args)...)};
		}
	
	}
	

template <class...Tuples>
using tuple_concat_t = _impl::_tuple_concat <Tuples...> ::type;

template <class Arg0, class...Args>
constexpr auto 
tuple_concat(Arg0&& arg0, Args&&...args){
	return _impl::_mtuple_concat (make_index_sequence <tuple_size_v <remove_cvref_t <Arg0>>>{}
                             ,forward <Arg0> (arg0)
							 ,forward <Args> (args)...);
	}


template<class...Args>
constexpr auto 
forward_as_tuple (Args&&...args)
    -> tuple <Args&&...>
    {
    return {forward<Args>(args)...};
    }


struct ignore_t
   {

   explicit ignore_t()=default;

   template<class T>
   constexpr const ignore_t& 
   operator= [[gnu::always_inline]] (const T&) const noexcept
       {
       return *this; 
       }

   };

inline constexpr ignore_t ignore;


template<class...Args>
class tie
    {

  private:

    tuple <Args&...>  tp_args;
    
  public:

    constexpr
    tie(Args&...args)
        :tp_args{args...}
        {}

    template<class T>
    constexpr auto 
    operator=(T&& v) 
        {
        assign(make_index_sequence <sizeof...(Args)>{}, forward<T>(v));
        return *this;
        }

  private:

    template<size_t...Is,class T>
    constexpr void 
    assign(index_sequence<Is...>,T&& v){
        (void (get <Is> (tp_args) = get <Is> (forward<T>(v))),...);
        }

    };


template <class...Args>
struct tuple_size <tuple <Args...>> : integral_constant <size_t,sizeof...(Args)> {};

template<size_t I,class A,class...Args>
struct tuple_element <I, tuple <A,Args...>> 
  {

  static_assert (I <= sizeof...(Args));

  using type = tuple_element <I-1, tuple<Args...>>::type;

  };

template <class A, class...Args>
struct tuple_element <0, tuple<A, Args...>> 
  {

  using type = A;

  };


}

#pragma GCC diagnostic pop
