#pragma once
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <limits>
#include <type_traits>
#include <utility>
#include <version>
#include <experimental/source_location>
#undef __GLIBC__


[[nodiscard]] inline auto
operator new (std::size_t size,void* ptr) noexcept
    -> void*
    { return ptr; }


[[nodiscard]] inline auto 
operator new[] (std::size_t size,void* ptr) noexcept
    -> void*
    { return ptr; }


inline void 
operator delete (void* ptr,void*) noexcept
    {}


inline void 
operator delete[] (void* ptr,void*) noexcept
    {}


namespace std
    {


    struct destroying_delete_t 
        {
        explicit destroying_delete_t() = default;
	};

    inline constexpr destroying_delete_t destroying_delete{};


    template <class T>
    [[nodiscard]] constexpr auto
    launder (T* p) noexcept
        -> T*
        { return __builtin_launder(p); }

    template <class R, class...Args, bool N>
    [[nodiscard]] constexpr void
    launder (R (*) (Args...) noexcept(N)) = delete;

    void 
    launder (void *) = delete;

    void 
    launder (const void *) = delete;

    void 
    launder (volatile void *) = delete;

    void 
    launder (const volatile void *) = delete;


    using ssize_t = make_signed_t<size_t>;

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


	template<class T>
	inline constexpr size_t tuple_size_v = tuple_size<T>::value;

	template<class,class,template<class> class,template<class> class>
	struct basic_common_reference;
	
	//common_reference_t
	namespace _impl{
	    template<class A>
	    using cref = add_lvalue_reference_t<const remove_reference_t<A>>;
	    template<class A>
	    struct xref{
	      template<class U>
	      using type = propagate_cvref_t<remove_cvref_t<U>,A>;
	      };
	    template<class From,class To>
	    using copycv = propagate_cv_t<To,From>;
	    template<class X,class Y>
	    using cond_res = decltype(false ? declval<X(&)()>()():declval<Y(&)()>()());
	
	    template<class A,class B,class=void,class=void,class=void>
	    struct com_ref_c;
	
	    template<class A,class B>
	    using com_ref = typename com_ref_c<conditional_t<is_lvalue_reference_v<A> && is_rvalue_reference_v<B>,B,A>
	                                      ,conditional_t<is_lvalue_reference_v<A> && is_rvalue_reference_v<B>,A,B>>
	                        ::type;
	    struct com_ref1{
	      template<class A,class B,class X=remove_reference_t<A>,class Y=remove_reference_t<B>,class=void>
	      struct type_{};
	      template<class A,class B,class X,class Y>
	      struct type_<A,B,X,Y
	                  ,enable_if_t<is_lvalue_reference_v<A> && is_lvalue_reference_v<B>
	                    && is_reference_v< cond_res<copycv<X,Y> &, copycv<Y,X>& > >>
	                  >
	        {
	        using type= cond_res<copycv<X,Y> &, copycv<Y,X>&>;
	        };
	      };
	    struct com_ref2{
	      template<class X_,class Y_>
	      using C =remove_reference_t<com_ref<X_&,Y_&>>&&;
	      template<class A,class B,class X=remove_reference_t<A>,class Y=remove_reference_t<B>,class=void>
	      struct type_{};
	      template<class A,class B,class X,class Y>
	      struct type_<A,B,X,Y,
	          enable_if_t<is_rvalue_reference_v<A> && is_rvalue_reference_v<B> &&
	                is_convertible_v<A,C<X,Y>> && is_convertible_v<B,C<X,Y>> >>
	        {
	        using type = C<X,Y>;
	        };
	      };
	    struct com_ref3{
	      template<class X_,class Y_>
	      using D = com_ref<const X_&,Y_&>;
	      template<class A,class B,class X=remove_reference_t<A>,class Y=remove_reference_t<B>,class=void>
	      struct type_{};
	      template<class A,class B,class X,class Y>
	      struct type_<A,B,X,Y,
	        enable_if_t<is_rvalue_reference_v<A> && is_lvalue_reference_v<B> 
	                && is_convertible_v<A,D<X,Y>> && is_convertible_v<B,D<X,Y>> >>
	        {
	        using type = D<X,Y>;
	        };
	      template<class A,class B,class X,class Y>
	      struct type_<A,B,X,Y,
	        enable_if_t<is_lvalue_reference_v<A> && is_rvalue_reference_v<B> 
	                && is_convertible_v<A,D<Y,X>> && is_convertible_v<B,D<Y,X>> >>
	        {
	        using type = D<Y,X>;
	        };
	      };
	    template<class A,class B>
	    struct com_ref_c<A,B,void_t<typename com_ref1::type_<A,B>::type>,void,void>{
	      using type = com_ref1::template type_<A,B>::type;
	      };
	    template<class A,class B,class T>
	    struct com_ref_c<A,B,void_t<typename com_ref2::template type_<A,B>::type>,T,void>{
	      using type = com_ref2::template type_<A,B>::type;
	      };
	    template<class A,class B,class T,class U>
	    struct com_ref_c<A,B,void_t<typename com_ref3::template type_<A,B>::type>,T,U>{
	      using type = com_ref3::template type_<A,B>::type;
	      };
	
	
	    template<class ,class ,class=void,class=void,class=void,class=void>
	    struct _common_ref2{};
	    template<class A,class B>
	    struct _common_ref2<A,B,void_t<com_ref<A,B>>,void,void,void>{
	      using type = com_ref<A,B>;
	      };
	    template<class A,class B,class T>
	    struct _common_ref2<A,B,void_t<typename basic_common_reference<remove_cvref_t<A>,remove_cvref_t<B>
	                                            ,xref<A>::template type,xref<B>::template type>::type>
	                            ,T,void,void>{
	      using type = basic_common_reference<remove_cvref_t<A>,remove_cvref_t<B>
	                                            ,xref<A>::template type,xref<B>::template type>::type;
	      };
	    template<class A,class B,class T,class U>
	    struct _common_ref2<A,B,void_t<cond_res<A,B>>,T,U,void>{
	      using type = cond_res<A,B>;
	      };
	    template<class A,class B,class T,class U,class V>
	    struct _common_ref2<A,B,void_t<common_type_t<A,B>>,T,U,V>{
	      using type = common_type_t<A,B>;
	      };
	    template<class T,class U,class=void>
	    struct reg_common_ref2{
	      struct type {};//no common ref with other types;
	      };
	    template<class T,class U>
	    struct reg_common_ref2<T,U,void_t<typename _common_ref2<T,U>::type>>{
	      using type = _common_ref2<T,U>::type;
	      };
	  }

	template<class...Others>
	struct common_reference{};
	template<class T>
	struct common_reference<T>{
	  using type=T;
	  };
	template<class T,class U>
	struct common_reference<T,U>:_impl::_common_ref2<T,U>{ };
	template<class T,class U,class...Others>
	struct common_reference<T,U,Others...>:common_reference<typename _impl::reg_common_ref2<T,U>::type,Others...>{};
	template<class...Ts>
	using common_reference_t = common_reference<Ts...>::type;


    }

	
     


