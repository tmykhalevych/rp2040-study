#include <FreeRTOS.h>
#include <task.h>

#include <assert.hpp>
#include <bsp/bsp.hpp>
#include <firmware.hpp>
#include <logger.hpp>
#include <priority.hpp>
#include <service.hpp>
#include <state.hpp>

#include <led_receiver.hpp>
#include <led_sender.hpp>
#include <shell.hpp>
#include <tracer.hpp>

namespace fw
{

static constexpr auto ROOT_THREAD_STACK_SIZE = 512;

Firmware::Firmware()
{
    const BaseType_t status = xTaskCreate(cmn::bind_to<Firmware, &Firmware::root_thread>, "root_thread",
                                          ROOT_THREAD_STACK_SIZE, this, fr::prio::REAL_TIME, nullptr);

    ASSERT(status == pdPASS);
}

void Firmware::start() const
{
    vTaskStartScheduler();
}

void Firmware::root_thread()
{
    logger::create_and_start(logger::Severity::DEBUG);

    LOG_INFO("init bsp");
    ASSERT(bsp::init());

    fr::tracer::init_if_enabled();

    LOG_INFO("init firmware");
    ASSERT(svc::create_and_start<Shell>());
    ASSERT(svc::create_and_start<LedReceiver>());
    ASSERT(svc::create_and_start<LedSender>());

    LOG_INFO("start firmware");
    vTaskDelete(nullptr);
}

}  // namespace fw
