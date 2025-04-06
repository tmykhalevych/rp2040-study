
#pragma once

#include <chrono>

namespace bsp::chrono
{

namespace steady_clock
{

std::chrono::steady_clock::time_point now();

}  // namespace steady_clock

namespace system_clock
{

std::chrono::system_clock::time_point now();

}  // namespace system_clock

namespace hardware_clock
{

static constexpr auto FREQUENCY = 1'000'000;  // 1MHz
uint32_t now();

}  // namespace hardware_clock

}  // namespace bsp::chrono
