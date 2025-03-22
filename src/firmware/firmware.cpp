#include <FreeRTOS.h>
#include <task.h>

#include <assert.hpp>
#include <bsp.hpp>
#include <firmware.hpp>
#include <logger.hpp>
#include <priority.hpp>
#include <service.hpp>
#include <state.hpp>

#include <led_receiver.hpp>
#include <led_sender.hpp>
#include <shell.hpp>

namespace fw
{

Firmware::Firmware()
{
    const BaseType_t status = xTaskCreate(cmn::bind_to<Firmware, &Firmware::root_thread>, "root_thread",
                                          configMINIMAL_STACK_SIZE, this, fr::prio::REAL_TIME, nullptr);

    ASSERT(status == pdPASS);
}

void Firmware::start() const
{
    vTaskStartScheduler();
}

void Firmware::root_thread()
{
    logger::create_and_start(logger::Severity::DEBUG);
    LOG_INFO("init logger");

    LOG_INFO("init bsp");
    ASSERT(bsp::init());

    LOG_INFO("init firmware");
    ASSERT(svc::create_and_start<Shell>());
    ASSERT(svc::create_and_start<LedReceiver>());
    ASSERT(svc::create_and_start<LedSender>());

    LOG_INFO("start firmware");
    vTaskDelete(nullptr);
}

}  // namespace fw
