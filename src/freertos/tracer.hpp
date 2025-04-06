#pragma once

#include <FreeRTOS.h>

namespace fr::tracer
{

/// @brief Start tracing facility if enabled by configuration
void init_if_enabled();

}  // namespace fr::tracer
