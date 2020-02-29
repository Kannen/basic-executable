#define ASSERTION_LEVEL 1
//#define COMPILE_TIME_CHECKS
#include <fcpp/assertion.hpp>
#include "utils.hpp"
#include <kernel/fs.hpp>
#include <kernel/kernel.hpp>
#include <elf/start.hpp>

#ifdef COMPILE_TIME_CHECKS
int h(int);
bool test(int i){
	return h(i) < 10;
	}
bool test2(int i){
	h(i);
	return i < 10;
	}
#endif


[[noreturn]] void elf::start(void* stack,void(*)()){
    using namespace kernel;
#ifdef COMPILE_TIME_CHECKS
#	if ASSERTION_LEVEL==2
	int i=20;
	int j=40;
    //UB with ASSERTION_LEVEL==1
	Assert(false);//		compile time assertion failure
	Assert(true);
	Assert(i<j,i,j);
	Assert(i<10,i);//		compile time assertion failure (RELEASE)
	Assert(test(i),i);//	assertion failure break constant expression (RELEASE)
	Assert(test2(i),i);//	compile time assertion failure (RELEASE)
#   elif ASSERTION_LEVEL==1 
	int i=5;
	Assert(test2(i),i);//	compile time trouble, assertion generates code 
#	endif
#endif    
	int v = get_argc(stack);
	Assert(v<2,v);
	if (v>=2){ //should optimized out with ASSERTION_LEVEL==1
	  out("No optimization thanks to assertion.\n"); //message not outputed if ASSERTION_LEVEL==1 and in RELEASE
	  }
	else{
	  out("Assertion enabled optimization.\n"); //message not outputed if ASSERTION_LEVEL!=1
	  }
	exit(0);
  	}
