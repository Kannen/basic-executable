#define ASSERTION_LEVEL 2
//#define COMPILE_TIME_CHECKS
#include <fcpp/tuple.hpp>
#include <fcpp/assertion.hpp>
#include "utils.hpp"
#include <kernel/file.hpp>
#include <kernel/process.hpp>
#include <elf/start.hpp>

struct A{};
struct B{
  int i;
  };
struct C{
  int i;
  B b;
  };

[[noreturn]] void elf::start(void* stack,void(*)())
	{
	using namespace kernel;
	using namespace std;
	{
	auto a = tuple{1,2.1};
	Assert(get<0>(a)==1);
	Assert(get<1>(a)==2.1);
	Assert(size(a)==2);
	const auto [b,c] = a;
	Assert(b==1 && c==2.1);
	}
	static_assert(sizeof(tuple<A,B>)==sizeof(B));
	static_assert(sizeof(tuple<B,A,B,B,A>)==3*sizeof(B));
	{
	constexpr auto a = tuple{3,B{10},5.1};
	constexpr auto b = tuple{C{10,3},'a'};
	static_assert(size(a) == 3);
	static_assert(tuple_size<decltype(a)>::value == 3);
	static_assert(tuple_size_v<decltype(b)> == 2);
	constexpr auto c = tuple_concat(a,b);
	static_assert( size(c) == 5);
	Assert(get<0>(c)==3);
	Assert(get<1>(c).i==10);
	Assert(get<2>(c)==5.1);
	Assert(get<3>(c).i==10);
	Assert(get<3>(c).b.i==3);
	Assert(get<4>(c)=='a');
	}
	{
	auto a = tuple{1,'a',3.5};
	int i;
	char j;
	double k;
	tie(i,j,k) = a;
	Assert(i==1);
	Assert(j=='a');
	Assert(k==3.5);
	}
	exit(0);
	}
