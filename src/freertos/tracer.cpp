#include <bsp/chrono.hpp>
#include <tracer.hpp>

#ifdef CONFIG_INCLUDE_SYSVIEW
#include <SEGGER_SYSVIEW.h>
#endif

namespace fr::tracer
{

/// @brief Start tracing facility if enabled by configuration
void init_if_enabled()
{
#ifdef CONFIG_INCLUDE_SYSVIEW
    SEGGER_SYSVIEW_Conf();
#endif
}

#ifdef CONFIG_INCLUDE_SYSVIEW
extern "C" U32 SEGGER_SYSVIEW_X_GetTimestamp()
{
    return bsp::chrono::hardware_clock::now();
}
#endif

}  // namespace fr::tracer
