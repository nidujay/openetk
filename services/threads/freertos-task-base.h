#ifndef FREERTOS_TASK_BASE_H
#define FREERTOS_TASK_BASE_H

#include <FreeRTOS/Source/include/FreeRTOS.h>
#include <FreeRTOS/Source/include/task.h>
#include "errors/icritical-error-handler.h"

namespace djetk {

/**
 * \brief FreeRTOS Task base class
 *
 * All application specific threads can be derived from this.
 */
class FreeRTOSTaskBase {
  public:
    /**
     * \brief Create a FreeRTOS task object
     * \param[in]   error_handler   Reference to application specific error handler
     * \param[in]   name            Task name used by FreeRTOS
     * \param[in]   stack_depth     Stack depth as required by FreeRTOS
     * \param[in]   priority        FreeRTOS task priority
     *
     * This function creates the FreeRTOS thread and provides TaskMainBase as the
     * thread entry function. The thread entry function invokes TaskMain which has
     * to be implemented in the derived class. If the scheduler is started, the newly
     * created task is executed immediately.
     *
     * Any errors during task creation results in a notification via the injected
     * error handler.
     */
    FreeRTOSTaskBase(ICriticalErrorHandler &error_handler, const signed char *name,
            unsigned short stack_depth, unsigned portBASE_TYPE priority);

    /**
     * \brief Destroy the task object
     *
     * This deletes the created task. The selected memory allocation scheme
     * determines whether the allocated memory is freed or not.
     */
    virtual ~FreeRTOSTaskBase();

    /**
     * \brief Suspend the task
     */
    void Suspend();

  private:
    /**
     * \brief User defined Task main function
     *
     * \note This function should not return.
     */
    virtual void TaskMain() = 0;

    /**
     * \brief Thread entry function.
     * \param[in]   self    Pointer to the object that owns the task.
     */
    static void TaskMainBase(void *self);

    /**
     * \brief Handle to the freeRTOS task
     */
    xTaskHandle handle_;
};

}   // namespace

#endif

