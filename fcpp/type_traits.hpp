#pragma once

#include <fcpp/types.hpp>
#include <type_traits>
#include <utility>
#include <experimental/source_location>

#undef __GLIBC__


namespace std
	{

	
    	/// Apply to type `To` the ref qualifier of `From`
	template<class From,class To>
	using propagate_ref_t = conditional_t<is_rvalue_reference_v<From>,To&&
	                     ,conditional_t<is_lvalue_reference_v<From>,To&
	                     ,To>>;
	/// Apply to type `To` the cv qualifier of `From`
	template<class From,class To>
	using propagate_cv_t =  conditional_t<is_const_v<From> && is_volatile_v<From>,const volatile To
	                     ,conditional_t<is_const_v<From>, const To
	                     ,conditional_t<is_volatile_v<From>, volatile To
	                     ,To>>>;
	/// Apply to type `To` the cv-ref qualifiers of `From`
	template<class From,class To>
	using propagate_cvref_t = propagate_ref_t<From,propagate_cv_t<remove_reference_t<From>,To>>;


	template<class T,class...Args>
	struct is_brace_constructible {

		template<class U=T,class=void>
		struct comp
			:false_type
			{
			static constexpr bool noex = false;
			};

		template<class U>
		struct comp<U,void_t<decltype(U{declval<Args>()...})>>
		  	:true_type
		  	{
		  	static constexpr bool noex = noexcept(U{declval<Args>()...});
		  	};
		static constexpr bool value = comp<>::value;
		static constexpr bool noex = comp<>::noex;
		};

	template<class T,class...Args>
	struct is_nothrow_brace_constructible{
		static constexpr bool value = is_brace_constructible <T, Args...> :: value;
		};

	template<class T,class...Args>
	static constexpr inline bool 
	is_brace_constructible_v = is_brace_constructible <T, Args...> :: value;

	template<class T,class...Args>
	static constexpr inline bool 
	is_nothrow_brace_constructible_v = is_nothrow_brace_constructible <T, Args...> :: value;

	template<class T,class...Args>
	struct is_construct_able{
		static constexpr bool 
		value = is_constructible_v<T,Args...> || is_aggregate_v<T> && is_brace_constructible_v<T,Args...>;
		};

	template<class T,class...Args>
	struct is_nothrow_construct_able{
		static constexpr bool 
		value = is_nothrow_constructible_v<T,Args...> || is_aggregate_v<T> && is_nothrow_brace_constructible_v<T,Args...>;
		};
	
	template<class T,class...Args>
	static inline constexpr bool 
	is_construct_able_v = is_construct_able<T, Args...> :: value;

	template<class T,class...Args>
	static inline constexpr bool is_nothrow_construct_able_v = is_nothrow_construct_able <T, Args...> :: value;
	  

	template <class T>
	struct passed_by_register
		: bool_constant< !is_reference_v<T> && is_trivially_copyable_v<remove_cv_t<T>> && sizeof(T) <= 2 * sizeof(void*)>
		{};

	template <class T>
	inline constexpr bool passed_by_register_v = passed_by_register <T> :: value;

	template<class T>
	struct add_noexcept{
	  using type = T;
	  };
	template<class R,class...Args>
	struct add_noexcept<R(Args...) >{
	  using type = R(Args...) noexcept;
	  };
	template<class R,class...Args>
	struct add_noexcept<R(Args...)& >{
	  using type = R(Args...)& noexcept;
	  };
	template<class R,class...Args>
	struct add_noexcept<R(Args...)&& >{
	  using type = R(Args...)&& noexcept;
	  };
	template<class R,class...Args>
	struct add_noexcept<R(Args...) const >{
	  using type = R(Args...)const noexcept;
	  };
	template<class R,class...Args>
	struct add_noexcept<R(Args...)const& >{
	  using type = R(Args...)const& noexcept;
	  };
	template<class R,class...Args>
	struct add_noexcept<R(Args...)const&& >{
	  using type = R(Args...)const&& noexcept;
	  };
	template<class R,class...Args>
	struct add_noexcept<R(Args...) const volatile >{
	  using type = R(Args...)const volatile noexcept;
	  };
	template<class R,class...Args>
	struct add_noexcept<R(Args...)const volatile& >{
	  using type = R(Args...)const& noexcept;
	  };
	template<class R,class...Args>
	struct add_noexcept<R(Args...)const volatile&& >{
	  using type = R(Args...)const volatile&& noexcept;
	  };
	///add noexcept specifier for function type (not for member function pointer type)
	template<class T>
	using add_noexcept_t = typename add_noexcept<T>::type;
	
	template<class T>
	struct remove_noexcept{
	  using type = T;
	  };
	template<class R,class...Args>
	struct remove_noexcept<R(Args...) noexcept>{
	  using type = R(Args...);
	  };
	template<class R,class...Args>
	struct remove_noexcept<R(Args...)& noexcept>{
	  using type = R(Args...)&;
	  };
	template<class R,class...Args>
	struct remove_noexcept<R(Args...)&& noexcept>{
	  using type = R(Args...)&&;
	  };
	template<class R,class...Args>
	struct remove_noexcept<R(Args...) const noexcept>{
	  using type = R(Args...)const;
	  };
	template<class R,class...Args>
	struct remove_noexcept<R(Args...)const& noexcept>{
	  using type = R(Args...)const&;
	  };
	template<class R,class...Args>
	struct remove_noexcept<R(Args...)const&& noexcept>{
	  using type = R(Args...)const&&;
	  };
	template<class R,class...Args>
	struct remove_noexcept<R(Args...) const volatile noexcept>{
	  using type = R(Args...)const volatile;
	  };
	template<class R,class...Args>
	struct remove_noexcept<R(Args...)const volatile& noexcept>{
	  using type = R(Args...)const&;
	  };
	template<class R,class...Args>
	struct remove_noexcept<R(Args...)const volatile&& noexcept>{
	  using type = R(Args...)const volatile&&;
	  };
	///remove noexcept specifier if it is a function type, otherwise T
	///not for member function pointer type
	template<class T>
	using remove_noexcept_t = typename remove_noexcept<T>::type;
}

