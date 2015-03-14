/**
 * \file
 * Test cases to validate the FreeRTOSQueue
 */

extern "C"
{
#include <unity.h>
}

#include <threads/freertos-task-base.h>
#include <threads/freertos-scheduler.h>
#include <testing/critical-error-handler-stub.h>
#include <messaging/freertos-queue.h>

using namespace djetk;

/**
 * \brief Test that messages can be successfully posted to a non-empty queue
 */
void test_PostMessage_EmptyQueue_SuccessfullyPostsMessage()
{
    CriticalErrorHandlerStub error_handler;
    static constexpr size_t kQueueSize = 1;
    FreeRTOSQueue queue(kQueueSize, error_handler);

    Message message(0, nullptr);
    TEST_ASSERT_TRUE(queue.PostMessage(message, 0));
}

/**
 * \brief Test that requests to post messages to a full queue fails
 */
void test_PostMessage_FullQueue_FailsWithTimeout()
{
    CriticalErrorHandlerStub error_handler;
    static constexpr size_t kQueueSize = 1;
    FreeRTOSQueue queue(kQueueSize, error_handler);

    Message message(0, nullptr);
    queue.PostMessage(message, 0);
    TEST_ASSERT_FALSE(queue.PostMessage(message, 0));
}

/**
 * \brief Test that messages can be received successfully from a non-empty queue
 */
void test_ReceiveMessage_NonEmptyQueue_SuccessfullyRetrievesMessage()
{
    CriticalErrorHandlerStub error_handler;
    static constexpr size_t kQueueSize = 1;
    FreeRTOSQueue queue(kQueueSize, error_handler);

    int dummy_data = 0;
    Message message(13, &dummy_data);
    queue.PostMessage(message, 0);

    Message result;
    TEST_ASSERT_TRUE(queue.ReceiveMessage(0, result));
    TEST_ASSERT_EQUAL(message.id, result.id);
    TEST_ASSERT_EQUAL(message.payload.pdata, result.payload.pdata);
}

/**
 * \brief Test that attempts to receive from an empty queue results in an error
 */
void test_ReceiveMessage_EmptyQueue_ReturnsFailure()
{
    CriticalErrorHandlerStub error_handler;
    static constexpr size_t kQueueSize = 1;
    FreeRTOSQueue queue(kQueueSize, error_handler);

    Message result;
    TEST_ASSERT_FALSE(queue.ReceiveMessage(0, result));
}

/**
 * \brief FreeRTOS task to run the tests from within
 *  The task invokes all the test cases define above before stopping the
 *  scheduler (thus terminating the test app)
 */
class TestRunnerTask : public FreeRTOSTaskBase {
  public:
    /**
     * \brief Construct a FreeRTOS task
     * \param[in] error_handler Error handler callback interface
     * \param[in] scheduler     Referenec to the FreeRTOS scheduler
     * Failure to allocate/start the task results in the error_handler
     * being invoked.
     */
    TestRunnerTask(ICriticalErrorHandler &error_handler, FreeRTOSScheduler &scheduler)
    : FreeRTOSTaskBase(error_handler, reinterpret_cast<const signed char *>("RUNNER"),
            100, tskIDLE_PRIORITY),
    scheduler_(scheduler)
    {
    }

 private:
    virtual void TaskMain()
    {
        RUN_TEST(test_PostMessage_EmptyQueue_SuccessfullyPostsMessage);
        RUN_TEST(test_PostMessage_FullQueue_FailsWithTimeout);
        RUN_TEST(test_ReceiveMessage_NonEmptyQueue_SuccessfullyRetrievesMessage);
        RUN_TEST(test_ReceiveMessage_EmptyQueue_ReturnsFailure);
        // TODO Add test cases to check the timeout is as expected

        scheduler_.Stop();
    }

    FreeRTOSScheduler &scheduler_;
};

/**
 * \brief Main entry point for test
 */
int main()
{
    UnityBegin(__FILE__);
    auto &scheduler = FreeRTOSScheduler::GetScheduler();

    // Create the test runner task. Once the scheduler starts, this task begins
    // executing all the test cases. In the end the runner disables the scheduler
    // bringing execution back here.
    CriticalErrorHandlerStub error_handler;
    TestRunnerTask runner(error_handler, scheduler);

    // The task should start when we start the scheduler
    scheduler.Start();

    return UnityEnd();
}
