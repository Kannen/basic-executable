#define ASSERTION_LEVEL 2
#include <fcpp/assertion.hpp>
#include <fcpp/variant.hpp>
#include "utils.hpp"
#include <kernel/file.hpp>
#include <kernel/process.hpp>
#include <elf/start.hpp>

using namespace kernel;
using namespace std;

struct h
	{
	h()=default;
	h(const h&){};
	~h(){}
	};

[[noreturn]] void 
elf::start (void* stack, void(*)())
	{
	variant <int,double> x{10.1};
	Assert(index(x)==1);
	static_assert(is_trivially_default_constructible_v <variant <int,double>>);
	static_assert(is_trivially_copy_constructible_v <variant <int,double>>);
	static_assert(is_trivially_move_constructible_v <variant <int,double>>);
	static_assert(is_trivially_copy_assignable_v <variant <int,double>>);
	static_assert(is_trivially_move_assignable_v <variant <int,double>>);
	static_assert(is_trivially_destructible_v <variant <int,double>>);
	variant <int,h> y{10};
	variant <h,h> w{in_place_index<0>};
	auto z = y;
	exit (0);
	}
