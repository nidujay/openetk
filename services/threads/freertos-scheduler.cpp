#include <FreeRTOS/Source/include/FreeRTOS.h>
#include <FreeRTOS/Source/include/task.h>

#include "threads/freertos-scheduler.h"

namespace djetk {

FreeRTOSScheduler& FreeRTOSScheduler::GetScheduler() {
    static FreeRTOSScheduler scheduler;
    return scheduler;
}

FreeRTOSScheduler::FreeRTOSScheduler()
{
}

void FreeRTOSScheduler::Start()
{
    vTaskStartScheduler();
}

void FreeRTOSScheduler::Stop()
{
    vTaskEndScheduler();
}

}   // namespace

extern "C" void vApplicationIdleHook()
{
}

