#pragma once

#include <fcpp/visitor.hpp>
#include <fcpp/union_of.hpp>
#include <fcpp/tuple.hpp>
#include <fcpp/assertion.hpp>

namespace std
{

namespace _impl
	{

	template <class T, size_t I, class...Ts>
	struct type_find_
		: integral_constant<ssize_t,-1> 
		{
		using others = integral_constant<ssize_t,-1>;
		}; 

	template <class T, size_t I, class...Ts>
	struct type_find_ <T, I, T, Ts...> 
		: integral_constant <ssize_t, I>
		{
		using others = type_find_ <T, I, Ts...>;
		};

	template <class T, size_t I, class U, class...Ts>
	struct type_find_ <T, I, U, Ts...> 
	        :type_find_ <T, I+1, Ts...>
		{};

	template <class T,class...Ts>
	using type_find = type_find_ <T, 0, Ts...>;

	
	template <class Arg, class T, size_t I
		,bool Isb = is_same_v <remove_cv_t<T>, bool>
		,class=void>
	struct select_alternative_1 {
	        void construct_with();
		};

	template <class T>
	using _arr = T[];

	template <class Arg, class T, size_t I>
	struct select_alternative_1 <Arg, T, I, false, void_t<decltype(_arr<T>{declval<Arg>()})>>{
	        static integral_constant<size_t,I> construct_with(T);
		};

	template <class Arg, class T, size_t I>
	struct select_alternative_1 <Arg, T, I, true, enable_if_t <is_same_v <remove_cvref_t <Arg>, bool>>>{
	        static integral_constant<size_t,I> construct_with(T);
		};

	template <class Arg, class Ind, class...Ts>
	struct select_alternatives;
	
	template <class Arg, size_t...Is, class...Ts>
	struct select_alternatives <Arg, index_sequence <Is...>, Ts...>
		:select_alternative_1 <Arg, Ts, Is>...{
		using select_alternative_1<Arg, Ts, Is> :: construct_with...;
		};

	template <class Arg, class...Ts>
	static constexpr size_t
	select_alternative = decltype(select_alternatives <Arg, index_sequence_for<Ts...>, Ts...> 
	                      :: construct_with(declval<Arg>()))::value;
	}



template <class...Ts>
struct variant
	{

	private:

	using index_type = conditional_t < (sizeof...(Ts) <= numeric_limits <uint8_t> :: max() + 1)
                                          ,uint8_t
					  ,uint16_t>;	

	using union_type = union_of <Ts...>;

	union_of <Ts...> value;

	index_type index_;

	template<class T>
	struct checked_index_for_alternative{
	        using found = _impl::type_find <T, Ts...>;
		static_assert(found::value == -1
		             ,"Variant does not hold this alternative type");
		static_assert(found::value != -1 && found::others::value != -1
		             ,"Variant does hold multiple alternatives of this type");
		static constexpr size_t value = found::value;
		};


	public:

	static constexpr size_t 
	size = sizeof...(Ts);

	template <class T>
	static constexpr size_t 
	index_for_alternative = checked_index_for_alternative <T>::value; 

	template <size_t I>
	using alternative_type = tuple_element_t <I, tuple<Ts...>>;


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
	get (const variant& self)
		{
		Assert(I == index (self));
		return get <I> (self.value);
		}
	template <class T>
	friend constexpr decltype(auto)
	get (const variant& self)
		{
		return get <index_for_alternative <T>> (self);
		}

	template <size_t I>
	friend constexpr decltype(auto)
	get (variant& self)
		{
		Assert(I == index (self));
		return get <I> (self.value);
		}
	template <class T>
	friend constexpr decltype(auto)
	get (variant& self)
		{
		return get <index_for_alternative <T>> (self);
		}

	template <size_t I>
	friend constexpr decltype(auto)
	get (variant&& self)
		{
		Assert(I == index (self));
		return get <I> (move (self.value));
		}
	template <class T>
	friend constexpr decltype(auto)
	get (variant&& self)
		{
		return get <index_for_alternative <T>> (move(self));
		}

	template <size_t I>
	friend constexpr decltype(auto)
	get (const variant&& self)
		{
		Assert(I == index(self));
		return get <I> (move (self.value));
		}
	template <class T>
	friend constexpr decltype(auto)
	get (const variant&& self)
		{
		return get <index_for_alternative <T>> (move(self));
		}


	explicit
	variant() =default;
	
	template <size_t I, class...Args>
	explicit
	variant(in_place_index_t<I>, Args&&...args)
		{
		construct_value <I> (forward <Args> (args)...);
		}

	template <class T, class...Args>
	explicit 
	variant (in_place_type_t<T>, Args&&...args)
		: variant (in_place_index <index_for_alternative <T>>, forward<Args>(args)...)
		{}

	template <class Arg>
		requires (!is_base_of_v <variant, remove_reference_t<Arg>>)
	variant (Arg&& arg)
		: variant (in_place_index <_impl::select_alternative <Arg&&, Ts...>>, forward<Arg>(arg))
		{}

	variant (const variant& other) 
		requires (!is_trivially_copy_constructible_v <union_type>)
		{
		copy (other);
		}
	variant (const variant& other) 
		requires (is_trivially_copy_constructible_v <union_type>)
		=default;

	variant (variant&& other)
		requires (!is_trivially_move_constructible_v <union_type>)
		{
		copy (move (other));
		}
	variant (variant&& other)
		requires (is_trivially_move_constructible_v <union_type>)
		=default;

	auto
	operator= (const variant& other)
		-> variant&
		requires (!is_trivially_copy_assignable_v <union_type>)
		{
		destroy_value();
		copy (other);
		}
	auto
	operator= (const variant& other)
		-> variant&
		requires (is_trivially_copy_assignable_v <union_type>)
		=default;

	auto
	operator= (variant&& other)
		-> variant&
		requires (!is_trivially_move_assignable_v <union_type>)
		{
		destroy_value();
		copy (move (other));
		}
	auto
	operator= (variant&& other)
		-> variant&
		requires (is_trivially_move_assignable_v <union_type>)
		=default;

	template <size_t I, class...Args>
	emplace(Args&&...args)
		{
		destroy_value();
		construct_value <I> (forward <Args> (args)...);
		}
	
	~variant()
		requires (!is_trivially_destructible_v <union_type>)
		{
		destroy_value();
		}
	~variant()
		requires (is_trivially_destructible_v <union_type>)
		=default;
	
	private:

    	template <size_t I, class...Args>
	void
	construct_value(Args&&...args)
		{
		index_ = I;
		new (& get <I> (value)) auto(construct <alternative_type <I>> (forward <Args> (args) ...));
		}

	void 
	destroy_value(){
		if constexpr (!is_trivially_destructible_v <union_type>)
			visit ([] <class T> (T& value) { value.~T(); }
				,*this);
		}

	void 
	copy (const variant& other)
		{
		index_ = other.index_;
		if constexpr (is_trivially_copy_constructible_v <union_type>)
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
		index_ = other.index_;
		if constexpr (is_trivially_move_constructible_v <union_type>)
			new (&value) auto (move (other.value));
		else
			visit ( [] (auto& self_v, auto& other_v){
				new (&self_v) auto(move(other_v));
				}
			,*this, other);
		}

	};


}

