#pragma once
#include <cstddef>
#include <initializer_list>
#include <utility> //tuple_element tupe_size
#include <version>
#include <compare>
#include <limits>

#include <bit> //for is_pow2 //for bit_cast when it will be implemented

#undef __GLIBC__

#ifndef  FREE_CPP_FREESTANDING
#	include <new>
#	include <memory>
#	include <tuple>
#else

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

	struct nothrow_t
		{
		explicit nothrow_t() = default;
		};
	inline constexpr auto nothrow = nothrow_t{};

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
	constexpr void
	launder (R (*) (Args...) noexcept(N)) = delete;
	
	void 
	launder (void *) = delete;
	
	void 
	launder (const void *) = delete;
	
	void 
	launder (volatile void *) = delete;
	
	void 
	launder (const volatile void *) = delete;

	

	template <size_t Align, class T>
	[[nodiscard, gnu::always_inline]]
	constexpr auto
	assume_aligned(T* ptr)
		{
		static_assert(ispow2(Align));
		return static_cast<T*>(__builtin_assume_aligned(ptr,Align));
		}

	template<class T>
	inline constexpr size_t tuple_size_v = tuple_size<T>::value;

    	}
#endif
