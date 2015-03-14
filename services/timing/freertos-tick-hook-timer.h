/**
    \file
    \brief Tick source running off FreeRTOS tick hook function

    \copyright
    Copyright (C) 2015 Dushara Jayasinghe

    This file is part of DJETK

    DJETK is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DJETK is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with DJETK.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FREERTOS_TICK_HOOK_TIMER_H
#define FREERTOS_TICK_HOOK_TIMER_H

#include <iterator>
#include <timing/itick-source.h>
#include <isr/isr-service.h>
#include <utilities/buffptr.h>
#include <errors/icritical-error-handler.h>

namespace djetk {

// typedef of tick client pointer list to make it easier to use later on
/**
 * \brief Buffer pointer to a list (array) of tick clients
 */
typedef buffptr<ITickClient*> TickClientsList;

/**
 * \brief FreeRTOS Tick Hook Timer definition
 *
 * This class maintains a list of registered timer clients to be notified on each tick
 * - Requires a buffer to be injected during construction. This buffer is contains the
 *   list of registered clients. This means the creator has to allocate a big enough
 *   buffer for all the timers.
 * - Implements the FreeRTOS tick hook - requires to be configUSE_TICK_HOOK defined as
 *   a non zero value
 * - Interrupts are not disabled during registration (this is assumed to occur before
 *   the scheduler is started)
 */
class FreeRTOSTickHookTimer : public ITickSource,
                              private IIsrHandler {
  public:
    /**
     * \brief Construct the timer using the injected clients buffer
     * \param[in]   clients_buffer  Buffer to store pointers to the registered clients
     * \param[in]   error_handler   Callback reference to notify of errors
     *
     * This class reads from and writes the clients buffer, but does not take owener
     * ship of the memory
     */
    FreeRTOSTickHookTimer(TickClientsList &clients_buffer,
            ICriticalErrorHandler &error_handler);
    ~FreeRTOSTickHookTimer();

    // Methods from ITickSource
    /**
     * \brief See \ref ITickSource::RegisterTickClient
     */
    virtual bool RegisterTickClient(ITickClient &client) override;
    /**
     * \brief See \ref ITickSource::MsToTicks
     */
    virtual uint32_t MsToTicks(uint32_t milliseconds) override;

  private:
    // Methods from IIsrHandler
    /**
     * \brief See \ref IIsrHandler::HandleIsr
     */
    virtual void HandleIsr(bool &task_woken) override;
    IsrService<FreeRTOSTickHookTimer> tick_hook_isr_;
    TickClientsList &clients_buffer_;

    TickClientsList::iterator last_registered_client_;
    ICriticalErrorHandler &error_handler_;
};

}    // namespace djetk

#endif    // FREERTOS_TICK_HOOK_TIMER_H