namespace std::_impl
	{
	template<class T,class X,class=void,class=void>
	struct has_conv_op:false_type{};
	template<class T,class X>
	struct has_conv_op<T,X
	  ,enable_if_t<is_same_v<remove_cvref_t<T>,T>>
	  ,void_t<decltype(static_cast<X(remove_cvref_t<T>::*)()>(&remove_cvref_t<T>::operator X))>>
	  :true_type{};
	template<class T,class X>
	struct has_conv_op<T,X
	  ,enable_if_t<is_same_v<const remove_cvref_t<T>,T>>
	  ,void_t<decltype(static_cast<X(remove_cvref_t<T>::*)()const>(&remove_cvref_t<T>::operator X))>>
	  :true_type{};
	template<class T,class X>
	struct has_conv_op<T,X
	  ,enable_if_t<is_same_v<volatile remove_cvref_t<T>,T>>
	  ,void_t<decltype(static_cast<X(remove_cvref_t<T>::*)()volatile>(&remove_cvref_t<T>::operator X))>>
	  :true_type{};
	template<class T,class X>
	struct has_conv_op<T,X
	  ,enable_if_t<is_same_v<const volatile remove_cvref_t<T>,T>>
	  ,void_t<decltype(static_cast<X(remove_cvref_t<T>::*)()const volatile>(&remove_cvref_t<T>::operator X))>>
	  :true_type{};
	template<class T,class X>
	struct has_conv_op<T,X
	  ,enable_if_t<is_same_v<remove_cvref_t<T>&,T>>
	  ,void_t<decltype(static_cast<X(remove_cvref_t<T>::*)()&>(&remove_cvref_t<T>::operator X))>>
	  :true_type{};
	template<class T,class X>
	struct has_conv_op<T,X
	  ,enable_if_t<is_same_v<const remove_cvref_t<T>&,T>>
	  ,void_t<decltype(static_cast<X(remove_cvref_t<T>::*)()const&>(&remove_cvref_t<T>::operator X))>>
	  :true_type{};
	template<class T,class X>
	struct has_conv_op<T,X
	  ,enable_if_t<is_same_v<volatile remove_cvref_t<T>&,T>>
	  ,void_t<decltype(static_cast<X(remove_cvref_t<T>::*)()volatile&>(&remove_cvref_t<T>::operator X))>>
	  :true_type{};
	template<class T,class X>
	struct has_conv_op<T,X
	  ,enable_if_t<is_same_v<const volatile remove_cvref_t<T>&,T>>
	  ,void_t<decltype(static_cast<X(remove_cvref_t<T>::*)()const volatile&>(&remove_cvref_t<T>::operator X))>>
	  :true_type{};
	template<class T,class X>
	struct has_conv_op<T,X
	  ,enable_if_t<is_same_v<remove_cvref_t<T>&&,T>>
	  ,void_t<decltype(static_cast<X(remove_cvref_t<T>::*)()&&>(&remove_cvref_t<T>::operator X))>>
	  :true_type{};
	template<class T,class X>
	struct has_conv_op<T,X
	  ,enable_if_t<is_same_v<const remove_cvref_t<T>&&,T>>
	  ,void_t<decltype(static_cast<X(remove_cvref_t<T>::*)()const&&>(&remove_cvref_t<T>::operator X))>>
	  :true_type{};
	template<class T,class X>
	struct has_conv_op<T,X
	  ,enable_if_t<is_same_v<volatile remove_cvref_t<T>&&,T>>
	  ,void_t<decltype(static_cast<X(remove_cvref_t<T>::*)()volatile&&>(&remove_cvref_t<T>::operator X))>>
	  :true_type{};
	template<class T,class X>
	struct has_conv_op<T,X
	  ,enable_if_t<is_same_v<const volatile remove_cvref_t<T>&&,T>>
	  ,void_t<decltype(static_cast<X(remove_cvref_t<T>::*)()const volatile&&>(&remove_cvref_t<T>::operator X))>>
	  :true_type{};
	template<class X>
	struct check_conv_op_nref{
	  template<class T,class=enable_if_t<has_conv_op<X,T&>::value>>
	  operator T&();
	  template<class T,class=enable_if_t<has_conv_op<X,T&&>::value>>
	  operator T&&();
	  template<class T,class=enable_if_t<has_conv_op<X,const T&>::value>>
	  operator const T&();
	  template<class T,class=enable_if_t<has_conv_op<X,const T&&>::value>>
	  operator const T&&();
	  template<class T,class=enable_if_t<has_conv_op<X,const volatile T&>::value>>
	  operator const volatile T&();
	  template<class T,class=enable_if_t<has_conv_op<X,const volatile T&&>::value>>
	  operator const volatile T&&();
	  template<class T,class=enable_if_t<has_conv_op<const X,T&>::value>>
	  operator T&()const;
	  template<class T,class=enable_if_t<has_conv_op<const X,T&&>::value>>
	  operator T&&()const;
	  template<class T,class=enable_if_t<has_conv_op<const X,const T&>::value>>
	  operator const T&()const;
	  template<class T,class=enable_if_t<has_conv_op<const X,const T&&>::value>>
	  operator const T&&()const;
	  template<class T,class=enable_if_t<has_conv_op<const X,const volatile T&>::value>>
	  operator const volatile T&()const;
	  template<class T,class=enable_if_t<has_conv_op<const X,const volatile T&&>::value>>
	  operator const volatile T&&()const;
	  template<class T,class=enable_if_t<has_conv_op<volatile X,T&>::value>>
	  operator T&()volatile;
	  template<class T,class=enable_if_t<has_conv_op<volatile X,T&&>::value>>
	  operator T&&()volatile;
	  template<class T,class=enable_if_t<has_conv_op<volatile X,const T&>::value>>
	  operator const T&()volatile;
	  template<class T,class=enable_if_t<has_conv_op<volatile X,const T&&>::value>>
	  operator const T&&()volatile;
	  template<class T,class=enable_if_t<has_conv_op<volatile X,const volatile T&>::value>>
	  operator const volatile T&()volatile;
	  template<class T,class=enable_if_t<has_conv_op<volatile X,const volatile T&&>::value>>
	  operator const volatile T&&()volatile;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X,T&>::value>>
	  operator T&() const volatile;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X,T&&>::value>>
	  operator T&&()const volatile;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X,const T&>::value>>
	  operator const T&()const volatile;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X,const T&&>::value>>
	  operator const T&&()const volatile;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X,const volatile T&>::value>>
	  operator const volatile T&()const volatile;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X,const volatile T&&>::value>>
	  operator const volatile T&&()const volatile;
	  };
	template<class X>
	struct check_conv_op_ref{
	  template<class T,class=enable_if_t<has_conv_op<X&,T&>::value>>
	  operator T&()&;
	  template<class T,class=enable_if_t<has_conv_op<X&,T&&>::value>>
	  operator T&&()&;
	  template<class T,class=enable_if_t<has_conv_op<X&,const T&>::value>>
	  operator const T&()&;
	  template<class T,class=enable_if_t<has_conv_op<X&,const T&&>::value>>
	  operator const T&&()&;
	  template<class T,class=enable_if_t<has_conv_op<X&,const volatile T&>::value>>
	  operator const volatile T&()&;
	  template<class T,class=enable_if_t<has_conv_op<X&,const volatile T&&>::value>>
	  operator const volatile T&&()&;
	  template<class T,class=enable_if_t<has_conv_op<const X&,T&>::value>>
	  operator T&()const&;
	  template<class T,class=enable_if_t<has_conv_op<const X&,T&&>::value>>
	  operator T&&()const&;
	  template<class T,class=enable_if_t<has_conv_op<const X&,const T&>::value>>
	  operator const T&()const&;
	  template<class T,class=enable_if_t<has_conv_op<const X&,const T&&>::value>>
	  operator const T&&()const&;
	  template<class T,class=enable_if_t<has_conv_op<const X&,const volatile T&>::value>>
	  operator const volatile T&()const&;
	  template<class T,class=enable_if_t<has_conv_op<const X&,const volatile T&&>::value>>
	  operator const volatile T&&()const&;
	  template<class T,class=enable_if_t<has_conv_op<volatile X&,T&>::value>>
	  operator T&()volatile&;
	  template<class T,class=enable_if_t<has_conv_op<volatile X&,T&&>::value>>
	  operator T&&()volatile&;
	  template<class T,class=enable_if_t<has_conv_op<volatile X&,const T&>::value>>
	  operator const T&()volatile&;
	  template<class T,class=enable_if_t<has_conv_op<volatile X&,const T&&>::value>>
	  operator const T&&()volatile&;
	  template<class T,class=enable_if_t<has_conv_op<volatile X&,const volatile T&>::value>>
	  operator const volatile T&()volatile&;
	  template<class T,class=enable_if_t<has_conv_op<volatile X&,const volatile T&&>::value>>
	  operator const volatile T&&()volatile&;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X&,T&>::value>>
	  operator T&() const volatile&;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X&,T&&>::value>>
	  operator T&&()const volatile&;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X&,const T&>::value>>
	  operator const T&()const volatile&;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X&,const T&&>::value>>
	  operator const T&&()const volatile&;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X&,const volatile T&>::value>>
	  operator const volatile T&()const volatile&;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X&,const volatile T&&>::value>>
	  operator const volatile T&&()const volatile&;
	  template<class T,class=enable_if_t<has_conv_op<X&&,T&>::value>>
	  operator T&()&&;
	  template<class T,class=enable_if_t<has_conv_op<X&&,T&&>::value>>
	  operator T&&()&&;
	  template<class T,class=enable_if_t<has_conv_op<X&&,const T&>::value>>
	  operator const T&()&&;
	  template<class T,class=enable_if_t<has_conv_op<X&&,const T&&>::value>>
	  operator const T&&()&&;
	  template<class T,class=enable_if_t<has_conv_op<X&&,const volatile T&>::value>>
	  operator const volatile T&()&&;
	  template<class T,class=enable_if_t<has_conv_op<X&&,const volatile T&&>::value>>
	  operator const volatile T&&()&&;
	  template<class T,class=enable_if_t<has_conv_op<const X&&,T&>::value>>
	  operator T&()const&&;
	  template<class T,class=enable_if_t<has_conv_op<const X&&,T&&>::value>>
	  operator T&&()const&&;
	  template<class T,class=enable_if_t<has_conv_op<const X&&,const T&>::value>>
	  operator const T&()const&&;
	  template<class T,class=enable_if_t<has_conv_op<const X&&,const T&&>::value>>
	  operator const T&&()const&&;
	  template<class T,class=enable_if_t<has_conv_op<const X&&,const volatile T&>::value>>
	  operator const volatile T&()const&&;
	  template<class T,class=enable_if_t<has_conv_op<const X&&,const volatile T&&>::value>>
	  operator const volatile T&&()const&&;
	  template<class T,class=enable_if_t<has_conv_op<volatile X&&,T&>::value>>
	  operator T&()volatile&&;
	  template<class T,class=enable_if_t<has_conv_op<volatile X&&,T&&>::value>>
	  operator T&&()volatile&&;
	  template<class T,class=enable_if_t<has_conv_op<volatile X&&,const T&>::value>>
	  operator const T&()volatile&&;
	  template<class T,class=enable_if_t<has_conv_op<volatile X&&,const T&&>::value>>
	  operator const T&&()volatile&&;
	  template<class T,class=enable_if_t<has_conv_op<volatile X&&,const volatile T&>::value>>
	  operator const volatile T&()volatile&&;
	  template<class T,class=enable_if_t<has_conv_op<volatile X&&,const volatile T&&>::value>>
	  operator const volatile T&&()volatile&&;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X&&,T&>::value>>
	  operator T&() const volatile&&;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X&&,T&&>::value>>
	  operator T&&()const volatile&&;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X&&,const T&>::value>>
	  operator const T&()const volatile&&;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X&&,const T&&>::value>>
	  operator const T&&()const volatile&&;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X&&,const volatile T&>::value>>
	  operator const volatile T&()const volatile&&;
	  template<class T,class=enable_if_t<has_conv_op<const volatile X&&,const volatile T&&>::value>>
	  operator const volatile T&&()const volatile&&;
	  };
	template<class,class,class=void>
	struct is_convertion_op_direct_bindable:false_type{};
	template<class From,class To>
	struct is_convertion_op_direct_bindable<From,To,enable_if_t<is_class_v<remove_reference_t<From>>>>{
	   using conv_ref = propagate_cvref_t<From, check_conv_op_ref<remove_cvref_t<From>>>;
	   using conv_nref = propagate_cvref_t<From, check_conv_op_nref<remove_cvref_t<From>>>;
	   static constexpr bool value = is_reference_v<To> && is_convertible_v<From,To> && 
	       (is_convertible_v<conv_ref,To> || is_convertible_v<conv_nref,To>);
	   };
	}

