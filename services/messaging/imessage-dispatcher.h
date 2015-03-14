#ifndef IMESSAGE_DISPATCHER_H
#define IMESSAGE_DISPATCHER_H

#include <messaging/message.h>

namespace djetk {

/**
 * \brief Message handler interface
 * Message handlers can register themselves to a \ref IMessageDispatcher
 * to be notified messages are available to be handled.
 */
class IMessageHandler {
  public:
    /**
     * \brief Handle the given message
     * \param[in] msg   Reference to Message object
     * \retval true  Message handled
     * \retval false Message ignored
     * \note The return code is not for error purposes. It's used to implement
     *       the 'chain of responsibility' design pattern
     */
    virtual bool HandleMessage(const Message &msg) = 0;
};

/**
 * \brief Message dispatcher interface
 */
class IMessageDispatcher {
  public:
    /**
     * \brief Register a \ref IMessageHandler
     * \param[in]   message_handler Reference to the message handler
     * \return true if successfull
     */
    virtual bool RegisterHandler(IMessageHandler &message_handler) = 0;

    /**
     * \brief Poll the message source (e.g. a queue) for messages
     */
    virtual void Poll() = 0;
};

}  // namespace djetk

#endif

