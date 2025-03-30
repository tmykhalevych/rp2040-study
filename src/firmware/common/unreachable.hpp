#pragma once

#include <assert.hpp>

namespace cmn
{

[[noreturn]] inline void unreachable()
{
    ASSERT(false);

    // we're not going to use MSVC compiler, Clang and GCC supported
    __builtin_unreachable();
}

}  // namespace cmn

#define UNREACHABLE() cmn::unreachable()
