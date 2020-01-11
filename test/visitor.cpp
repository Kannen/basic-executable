#define ASSERTION_LEVEL 2
#include <fcpp/visitor.hpp>
#include <fcpp/tuple.hpp>
#include <fcpp/assertion.hpp>
#include "utils.hpp"
#include <kernel/file.hpp>
#include <kernel/process.hpp>
#include <elf/start.hpp>

using namespace kernel;
using namespace std;

template <class...Args>
struct visited
	: std::tuple<Args...>
	{
	int index = 0;
	friend constexpr auto
	index(const visited& v){
		return v.index;
		}
	};
template <class...Args>
visited (Args...args) -> visited <Args...>;

static_assert (visitable<visited<int,int>>);

[[noreturn]] void elf::start(void* stack,void(*)()){

	auto a = visited{'a',10,11.1};
	int false_exec=0;
	visit <void> ([&](auto x){
			if constexpr (is_same_v <decltype(x), char>)
				Assert(x=='a');	
			else
				false_exec++;
			}, a);
	a.index=1;
	visit <void> ([&](auto x){
			if constexpr (is_same_v <decltype(x), int>)
				Assert(x==10);	
			else
				false_exec++;
			}, a);
	a.index=2;
	visit <void> ([&](auto x){
			if constexpr (is_same_v <decltype(x), double>)
				Assert(x==11.1);	
			else
				false_exec++;
			}, a);
	auto b = visited{'a',8.1};
	visit <void> ([&](auto x, auto y){
			if constexpr (is_same_v <decltype(x), double>){
				Assert(x==11.1);	
				}
			else
				false_exec++;
			if constexpr (is_same_v <decltype(y), char>){
				Assert(y=='a');	
				}
			else
				false_exec++;
			}, a,b);
	b.index=1;
	visit <void> ([&](auto x, auto y){
			if constexpr (is_same_v <decltype(x), double>){
				Assert(x==11.1);	
				}
			else
				false_exec++;
			if constexpr (is_same_v <decltype(y), double>){
				Assert(y==8.1);	
				}
			else
				false_exec++;
			}, a,b);
	a.index=0;
	Assert(visit ([&](auto x){
			if constexpr (is_same_v <decltype(x), char>){
				Assert(x=='a');	
				return 11.1;
				}
			else{
				false_exec++;
				return 10;
				}
			}, a) == 11.1);

	static_assert(is_same_v<decltype(visit ([&](auto x){
			if constexpr (is_same_v <decltype(x), char>){
				return 11.1;
				}
			else{
				false_exec++;
				return 10;
				}
			}, a) ),double>);
	a.index=1;
	b.index=0;
	Assert(visit     ([&](auto x, auto y){
			if constexpr (is_same_v <decltype(x), int>){
				Assert(x==10);	
				}
			else {
				false_exec++;
				}
			if constexpr (is_same_v <decltype(y), char>){
				Assert(y=='a');	
				}
			else{
				false_exec++;
				}
			return x;
			}, a,b) == 10);
	Assert(false_exec==0);
	a.index = 0;
	Assert(visit     ([&](auto x, int y){
			if constexpr (is_same_v <decltype(x), char>){
				Assert(x=='a');	
				}
			else {
				false_exec++;
				}
			return x;
			}, a,20) == 'a');

	exit(0);
  	}
