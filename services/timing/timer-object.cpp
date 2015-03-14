/**
    \file
    \brief

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

#include <timing/timer-object.h>

namespace djetk {

TimerObject::TimerObject(ITickSource &tick_source, IMessageQueue &message_queue,
            uint32_t message_id, ICriticalErrorHandler &error_handler,
            IOsServices &os_services)
    : tick_source_(tick_source),
    os_services_(os_services),
    message_queue_(message_queue),
    message_id_(message_id),
    reference_(0),
    started_(false),
    ticks_remaining_(0)
{
    if (!tick_source.RegisterTickClient(*this)) {
        // Normally this function doesn't return, but during unit testing it
        // does so break out after calling it.
        error_handler.NotifyCriticalError(ICriticalErrorHandler::service_registration_error,
                __FILE__, __LINE__);
        return;
    }
}

TimerObject::~TimerObject()
{
    // TODO Unregister from the tick source
}

void TimerObject::Start(uint32_t timeout_ms, int reference)
{
    auto ticks = tick_source_.MsToTicks(timeout_ms);
    if (!ticks) {
        ticks++;
    }

    // Nested block has interrupts disabled
    {
        AutoInterruptDisabler disabler(os_services_);
        reference_ = reference;
        ticks_remaining_ = ticks;
        started_ = true;
    }
}

void TimerObject::Stop()
{
    AutoInterruptDisabler disabler(os_services_);
    started_ = false;
}

bool TimerObject::OnTickFromIsr(bool &task_woken)
{
    if (!started_) {
        return true;
    }

    ticks_remaining_--;
    if (ticks_remaining_) {
        return true;
    }

    // Stop the timer
    started_ = false;
    TimeoutMessage message(message_id_, reference_);
    return message_queue_.PostMessageFromIsr(message, task_woken);
}

}    // namespace djetk

