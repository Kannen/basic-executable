#pragma once

#include <visitor.hpp>
#include <union_of.hpp>
#include <tuple.hpp>
#include <assertion.hpp>

namespace std
{

template <class...Ts>
struct variant
	{

	private:

	using index_type = conditionnal_t < (sizeof...(Ts) <= numeric_limits <uint8_t> :: max() + 1)
                                          ,uint8_t
					  ,uint16_t>;	

	using union_type = union_of <Ts...>;

	union_of <Ts...> value;

	index_type index;


	public:

	static constexpr size_t size = sizeof...(Args);

	template <size_t I>
	using value_type = tuple_element_t <I, tuple<Ts...>>;


	friend constexpr auto 
	size (const variant&)
		-> ssize_t
		{ return variant::size; }

	friend constexpr auto 
	index (const variant& v)
		-> ssize_t
		{ return v.index_; }

	
	template <size_t I>
	friend constexpr decltype(auto)
	get (const variant&)
		{
		Assert(I == index());
		return get <I> (value);
		}

	template <size_t I>
	friend constexpr decltype(auto)
	get (variant&)
		{
		Assert(I == index());
		return get <I> (value);
		}

	template <size_t I>
	friend constexpr decltype(auto)
	get (variant&&)
		{
		Assert(I == index());
		return get <I> (move (value));
		}

	template <size_t I>
	friend constexpr decltype(auto)
	get (const variant&&)
		{
		Assert(I == index());
		return get <I> (move (value));
		}


	variant() =default;
	
	template <size_t I, class...Args>
	variant(in_place_index<I>, Args&&...args)
		{
		construct_value <I> (forward <Args> (args)...);
		}

	variant (const variant& other) 
		requires (!is_trivialy_copy_constructible_v <union_type>)
		{
		copy (other);
		}

	variant (variant&& other)
		requires (!is_trivialy_move_constructible_v <union_type>)
		{
		copy (move (other));
		}

	auto
	operator= (const variant& other)
		-> variant&
		requires (!is_trivialy_copy_assignable_v <union_type>)
		{
		destroy_value();
		copy (other);
		}

	auto
	operator= (variant&& other)
		-> variant&
		requires (!is_trivialy_move_assignable_v <union_type>)
		{
		destroy_value();
		copy (move (other));
		}

	template <size_t I, class...Args>
	emplace(Args&&...args)
		{
		destroy_value();
		construct_value <I> (forward <Args> (args)...);
		}
	
	~variant()
		requires (!is_trivialy_desctructible <union_type>)
		{
		destroy_value();
		}
	
	private:

    	template <size_t I, class...Args>
	void
	construct_value(Args&&...args)
		{
		index = I;
		new (& get <I> (value)) auto(construct <value_type<I>> (forward <Args> (args) ...));
		}

	void 
	destroy_value(){
		if constexpr (!is_trivialy_destructible <union_type>)
			visit ([] <class T> (T& value) { value.~T(); }
				,*this)
				}

	void 
	copy (const variant& other)
		{
		index = other.index;
		if constexpr (is_trivialy_copy_constructible <union_type>)
			new (&value) auto (other.value);
		else
			visit ( [] (auto& self_v, const auto& other_v){
					new (&self_v) auto(other_v);
					}
				,*this, other);
		}

	void 
	copy (variant&& other)
		{
		index = other.index;
		if constexpr (is_trivialy_move_constructible <union_type>)
			new (&value) auto (move (other.value));
		else
			visit ( [] (auto& self_v, auto& other_v){
				new (&self_v) auto(move(other_v));
				}
			,*this, other);
		}

	};


}

