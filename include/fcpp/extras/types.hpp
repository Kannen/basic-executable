#pragma once
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace std
	{

	using ssize_t = make_signed_t<size_t>;

	}
