/**
    \file
    \brief Timer service running off FreeRTOS tick hook

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

#include <FreeRTOS/Source/include/FreeRTOS.h>
#include <timing/freertos-tick-hook-timer.h>
#include <timing/freertos-ticks.h>

namespace djetk {

FreeRTOSTickHookTimer::FreeRTOSTickHookTimer(TickClientsList &clients_buffer,
        ICriticalErrorHandler &error_handler)
    : clients_buffer_(clients_buffer),
    last_registered_client_(clients_buffer.begin()),
    error_handler_(error_handler)
{
    // We're running off the tick hook isr. So register ourselves for that service
    if (!tick_hook_isr_.RegisterHandler(*this)) {
        error_handler_.NotifyCriticalError(ICriticalErrorHandler::service_registration_error,
            __FILE__, __LINE__);
    }
}

FreeRTOSTickHookTimer::~FreeRTOSTickHookTimer()
{
    tick_hook_isr_.UnregisterHandler();
}

bool FreeRTOSTickHookTimer::RegisterTickClient(ITickClient &client)
{
    // Register the client if the buffer is not full
    if (last_registered_client_ == clients_buffer_.end()) {
        return false;
    }

    *last_registered_client_= &client;
    last_registered_client_++;
    return true;
}

uint32_t FreeRTOSTickHookTimer::MsToTicks(uint32_t milliseconds)
{
    auto ticks = ms_to_FreeRTOSTicks(milliseconds);
    return ticks;
}

void FreeRTOSTickHookTimer::HandleIsr(bool &task_woken)
{
    // Notify all the registered tick clients that a tick event has occurred
    for (auto it = clients_buffer_.begin(); it != last_registered_client_; it++) {
        auto result = (*it)->OnTickFromIsr(task_woken);
        // Notify error handler if failed to write to queue
        if (!result) {
            error_handler_.NotifyCriticalError(ICriticalErrorHandler::isr_handler_error,
                __FILE__, __LINE__);
        }
    }
}

/**
 * \brief FreeRTOS tick hook implementation
 *
 * This is invoked by FreeRTOS if the preprocessor define configUSE_TICK_HOOK is set to 1
 */
extern "C" void vApplicationTickHook()
{
    bool task_woken = false;
    IsrService<FreeRTOSTickHookTimer>::ISR(task_woken);
    // TODO Enable following line.
    //portEND_SWITCHING_ISR(task_woken ? pdTRUE : pdFALSE);
}

}    // namespace djetk


