#ifndef IMESSAGE_QUEUE_H
#define IMESSAGE_QUEUE_H

#include <cstdint>
#include "messaging/message.h"

namespace djetk {

/**
 * \brief Interface to a message queue
 */
class IMessageQueue {
  public:
    /**
     * \brief Post a message into the queue from thread context
     * \param[in] message       Reference to the message object
     * \param[in] timeout_ms    ms Timeout if the message queue is full
     * \retval true Message successfully queued
     * \retval false Timed out
     */
    virtual bool PostMessage(const Message &message, uint32_t timeout_ms) = 0;

    /**
     * \brief Post a message into the queue from ISR context
     * \param[in] message       Reference to the message object
     * \param[out]  task_woken  Indicates if a reschedule is required
     * \retval true Message successfully queued
     * \retval false Failed to post the message (queue full)
     */
    virtual bool PostMessageFromIsr(const Message &message, bool &task_woken) = 0;

    /**
     * \brief Receive a message from the queue from thread context
     * \param[in]  timeout_ms   ms Timeout if the message queue is empty
     * \param[out] message      Reference to the message object write location
     * \retval true Message successfully retrieved
     * \retval false Timed out
     */
    virtual bool ReceiveMessage(uint32_t timeout_ms, Message &message) = 0;

    /**
     * \brief Receive a message from the queue from ISR context
     * \param[out] message      Reference to the message object write location
     * \param[out] task_woken   Indicates if a reschedule is required
     * \retval true Message successfully retrieved
     * \retval false Queue empty
     */
    virtual bool ReceiveMessageFromIsr(Message &message, bool &task_woken) = 0;
};

}   // namespace djetk

#endif

