#define ASSERTION_LEVEL 2
#include <fcpp/union_of.hpp>
#include <fcpp/tuple.hpp>
#include <fcpp/assertion.hpp>
#include "utils.hpp"
#include <kernel/file.hpp>
#include <kernel/process.hpp>
#include <elf/start.hpp>

[[noreturn]] void elf::start(void* stack,void(*)()){
	using namespace kernel;
	using namespace std;
	}
