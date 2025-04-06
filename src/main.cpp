#include <FreeRTOS.h>
#include <task.h>

#include <firmware.hpp>
#include <unreachable.hpp>

#include <cstdio>

int main()
{
    auto firmware = fw::Firmware();
    firmware.start();

    return 0;
}

extern "C" void vApplicationMallocFailedHook()
{
    std::printf("allocation failure\n");
    STUCK();
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t, char* task_name)
{
    std::printf("stack overflow [tsk:%s]\n", task_name);
    STUCK();
}

extern "C" void vAssertCalled(const char* file_name, unsigned line)
{
    std::printf("assert at %s:%u\n", file_name, line);
    STUCK();
}
