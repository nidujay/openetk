#ifndef FREERTOS_SCHEDULER_H
#define FREERTOS_SCHEDULER_H

namespace djetk {

/**
 * \brief Wrapper class to access the FreeRTOS scheduler
 * \todo We probably don't need a class for this as there's no
 * state managed by this object.
 * Uses the singleton pattern to limit the number of class instances
 * to 1.
 */
class FreeRTOSScheduler {
 public:
    /**
     * \brief Get an instance to the scheduler
     */
    static FreeRTOSScheduler& GetScheduler();

    /**
     * \brief Start the FreeRTOS scheduler
     */
    void Start();

    /**
     * \brief Stop the FreeRTOS scheduler.
     * \note Support for this function depends on the selected port
     */
    void Stop();

 private:
    /**
     * \brief Default constructor used privately
     */
    FreeRTOSScheduler();

    /**
     * \brief Disabled copy constructor
     */
    FreeRTOSScheduler(const FreeRTOSScheduler &rhs);
};

}   // namespace

#endif

