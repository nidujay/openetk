#ifndef FREERTOS_QUEUE_H
#define FREERTOS_QUEUE_H

#include <FreeRTOS/Source/include/FreeRTOS.h>
#include <FreeRTOS/Source/include/queue.h>
#include "messaging/imessage-queue.h"
#include "errors/icritical-error-handler.h"

namespace djetk {

/**
 * \brief FreeRTOS specific queue implementation of a message queue
 */
class FreeRTOSQueue : public IMessageQueue {
  public:
    /**
     * \brief Construct a FreeRTOS Queue object
     * \param[in] queue_length  Number of \ref Message objects held by the queue
     * \param[in] error_handler Callback reference to notify of errors
     * This function allocates a freeRTOS queue. Errors are notified via the
     * injected error handler.
     */
    FreeRTOSQueue(size_t queue_length, ICriticalErrorHandler &error_handler);

    /**
     * \brief Destructor
     * Invokes the underlying FreeRTOS queue deletion function. The ability
     * delete the queue depends on selected the memory allocation scheme
     */
    ~FreeRTOSQueue();

    virtual bool PostMessage(const Message &message, uint32_t timeout_ms) override;
    virtual bool PostMessageFromIsr(const Message &message, bool &task_woken) override;
    virtual bool ReceiveMessage(uint32_t timeout_ms, Message &message) override;
    virtual bool ReceiveMessageFromIsr(Message &message, bool &task_woken) override;

  private:
    FreeRTOSQueue(const FreeRTOSQueue &rhs);
    const FreeRTOSQueue& operator=(const FreeRTOSQueue &rhs);

    xQueueHandle message_queue_;
};

}   // namespace djetk

#endif

