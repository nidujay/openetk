extern "C"
{
#include <unity.h>
}

#include <timing/test-timing/test-timing.h>
#include <timing/timer-object.h>
#include <timing/itick-source.h>
#include <testing/message-queue-stub.h>
#include <testing/critical-error-handler-stub.h>
#include <testing/os-services-stub.h>

using namespace djetk;

/**
 * \brief Tick Source stub implementation
 */
class TickSourceStub : public ITickSource {
  public:
    TickSourceStub()
        : result(true),
        registered_client(nullptr),
        ms_to_ticks_result(1)
    {
    }

    virtual bool RegisterTickClient(ITickClient &client) override
    {
        registered_client = &client;
        return result;
    }

    virtual uint32_t MsToTicks(uint32_t milliseconds) override
    {
        (void)milliseconds;
        return ms_to_ticks_result;
    }

    /**
     * \privatesection Stub result/injected data
     */
    bool result;
    ITickClient *registered_client;
    uint32_t ms_to_ticks_result;
};

/**
    \brief Container class to construct the TimerObject
*/
class TestTimerObjectContainer {
  public:
    /**
     * \brief Test container constructor
     * \param[in]   tick_source Tick source constructed seperately
     * Brings up all the objects necessary for testing the timer object
     */
    TestTimerObjectContainer(TickSourceStub &tick_source)
        : timer_object(tick_source, message_queue, message_id, error_handler, os_services)
    {
    }

    /**
     * \privatesection Test container injected stubs
     */
    MessageQueueStub message_queue;
    CriticalErrorHandlerStub error_handler;
    OsServicesStub os_services;
    TimerObject timer_object;
    static constexpr uint32_t message_id = 0x1234;
};

/**
 * \test Test that timer object registers with an injected tick source during construction
 */
void test_Constructor_NoContructorErrors_RegistersToTickSource()
{
    // Construct the tested object
    TickSourceStub tick_source;
    TestTimerObjectContainer container(tick_source);
    TEST_ASSERT_EQUAL(&container.timer_object, tick_source.registered_client);
}

/**
 * \test Test that registration failure in timer object construction notifies error handler
 */
void test_Constructor_TickSourceRegistrationFailure_NotifiesCriticalErrorHandler()
{
    // Setup tick source to indicate registration failure
    TickSourceStub tick_source;
    tick_source.result = false;

    // Construct the tested object
    TestTimerObjectContainer container(tick_source);

    // Check that the error was reported
    TEST_ASSERT_EQUAL(true, container.error_handler.is_critical_error);
    TEST_ASSERT_EQUAL(ICriticalErrorHandler::service_registration_error,
            container.error_handler.last_error_code);
}

/**
 * \brief Test timeout of started timer on ticks in ISR context
 */
void test_OntickFromISR_StartedTimerExpires_PostsATimeoutMessageToTheQueue()
{
    // Construct the tested object
    TickSourceStub tick_source;
    TestTimerObjectContainer container(tick_source);

    // Start the timer which will expire in one tick. The ms value is
    // arbitrary. The timer object will get the tick source to perform
    // the conversion from ms to ticks. Thus it's the converted tick
    // count that matters
    tick_source.ms_to_ticks_result = 1;
    static constexpr uint32_t kTimeoutMs = 100;
    static constexpr int kReference = 23;
    container.timer_object.Start(kTimeoutMs, kReference);

    // Generate a tick event
    bool task_woken;
    tick_source.registered_client->OnTickFromIsr(task_woken);

    // Check that the timeout message is sent to the registered queue.
    auto &posted_msg = container.message_queue.posted_msg;
    TEST_ASSERT_EQUAL(container.message_id, posted_msg.id);
    auto timeout_msg = static_cast<TimeoutMessage&>(posted_msg);
    TEST_ASSERT_EQUAL(kReference, timeout_msg.GetReference());
}

/**
 * \brief Test timeout events aren't generated if stopped
 */
