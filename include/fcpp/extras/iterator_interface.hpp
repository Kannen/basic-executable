#pragma once
#include <iterator>

// iterators needed typenames
//   - difference_type : either specialise incrementable_traits
//                       or deduced as It::difference_type
//                       or as decltype(a_It-a_It)
//   - element_type    : either specialize indirectly_readable_traits
//                       or It::value_type
//                       or It::element_type (not both)
//
//    -iter_value_t    : either for indirectly_readable_trait or iterator_trait if specialized
//    -iter_different_t: either for incrementable_trait or iterator_trait if specialized
//    iterator_traits::iteratog_category
//    iterator_traits::pointer  : decltype(a.operator->()) or void if no operator->
//    iterator_traits::reference : decltype(*a)
//    
//    for output_iterators:
//      iterator_traits::value_type
//      iterator_traits::difference_type
//      iterator_traits::reference
//
//    forward iterator iter_referenc_t must be an lvalue_reference 
//
//
//    for iterator_trait not to be confused
//    It::iterator_category
//    It::value_type
//    It::difference_type
//    It::pointer
//    It::reference
//
//    but for pointer trait not to be confused:
//    It::element_type
//
//    So better It defines element_type
//    then pointer_traits will define value_type thanks to indirectly_readable_traits
//
//    iterator_category has almost no meaning any more
//    iterators concept are different that cpp17 iterator requirement
//    most notably forward iterator does not have to refers to a lvalue reference type and no need to be default constructible


namespace std
{

template <class Self, class DifferenceType = std::ptrdiff_t>
class random_access_iterator_interface
	{
	constexpr auto
	self() -> Self&
		{
		return static_cast <Self&> (*this);
		}

	constexpr auto
	self() const -> const Self&
		{
		return static_cast <const Self&> (*this);
		}

	public:

	using difference_type = std::ptrdiff_t;

	constexpr auto
	operator -> () const
		{
		return & *self();
		}
		
	constexpr auto	
	operator ++ (int)
		{
		auto old = self();
		++ self ();
		return old;
		}

	constexpr auto	
	operator -- (int)
		{
		auto old = self();
		-- self ();
		return old;
		}

	friend constexpr auto	
	operator + (Self self, difference_type i)
		{
		self += i;
		return self;
		}

	friend constexpr auto	
	operator + (difference_type i, Self self)
		{
		self += i;
		return self;
		}

	friend constexpr auto	
	operator - (Self self, difference_type i)
		{
		self -= i;
		return self;
		}

	constexpr decltype(auto)
	operator [] (difference_type i) const
		{
		auto other = self() + i;
		return *other;
		}

        auto
	operator == (const random_access_iterator_interface&) const
		-> bool = default;

        auto
	operator <=> (const random_access_iterator_interface&) const
		-> std::strong_ordering = default;

	};


template <class Self, class PointerType, class ConstIterator = Self>
class pointer_wrapper_iterator_interface
	:public random_access_iterator_interface 
		<Self, iter_difference_t <PointerType>> 
	{

	using base = random_access_iterator_interface <Self>;

	constexpr auto
	self() -> Self&
		{
		return static_cast <Self&> (*this);
		}

	constexpr auto
	self() const -> const Self&
		{
		return static_cast <const Self&> (*this);
		}

	public:

	PointerType value;

	using iterator_category = contiguous_iterator_tag;
	using element_type	= iter_value_t <PointerType>;
	using difference_type	= iter_difference_t <PointerType>;
	using reference		= iter_reference_t <PointerType>;
	using pointer           = PointerType;

	pointer_wrapper_iterator_interface () 
		= default;

	constexpr
	pointer_wrapper_iterator_interface (PointerType value)
		:value {value}
		{}

	constexpr auto
	operator * () const
		-> reference
		{
		return *value;
		}

	constexpr auto
	operator ++ () 
		-> Self &
		{
		++value;
		return self();
		}

	using base::operator ++;

	constexpr auto
	operator -- () 
		-> Self &
		{
		--value;
		return self();
		}

	using base::operator --;

	constexpr auto
	operator += (difference_type i) 
		-> Self & 
		{
		value += i;
		return self();
		}

	constexpr auto
	operator -= (difference_type i) 
		-> Self &
		{
		value -= i;
		return self();
		}

	friend constexpr auto
	operator - (pointer_wrapper_iterator_interface a
	           ,pointer_wrapper_iterator_interface b) 
		-> difference_type
		{
		return a.value - b.value;
		}

	constexpr
	operator ConstIterator () const
		{
		return ConstIterator {value};
		}

	explicit constexpr
	operator bool() const
		{
		return value;
		}

        constexpr auto
	operator == (const pointer_wrapper_iterator_interface & other) const 
		{
		return value == other.value;
		}

        constexpr auto
	operator <=> (const pointer_wrapper_iterator_interface & other) const
		-> std::strong_ordering
		{
		return value <=> other.value;
		}

	};

}





