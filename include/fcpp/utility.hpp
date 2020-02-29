#pragma once

#include <utility>
#include <tuple>
#include <optional>
#include <variant>
#include <any>
#include <bitset>
#include <functional>
#include <ratio>
#include <charconv>
#include <string_view>
#include <span>

#include <fcpp/extra/assertion.hpp>
#include <fcpp/extra/utility.hpp>
#include <fcpp/extra/visitor.hpp>

#ifdef  FREE_CPP_FREESTANDING
#undef   __GLIBC__
#endif
