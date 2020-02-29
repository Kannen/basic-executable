#include "utils.hpp"
#include <kernel/fs.hpp>

namespace std{
	void dynamic_assertion_failure(const char* expr, const char* file, int line)
		{
		using namespace kernel;
		out("Assertion failure ");
		out(file);out("[");out(line);out("]\n\t");
		out(expr);out("\n");
		}
	}
