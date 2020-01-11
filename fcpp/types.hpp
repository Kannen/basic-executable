#pragma once
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#undef __GLIBC__

namespace std
	{

	using ssize_t = make_signed_t<size_t>;

	}
