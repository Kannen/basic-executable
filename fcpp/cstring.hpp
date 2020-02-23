#pragma once
#include <fcpp/types.hpp>

#ifndef  FREE_CPP_FREESTANDING
#	include <cstring>
#else
//definitions in cstring.cpp and gcc-required.cpp
//
namespace std
	{

	inline auto 
	memcpy (void* __restrict x,const void* __restrict y, std::size_t count)
		{
		return __builtin_memcpy(x,y,count);
		}

	inline auto 
	memmove (void* x, const void* y, std::size_t count)
		{
		return __builtin_memmove(x,y,count);
		}

	inline auto 
	memset (void* x, int val, const std::size_t count)
		{
		return __builtin_memset(x,val,count);
		}

	inline auto 
	memcmp (const void* lhs,const void* rhs,const std::size_t count)
		{
		return __builtin_memcmp(lhs,rhs,count);
		}


	inline auto
	strcmp (const char* x,const char* y)
		{
		return __builtin_strcmp(x,y);
		}

	template<size_t N>
	constexpr inline auto
	strlen(const char (&text)[N]){
		return N-1;
		}

	inline auto
	strlen (const char* x)
		{
		return __builtin_strlen(x);
		}

	}

#endif
