#define ASSERTION_LEVEL 2
//#define COMPILE_TIME_CHECKS
#include <fcpp/array.hpp>
#include "utils.hpp"
#include <kernel/fs.hpp>
#include <kernel/kernel.hpp>
#include <elf/start.hpp>


[[noreturn]] void elf::start(void* stack,void(*)()){
	using namespace kernel;
	using namespace std;
	array x{0,5};
	Assert(size(x)==2);
	Assert(x[0]==0);
	Assert(x[1]==5);
	auto [a,b]=x;
	Assert(a==0);
	Assert(b==5);
	int count=0;
	for(auto val:x){
		if (count == 0) Assert (val == 0);
		if (count == 1) Assert (val == 5);
		++count;
		}
	Assert(count==2);
	exit(0);
  	}
