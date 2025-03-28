#include <bsp/chrono.hpp>

namespace bsp::chrono
{

namespace steady_clock
{

std::chrono::steady_clock::time_point now()
{
    return std::chrono::steady_clock::now();
}

}  // namespace steady_clock

namespace system_clock
{

std::chrono::system_clock::time_point now()
{
    return std::chrono::system_clock::now();
}

}  // namespace system_clock

}  // namespace bsp::chrono
