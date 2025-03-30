#pragma once

#include <assert.hpp>

namespace cmn
{
namespace details
{

[[noreturn]] inline void unreachable()
{
    // we're not going to use MSVC compiler, Clang and GCC supported
    __builtin_unreachable();
}

}  // namespace details

/// @brief Asserts and never returns
[[noreturn]] inline void unreachable()
{
    ASSERT(false);
    details::unreachable();
}

/// @brief Stucks in an infinite loop and never returns
[[noreturn]] inline void stuck()
{
    while (true) {
    }
    details::unreachable();
}

}  // namespace cmn

#define UNREACHABLE() cmn::unreachable()
#define STUCK() cmn::stuck()
