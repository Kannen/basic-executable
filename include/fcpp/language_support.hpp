#pragma once

#include <new>
#include <cstdarg>
#include <coroutine>

//because of tuple_element
//           tuple_size
#include <tuple>


#ifdef  FREE_CPP_FREESTANDING
#undef   __GLIBC__
#endif
