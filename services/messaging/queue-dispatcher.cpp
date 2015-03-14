#include <messaging/queue-dispatcher.h>
#include <timing/freertos-ticks.h>

namespace djetk {

QueueDispatcher::QueueDispatcher(IMessageQueue &message_queue)
    : message_queue_(message_queue),
    message_handler_(nullptr)
{
}

bool QueueDispatcher::RegisterHandler(IMessageHandler &message_handler)
{
    if (message_handler_ != nullptr) {
        return false;
    }

    message_handler_ = &message_handler;
    return true;
}

void QueueDispatcher::Poll()
{
    if (message_handler_ == nullptr) {
        return;
    }

    // Read from the message queue. This function should also support a
    // polled timer list that sets the timeout. The timer list can also post
    // a timeout message with the id of the timer that expired
    Message msg;
    // TODO timeout
    if (message_queue_.ReceiveMessage(infinite_ms, msg)) {
        message_handler_->HandleMessage(msg);
        // TODO check return code
    }
}

}

