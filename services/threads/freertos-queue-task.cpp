#include "threads/freertos-queue-task.h"

namespace djetk {

FreeRTOSQueueTask::FreeRTOSQueueTask(ICriticalErrorHandler &error_handler, const signed char *name,
            unsigned short stack_depth, unsigned portBASE_TYPE priority,
            IMessageDispatcher &message_dispatcher)
:FreeRTOSTaskBase(error_handler, name, stack_depth, priority),
message_dispatcher_(message_dispatcher)
{
}

FreeRTOSQueueTask::~FreeRTOSQueueTask()
{
}

void FreeRTOSQueueTask::TaskMain()
{
    for(;;) {
        // The dispatcher takes care of waiting and acting on events
        message_dispatcher_.Poll();
    }
}

}   // namespace