namespace std{
	//is cv qualifier of T1 greater or equal to cv qualifiers of T2
	template<class T1,class T2>
	static constexpr bool is_cv_greater_or_eq = 
	  (   is_const_v<T1>    && is_volatile_v<T1>
	  || !is_const_v<T2> && !is_volatile_v<T2>
	  || !is_const_v<T1>    && is_volatile_v<T1> && !is_const_v<T2>
	  || !is_volatile_v<T1> && is_const_v<T1> && !is_volatile_v<T2>);
	
	//is T1 reference related to T2
	template<class T1,class T2>
	static constexpr bool is_reference_related_to_v = 
	      is_same_v<remove_cvref_t<T1>,remove_cvref_t<T2>> 
	   || is_base_of_v<remove_cvref_t<T1>,remove_cvref_t<T2>>;
	
	//is T1 reference compatible to T2
	template<class From,class To>
	static constexpr bool is_reference_compatible_v = is_reference_v<To>
	  && is_cv_greater_or_eq<remove_reference_t<To>,remove_reference_t<From>> 
	  && is_reference_related_to_v<remove_reference_t<To>,remove_noexcept_t<remove_cvref_t<From>>>;
	
	//is a value of type From (xvalue -> rv ref,prvale -> not ref) be bound to a
	//reference of type T1 without construction of a temporary
	template<class From,class To>
	static constexpr bool is_direct_bindable_v = is_reference_v<To> && (
	   (   is_lvalue_reference_v<To> &&
	     (   is_lvalue_reference_v<From> && is_reference_compatible_v<From,To> 
	      || _impl::is_convertion_op_direct_bindable<From,To>::value))
	 ||   
	      ((is_rvalue_reference_v<To> || is_const_v<remove_reference_t<To>> && !is_volatile_v<remove_reference_t<To>>)
	   && (   (!is_lvalue_reference_v<From> || is_function_v<remove_reference_t<From>>) && is_reference_compatible_v<From,To>
	        || _impl::is_convertion_op_direct_bindable<From,To>::value)));
	
	
    	}
