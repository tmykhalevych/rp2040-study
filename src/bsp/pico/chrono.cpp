#include <bsp/chrono.hpp>

#include <pico/time.h>

namespace
{

uint64_t ms_since_boot()
{
    return to_ms_since_boot(get_absolute_time());
}

}  // namespace

namespace bsp::chrono
{

// TODO: Add RTC to have real time value instead started from boot

namespace steady_clock
{

std::chrono::steady_clock::time_point now()
{
    return std::chrono::steady_clock::time_point{std::chrono::milliseconds{ms_since_boot()}};
}

}  // namespace steady_clock

namespace system_clock
{

std::chrono::system_clock::time_point now()
{
    return std::chrono::system_clock::time_point{std::chrono::milliseconds{ms_since_boot()}};
}

}  // namespace system_clock

}  // namespace bsp::chrono
