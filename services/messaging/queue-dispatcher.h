#ifndef QUEUE_DISPATCHER_H
#define QUEUE_DISPATCHER_H

#include <cstddef>
#include "messaging/imessage-dispatcher.h"
#include "messaging/imessage-queue.h"

namespace djetk {

/**
 * \brief Implementation of a dispatcher of messages from a queue
 */
class QueueDispatcher : public IMessageDispatcher {
  public:
    /**
     * \brief Dispatcher constructor
     * \param[in]   message_queue Reference to the message queue to dispatch from.
     *              The caller owns the object.
     */
    explicit QueueDispatcher(IMessageQueue &message_queue);

    /**
     * \brief See \ref IMessageDispatcher::RegisterHandler
     * This class supports a single message handler. Subsequent calls returns
     * false to indicate a failure.
     */
    virtual bool RegisterHandler(IMessageHandler &message_handler) override;

    /**
     * \brief See \ref IMessageDispatcher::Poll
     */
    virtual void Poll() override;

  private:
    /**
     * \brief The message queue that the dispatcher will block on
     */
    IMessageQueue &message_queue_;

    /**
     * \brief Reference to registered message handler.
     */
    IMessageHandler *message_handler_;
};

}  // namespace djetk

#endif

