extern "C"
{
#include <unity.h>
}

#include <array>
#include <threads/freertos-task-base.h>
#include <threads/freertos-queue-task.h>
#include <threads/freertos-scheduler.h>
#include <messaging/freertos-queue.h>
#include <messaging/queue-dispatcher.h>
#include <testing/critical-error-handler-stub.h>

using namespace djetk;

/**
 * \brief Test task implementation
 *
 * Purpose of this to test the following functionality
 * 1. Successful creation of a FreeRTOS task
 * 2. Execution of the task when the scheduler is started
 * 3. Successful termination of the scheduler (which terminates the task)
 *
 * The task simply sets a flag to indicate the start and stops the scheduler via
 * an injected interface
 */
class TaskImplementation : public FreeRTOSTaskBase {
  public:
    /**
     * \brief Task constructor
     * \param[in]   error_handler   Reference to the error handler (observer)
     * \param[in]   scheduler       Reference to the FreeRTOS scheduler
     */
    TaskImplementation(ICriticalErrorHandler &error_handler, FreeRTOSScheduler &scheduler)
    : FreeRTOSTaskBase(error_handler, reinterpret_cast<const signed char *>("TEST"),
            100, tskIDLE_PRIORITY + 1),
    started(false),
    scheduler_(scheduler)
    {
    }

    /**
     * \brief Flag to indicate that the task had started
     */
    bool started;

 private:
    virtual void TaskMain()
    {
        started = true;
        scheduler_.Stop();
    }

    FreeRTOSScheduler &scheduler_;
};

void test_ThreadEntryInvocation()
{
    auto &scheduler = FreeRTOSScheduler::GetScheduler();

    // Create the test task. Once the scheduler starts, this task begins
    // executing.
    CriticalErrorHandlerStub error_handler;
    TaskImplementation task(error_handler, scheduler);

    // The task should start when we start the scheduler
    scheduler.Start();

    // If we get here, there are one of 2 reasons:
    // 1. The scheduler never start in which case the task wouldn't have
    // 2. The task started and stopped the scheduler which is what we want

    TEST_ASSERT_EQUAL(true, task.started);
}

static constexpr auto kProducerPriority = tskIDLE_PRIORITY + 2;
static constexpr auto kConsumerPriority = tskIDLE_PRIORITY + 1;
static constexpr size_t kDataSetSize = 8;
typedef std::array<int,kDataSetSize> TestDataSet;

/**
 * \brief Data producer task
 *
 * This task generates an injected dataset to an injected message queue interface
 * afterwards it suspends itself
 */
class TestProducerTask : public FreeRTOSTaskBase {
  public:
    /**
     * \brief Class constructor
     * \param[in]   error_handler   Reference to critical error handler
     * \param[in]   data_set        Data set to send via message queue
     * \param[in]   message_queue   Message queue to write to
     */
    TestProducerTask(ICriticalErrorHandler &error_handler, const TestDataSet& data_set,
            IMessageQueue &message_queue)
    : FreeRTOSTaskBase(error_handler, reinterpret_cast<const signed char *>("PRODUCER"),
            100, kProducerPriority),
    data_set_(data_set),
    message_queue_(message_queue)
    {
    }

 private:
    virtual void TaskMain()
    {
        for (auto &data : data_set_) {
            Message msg(0, &data);
            message_queue_.PostMessage(msg, 10000); // TODO (wait forever)
        }

        // Now our job is done, so suspend forever
        Suspend();
    }

    const TestDataSet &data_set_;
    IMessageQueue &message_queue_;
};

/**
 * \brief Message handler of implementing a queue consumer
 *
 * - Used to test user-space FreeRTOS queue operations
 * - Fills an injected data set array with values contained in incoming messages
 * - Stops the scheduler when the data set is completely filled
 */
class ConsumerMessageHandler : public IMessageHandler {
  public:
    /**
     * \brief Constructor of message handler object
     * \param[in]   data_set    Array to write the data pulled from the queue
     * \param[in]   scheduler   Reference to the FreeRTOS scheduler.
     */
    ConsumerMessageHandler(TestDataSet &data_set, FreeRTOSScheduler &scheduler)
        : data_set_(data_set),
          next_data_(data_set_.begin()),
          scheduler_(scheduler)
    {
    }

    /**
     * \brief See \ref djetk::IMessageHandler::HandleMessage
     */
    virtual bool HandleMessage(const Message &msg) override
    {
        // Fill the data set with the incoming messages
        // The payload points to data that we're after. Copy it to
        // the data set
        auto data = static_cast<const int*>(msg.payload.pdata);
        *next_data_ = *data;
        next_data_++;

        // Once we're done, we'll stop the scheduler to return to the
        // test that created us.
        if (next_data_ == data_set_.end()) {
            // All done. Stop the scheduler
            scheduler_.Stop();
            return false;
        }
        return true;
    }

  private:
    TestDataSet &data_set_;
    TestDataSet::iterator next_data_;
    FreeRTOSScheduler &scheduler_;
};

/**
 * \brief Integration test of FreeRTOSQueue, FreeRTOSQueueTask, QueueDispatcher
 *          and IMessageHandler
 *
 * This is a producer-consumer test. The producer task pushes a dataset to
 * a queue which is read by a QueueDispatcher. The QueueDispatcher invokes
 * a message handler (the consumer). The QueueDispatcher is polled by a
 * FreeRTOSQueueTask. This particular test exercises the user space operations
 * of the FreeRTOSQueue task.
 */
void test_ThreadSpaceQueueSendAndReceive()
{
    auto &scheduler = FreeRTOSScheduler::GetScheduler();

    static constexpr TestDataSet kProducerData{{
        1, 4, -3, 8, 2, 9, 11, -15
    }};

    // Create the Producer and consumer tasks. They won't execute until the
    // scheduler is started
    CriticalErrorHandlerStub error_handler;

    // Make the queue size smaller than the data set so that we get
    // some context switches as well.
    static constexpr size_t queue_size = kProducerData.size() / 2;
    static_assert(queue_size != 0, "Data array too small");
    FreeRTOSQueue queue(queue_size, error_handler);
    TestProducerTask producer(error_handler, kProducerData, queue);

    // The consumer pulls data out of the queue through a dispatcher
    QueueDispatcher dispatcher(queue);
    FreeRTOSQueueTask consumer(error_handler,
            reinterpret_cast<const signed char *>("CONSUMER"),
            100, kConsumerPriority, dispatcher);

    TestDataSet consumer_data;
    ConsumerMessageHandler message_handler(consumer_data, scheduler);
    dispatcher.RegisterHandler(message_handler);

    // Start the tasks. Once the Consumer has filled the whole array, it will
    // stop the scheduler.
    scheduler.Start();

    // Check that the consumer has correctly received all the data
    TEST_ASSERT(kProducerData == consumer_data);
}

extern "C" void vApplicationTickHook()
{
}

int main()
{
    UnityBegin(__FILE__);
    // FreeRTOS posix appears to have a problem with stopping and starting the
    // scheduler so there's only one test enabled at the moment
//    RUN_TEST(test_ThreadEntryInvocation);
    RUN_TEST(test_ThreadSpaceQueueSendAndReceive);
    return UnityEnd();
}

