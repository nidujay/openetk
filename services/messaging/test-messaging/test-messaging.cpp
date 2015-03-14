extern "C"
{
#include <unity.h>
}

#include <messaging/queue-dispatcher.h>
#include <testing/message-queue-stub.h>

using namespace djetk;

/**
 * \brief Message handler stub
 */
class MessageHandlerStub : public IMessageHandler {
  public:
    virtual bool HandleMessage(const Message &msg) override
    {
        msg_in = msg;
        return true;
    }

    /**
     * \brief Copy of last received message
     */
    Message msg_in;
};

/**
 * \test Test that a \ref QueueDispatcher, when polled, reads from the injected
 *     message queue and notifies the registered message handler of the retrieved
 *     message.
 */
void test_Poll_PushesMessageFromQueueToHandler()
{
    // Construct the queue dispatcher with a stubbed queue
    MessageQueueStub queue;
    QueueDispatcher dispatcher(queue);

    // Construct and register a message handler
    MessageHandlerStub handler;
    dispatcher.RegisterHandler(handler);

    // Place the next item in the queue
    handler.msg_in.id = 0;
    handler.msg_in.payload.pdata = nullptr;
    queue.msg_out.id = 12345;
    queue.msg_out.payload.pdata = &queue;

    dispatcher.Poll();

    // Ensure the handler was notified of the message pulled from the queue
    TEST_ASSERT_EQUAL(handler.msg_in.id, queue.msg_out.id);
    TEST_ASSERT_EQUAL(handler.msg_in.payload.pdata, queue.msg_out.payload.pdata);
}

int main()
{
    UnityBegin(__FILE__);
    RUN_TEST(test_Poll_PushesMessageFromQueueToHandler);
    return UnityEnd();
}

