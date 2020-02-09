#pragma once

#include <fcpp/language-support.hpp>
#include <fcpp/types.hpp>
#include <fcpp/assertion.hpp>


namespace std{

/// simple array version
template<class T, size_t N>
struct array
	{
	
	T data[N];
	
	using value_type = T;

	static constexpr ssize_t size = N;

	
	friend constexpr auto 
	size(const array&) noexcept
	    {
	    return static_cast<ssize_t>(N);
	    }
	
	friend constexpr auto
	begin(const array& x){
	    return x.data;
	    }
	
	friend constexpr auto
	begin(array& x){
	    return x.data;
	    }
	
	friend constexpr auto
	end(const array& x){
	    return x.data+N;
	    }
	
	friend constexpr auto
	end(array& x){
	    return x.data+N;
	    }

	
	constexpr T&
	operator[] [[gnu::always_inline]] (ssize_t n) noexcept{
	    Assert (static_cast <size_t> (n) < N && n >= 0);
	    return data [n];
	    }
	
	constexpr const T&
	operator[] [[gnu::always_inline]] (ssize_t n) const noexcept{
	    Assert (static_cast <size_t> (n) < N && n >= 0);
	    return data [n];
	    }
	
	template <size_t I>
	friend constexpr auto
	get (array& a)
		-> T&
		{
		return a[I];
		}

	template <size_t I>
	friend constexpr auto
	get (const array& a)
		-> const T&
		{
		return a[I];
		}

	template <size_t I>
	friend constexpr auto
	get (array&& a)
		-> T&&
		{
		return move(a[I]);
		}

	template <size_t I>
	friend constexpr auto
	get (const array&& a)
		-> const T&&
		{
		return move(a[I]);
		}
	
	};

template <class T, class...Ts>
array (T,Ts...) -> array <T,sizeof...(Ts)+1>;

template <class T, size_t N>
struct tuple_size <array <T,N>> : integral_constant<size_t,N>{};

template <class T, size_t N, size_t I>
struct tuple_element <I, array<T,N>> : type_identity<T> {};



/// @}


}

