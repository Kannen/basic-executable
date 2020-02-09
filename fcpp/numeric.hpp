#pragma once

#include <numeric> //éviter les parallèles
#include <bit>

#undef __GLIBC__

namespace std{
	template <class Arg, class...Args>
	consteval auto
	max_of(Arg&& arg, Args&&...args)
		{
		if constexpr (sizeof...(Args) == 0)
			return arg;
		else {
			auto r = max_of(forward<Args>(args)...);
			return r > arg ? r : arg;
			}
		}
	}
		

