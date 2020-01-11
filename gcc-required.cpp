//As documented in gcc-manual §2 Language Standards Supported by GCC
//Even in freestanding environment, gcc expects that a library
//provides memcpy, memmove, memset, memcmp whit the specified behavior
//
//TODO: optimized versions
//
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-declarations"

#include <fcpp/types.hpp>

extern "C" 
[[gnu::pure]] auto 
memcpy (void* __restrict x,const void* __restrict y, std::size_t count)
	-> void*
	{
	std::ssize_t i=0;
  	while (i < static_cast <std::ssize_t> (count)){
		static_cast <unsigned char*> (x) [i] = static_cast <const unsigned char*> (y) [i];
		++i;
		}
	return x;
	}

extern "C" 
[[gnu::pure]] auto 
memmove (void* x, const void* y, std::size_t count)
	-> void*
	{
	std::ssize_t i=0;
  	while (i < static_cast <std::ssize_t> (count)){
		static_cast <unsigned char*> (x) [i] = static_cast <const unsigned char*> (y) [i];
		++i;
		}
	return x;
	}

extern "C" 
[[gnu::pure]] auto 
memset (void* x, int val, const std::size_t count)
	{
  	auto c = static_cast <unsigned char> (val);
  	std::ssize_t i=0;
  	while (i < static_cast <std::ssize_t> (count)){
  		static_cast <unsigned char*> (x) [i] = c;
  		++i;
  		}
  	return x;
  	}

extern "C" 
[[gnu::pure]] auto 
memcmp (const void* lhs,const void* rhs,const std::size_t count)
	-> int
	{
	std::ssize_t i=0;
  	while (i < static_cast <std::ssize_t> (count)){
	        const auto def = static_cast <const unsigned char*> (lhs) [i] 
				- static_cast <const unsigned char*> (rhs) [i];
		if (def != 0) return def;
		++i;
		}
	return 0;
	}
#pragma GCC diagnostic pop
