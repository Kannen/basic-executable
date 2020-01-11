#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-declarations"

//some memcpy familly defined in gcc-required.cpp
//
#include <fcpp/c++support.hpp>
#include <fcpp/types.hpp>

extern "C"{

[[gnu::pure]] auto
strcmp (const char* x,const char* y)
	-> int
	{
	int c=*x-*y;
	while (c == 0 && *x != '\0'){
		++x; ++y;
		c = *x - *y;
		}
	return c;
	}

[[gnu::pure]] auto
strlen (const char* x)
	-> std::size_t
	{
	std::ssize_t long c=0;
	while (x [c] != '\0') ++c;
	return static_cast <std::size_t>(c);
	}
 
}
#pragma GCC diagnostic pop
