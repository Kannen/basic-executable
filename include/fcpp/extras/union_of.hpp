#pragma once

#include <fcpp/types.hpp>
#include <fcpp/type_traits.hpp>


namespace std{

template <class...Types>
class union_of; 

template <>
class union_of <>{};

template <class T, class...Types>
class union_of <T, Types...>
	{

	private:

	union{
		T first;
		union_of <Types...> others;
		};


	public:

	union_of() =default;
	
	template<class...Args>
	constexpr
	union_of(in_place_index_t <0>, Args&&...args)
		: first (construct <T> (forward <Args> (args)...)) {}
	
	template<size_t I,class...Args>
	constexpr
	union_of(in_place_index_t <I>, Args&&...args)
		:others (in_place_index<I-1>, forward<Args>(args)...){}
	

	union_of (const union_of&) =default;

	union_of (union_of&&) =default;

	union_of&
	operator=(const union_of&) =default;

	union_of&
	operator=(union_of&&) =default;


	~union_of() requires ( (! is_trivially_destructible_v <T> || ... || ! is_trivially_destructible_v <Types>) )
		{}

	~union_of() requires ( (is_trivially_destructible_v <T> && ... && is_trivially_destructible_v <Types>) )
		=default;	
	

	template <size_t I>
	friend constexpr decltype(auto) 
	get (union_of& self) {
		if constexpr (I==0)
			return (self.first);
		else
			return get<I-1>(self.others);
		}

	template <size_t I>
	friend constexpr decltype(auto) 
	get (const union_of& self) {
		if constexpr (I==0)
			return (self.first);
		else
			return get<I-1>(self.others);
		}

	template <size_t I>
	friend constexpr decltype(auto) 
	get (union_of&& self) {
		if constexpr(I==0)
		  return move(self).first;
		else
		  return get<I-1>(move(self).others);
		}

	template <size_t I>
	friend constexpr decltype(auto) 
	get(const union_of&& self) noexcept{
		if constexpr(I==0)
			return move(self).first;
		else
			return get<I-1>(move(self).others);
		}


	};

}
