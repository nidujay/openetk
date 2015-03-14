/**
    \file
    \brief Message Queue test stub

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

#ifndef MESSAGE_QUEUE_STUB_H
#define MESSAGE_QUEUE_STUB_H

#include <messaging/imessage-queue.h>

namespace djetk {

/**
 * \brief Message queue stub for testing
 */
class MessageQueueStub : public IMessageQueue {
  public:
    MessageQueueStub()
        : post_count(0) {}

    virtual bool PostMessage(const Message &message, uint32_t timeout_ms) override
    {
        posted_msg = message;
        post_count++;
        (void)timeout_ms;
        return true;
    }

    virtual bool PostMessageFromIsr(const Message &message, bool &task_woken) override
    {
        (void)task_woken;
        posted_msg = message;
        post_count++;
        return true;
    }

    virtual bool ReceiveMessage(uint32_t timeout_ms, Message &message) override
    {
        (void)timeout_ms;
        message = msg_out;
        return true;
    }

    virtual bool ReceiveMessageFromIsr(Message &message, bool &task_woken) override
    {
        (void)message;
        (void)task_woken;
        return true;
    }

    /**
     * \brief Message returned from received functions
     */
    Message msg_out;

    /**
     * \brief Copy of the last message posted
     */
    Message posted_msg;

    /**
     * \brief Number of messages posted
     */
    int post_count;
};

}    // namespace djetk

#endif    // MESSAGE_QUEUE_STUB_H

