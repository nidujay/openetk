#include "threads/freertos-task-base.h"

namespace djetk {

FreeRTOSTaskBase::FreeRTOSTaskBase(ICriticalErrorHandler &error_handler,
        const signed char *name, unsigned short stack_depth, unsigned portBASE_TYPE priority)
{
    auto rv = xTaskCreate(TaskMainBase, name, stack_depth, this,
                priority, &handle_);

    // Critical Error Handler notifications don't return
    if (rv != pdPASS) {
        error_handler.NotifyCriticalError(ICriticalErrorHandler::freertos_error,
               __FILE__, __LINE__ );
    }
}

FreeRTOSTaskBase::~FreeRTOSTaskBase()
{
    vTaskDelete(handle_);
}

void FreeRTOSTaskBase::Suspend()
{
    vTaskSuspend(handle_);
}

void FreeRTOSTaskBase::TaskMainBase(void *self)
{
    auto self_ = static_cast<FreeRTOSTaskBase *>(self);
    self_->TaskMain();
}

}   // namespace

