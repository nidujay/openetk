/**
    \file
    \brief Timer Object definition

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

#ifndef TIMER_OBJECT_H
#define TIMER_OBJECT_H

#include <timing/itick-client.h>
#include <timing/itick-source.h>
#include <messaging/imessage-queue.h>
#include <errors/icritical-error-handler.h>
#include <os/ios-services.h>

namespace djetk {

/**
 * \brief Timer Object
 *  - Listens to tick events from an injected tick source and posts
 *    messages (id injected) to an injected message queue
 *  - Expects ticks to originate from ISR context
 *  - Start/Stop operations disable interrupts temporarily
 */
class TimerObject : private ITickClient {
  public:
    /**
     * \brief Construct a timer object
     * \param[in]   tick_source     Tick Source to register to
     * \param[in]   message_queue   Queue to post timeout messages to
     * \param[in]   message_id      ID of posted message
     * \param[in]   error_handler   Callback to notify construction errors
     * \param[in]   os_services     Interface to OS services
     *
     * Registers to the injected tick source during construction. Any
     * errors during construction are communicated via the error handler
     */
    TimerObject(ITickSource &tick_source, IMessageQueue &message_queue,
            uint32_t message_id, ICriticalErrorHandler &error_handler,
            IOsServices &os_services);

    ~TimerObject();

    /**
     * \brief (Re)Start the timer
     * \param[in]   timeout_ms  Timeout period in milliseconds
     * \param[in]   reference   Application specific reference id
     *
     * Start the timer if not already running. If the timer is running, it's
     * re-started. The reference number is included in the timeout message that
     * is posted to the queue that is injected during construction. This allows
     * the client to avoid a race that can occur when a timeout event is in the
     * queue when the Stop method is invoked.
     *
     * \note if the timeout_ms never converts to 0 ticks. At-least 1 system
     * tick is guaranteed.
     */
    void Start(uint32_t timeout_ms, int reference);

    /**
     * \brief Stop the timer if it's started
     */
    void Stop();

  private:
    ITickSource &tick_source_;
    IOsServices &os_services_;
    IMessageQueue &message_queue_;
    uint32_t message_id_;
    int reference_;
    bool started_;
    uint32_t ticks_remaining_;

    // ITickClient interface implementation is private so that only the service
    // that the interface is exposed only to the service that this object registers
    // with.
    /**
     * \brief See \ref ITickClient::OnTickFromIsr
     */
    virtual bool OnTickFromIsr(bool &task_woken) override;
};

/**
 * \brief Timer object expiry message
 * This message is sent when the \ref TimerObject expires
 */
struct TimeoutMessage : public Message {
    /**
     * \brief Construct a timeout message
     * \param[in]   id          Application specific message id
     * \param[in]   reference   Timer reference id
     *
     * The id and reference are the values provided when creating and
     * starting the timer object respectively.
     */
    explicit TimeoutMessage(uint32_t id, int reference)
        : Message(id, reference) {}

    /**
     * \brief Get the reference id of the timer
     */
    int GetReference() const
    {
        return static_cast<int>(payload.data);
    }
};

}    // namespace djetk

#endif    // TIMER_OBJECT_H

