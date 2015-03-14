#include <FreeRTOS/Source/include/FreeRTOS.h>
#include <FreeRTOS/Source/include/task.h>
#include "messaging/freertos-queue.h"
#include <timing/freertos-ticks.h>

namespace djetk {

FreeRTOSQueue::FreeRTOSQueue(size_t queue_length, ICriticalErrorHandler &error_handler)
{
    message_queue_ = xQueueCreate(queue_length, sizeof(Message));
    if (message_queue_ == 0) {
        error_handler.NotifyCriticalError(ICriticalErrorHandler::freertos_error,
               __FILE__, __LINE__ );
    }
}

FreeRTOSQueue::~FreeRTOSQueue()
{
    vQueueDelete(message_queue_);
}

bool FreeRTOSQueue::PostMessage(const Message &message, uint32_t timeout_ms)
{
    auto timeout = ms_to_FreeRTOSTicks(timeout_ms);
    if (xQueueSend( message_queue_, &message, timeout ) != pdPASS ) {
        return false;
    }

    return true;
}

bool FreeRTOSQueue::PostMessageFromIsr(const Message &message, bool &task_woken)
{
    // TODO Need a test case for this function
    portBASE_TYPE xHigherPriorityTaskWoken;
    if (xQueueSendFromISR(message_queue_, &message, &xHigherPriorityTaskWoken) == pdFALSE) {
        return false;
    }

    task_woken = (xHigherPriorityTaskWoken == pdTRUE);
    return true;
}

bool FreeRTOSQueue::ReceiveMessage(uint32_t timeout_ms, Message &message)
{
    auto timeout = ms_to_FreeRTOSTicks(timeout_ms);
    if (xQueueReceive(message_queue_, &message, timeout) != pdPASS ) {
        return false;
    }

    return true;
}

bool FreeRTOSQueue::ReceiveMessageFromIsr(Message &message, bool &task_woken)
{
    // TODO Need a test case for this function
    portBASE_TYPE xHigherPriorityTaskWoken;
    if (xQueueReceiveFromISR(message_queue_, &message, &xHigherPriorityTaskWoken) == pdFALSE) {
        return false;
    }

    task_woken = (xHigherPriorityTaskWoken == pdTRUE);
    return true;
}

}   // namespace djetk


