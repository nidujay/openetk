#ifndef FREERTOS_QUEUE_TASK_H
#define FREERTOS_QUEUE_TASK_H

#include "threads/freertos-task-base.h"
#include "messaging/imessage-dispatcher.h"

namespace djetk {

/**
 * \brief FreeRTOS task that polls a \ref IMessageDispatcher
 *
 * - Continually polls the injected message dispatcher.
 */
class FreeRTOSQueueTask : public FreeRTOSTaskBase {
  public:
    /**
     * \brief Task constructor
     * \param[in]   error_handler   Reference to error handling interface
     * \param[in]   name            Task name
     * \param[in]   stack_depth     Task stack depth
     * \param[in]   priority        Task priority
     * \param[in]   message_dispatcher  Reference to message dispatcher to poll
     */
    FreeRTOSQueueTask(ICriticalErrorHandler &error_handler, const signed char *name,
            unsigned short stack_depth, unsigned portBASE_TYPE priority,
            IMessageDispatcher &message_dispatcher);
    /**
     * \brief See \ref FreeRTOSTaskBase destructor for more details
     */
    virtual ~FreeRTOSQueueTask();

  private:
    virtual void TaskMain() override;

    IMessageDispatcher &message_dispatcher_;
};

}   // namespace

#endif