void test_OntickFromISR_TimerNotStarted_NoMessagesArePosted()
{
    // Construct the tested object
    TickSourceStub tick_source;
    TestTimerObjectContainer container(tick_source);

    // Start the timer which will expire in one tick. The ms value is
    // arbitrary. The timer object will get the tick source to perform
    // the conversion from ms to ticks. Thus it's the converted tick
    // count that matters
    tick_source.ms_to_ticks_result = 1;
    static constexpr uint32_t kTimeoutMs = 100;
    static constexpr int kReference = 23;
    container.timer_object.Start(kTimeoutMs, kReference);

    // Stop the timer. The next tick event shouldn't post anything to the
    // queue
    container.timer_object.Stop();

    // Generate a tick event
    bool task_woken;
    tick_source.registered_client->OnTickFromIsr(task_woken);

    // Check that the message queue is 'empty'
    TEST_ASSERT_EQUAL(0, container.message_queue.post_count);
}

/**
 * \brief Test the expiry after receiving the required number of tick events
 */
void test_OntickFromISR_TimerExpiresAfterManyTicks_PostsATimeoutMessageToTheQueue()
{
    // Construct the tested object
    TickSourceStub tick_source;
    TestTimerObjectContainer container(tick_source);

    // Start the timer which will expire after many ticks. The ms value is
    // arbitrary. The timer object will get the tick source to perform
    // the conversion from ms to ticks. Thus it's the converted tick
    // count that matters
    static constexpr uint32_t kTicksToExpiry = 10;
    tick_source.ms_to_ticks_result = kTicksToExpiry;

    static constexpr uint32_t kTimeoutMs = 100;
    static constexpr int kReference = 23;
    container.timer_object.Start(kTimeoutMs, kReference);

    // Generate tick events but not enough to expire the timer
    bool task_woken;
    for (uint32_t i = 0; i < (kTicksToExpiry - 1); i++) {
        tick_source.registered_client->OnTickFromIsr(task_woken);
    }
    // Nothing should be in the queue
    TEST_ASSERT_EQUAL(0, container.message_queue.post_count);

    // One more tick expires the timer
    tick_source.registered_client->OnTickFromIsr(task_woken);
    // Check that the timeout message is sent to the registered queue.
    TEST_ASSERT_EQUAL(1, container.message_queue.post_count);
    auto &posted_msg = container.message_queue.posted_msg;
    TEST_ASSERT_EQUAL(container.message_id, posted_msg.id);
    auto timeout_msg = static_cast<TimeoutMessage&>(posted_msg);
    TEST_ASSERT_EQUAL(kReference, timeout_msg.GetReference());
}

/**
 *  \brief Test that timer object always starts with a minimum of 1 tick
 */
void test_Start_TimeoutTranslatesToZeroTicks_PostsTimeoutMsgToQueueAfter1Tick()
{
    // Construct the tested object
    TickSourceStub tick_source;
    TestTimerObjectContainer container(tick_source);

    // Ms to Ticks conversion yields 0. But the start method should
    // round it up to 1 so that on the next tick interrupt, we get
    // a timeout message.
    //
    // The ms value is arbitrary. The timer object will get the tick
    // source to perform the conversion from ms to ticks. Thus it's
    // the converted tick count that matters
    tick_source.ms_to_ticks_result = 0;
    static constexpr uint32_t kTimeoutMs = 100;
    static constexpr int kReference = 23;
    container.timer_object.Start(kTimeoutMs, kReference);

    // Generate a tick event
    bool task_woken;
    tick_source.registered_client->OnTickFromIsr(task_woken);

    // Check that the timeout message is sent to the registered queue.
    auto &posted_msg = container.message_queue.posted_msg;
    TEST_ASSERT_EQUAL(container.message_id, posted_msg.id);
    auto timeout_msg = static_cast<TimeoutMessage&>(posted_msg);
    TEST_ASSERT_EQUAL(kReference, timeout_msg.GetReference());
}

