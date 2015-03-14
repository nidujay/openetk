/**
    \file
    \brief FreeRTOS tick hook timer tests

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

#include <array>

extern "C"
{
#include <unity.h>
}

#include <timing/freertos-tick-hook-timer.h>
#include <timing/test-timing/test-freertos-tick-hook-timer.h>
#include <timing/timer-object.h>
#include <testing/critical-error-handler-stub.h>

using namespace djetk;

extern "C" void vApplicationTickHook();

/**
    \brief Container class to construct the FreeRTOSTickHookTimer
*/
class TestFreeRTOSTickHookTimerContainer {
  public:
    TestFreeRTOSTickHookTimerContainer()
        : timer_object_list(timer_objects.data(), timer_objects.size()),
        freertos_tick_hook_timer(timer_object_list, error_handler)
    {
    }

    /**
     * \brief Max number of possible timer clients
     */
    static constexpr size_t kTimerObjectCount = 3;
    /**
     * \brief Class under test
     */
    FreeRTOSTickHookTimer &GetTimer()
    {
        return freertos_tick_hook_timer;
    }

    /**
     * \brief Buffer where references to registered tick clients are held
     */
    std::array<ITickClient *, kTimerObjectCount> timer_objects;
  private:
    CriticalErrorHandlerStub error_handler;
    TickClientsList timer_object_list;
    FreeRTOSTickHookTimer freertos_tick_hook_timer;
};

/**
 * \brief Tick Client stub for testing purposes
 */
class TickClientStub : public ITickClient {
  public:
    TickClientStub ()
        : is_tick_notified(false)
    {}

    virtual bool OnTickFromIsr(bool &task_woken) override
    {
        task_woken = false;
        is_tick_notified = true;
        return true;
    }

    /**
     * \brief Flag indicating that a tick event was generated
     */
    bool is_tick_notified;
};

/**
 * \test Test that tick clients are able to register successfully
 * - Register the maximum possible tick clients and check that the
 *   result is successful.
 */
void test_RegisterTickClient_RegisteringToFullCapacity_SuccessfulResult()
{
    // Construct the tested object
    TestFreeRTOSTickHookTimerContainer container;

    std::array<TickClientStub, container.kTimerObjectCount> tick_clients;
    size_t i=0;
    for (auto &client : tick_clients) {
        auto result = container.GetTimer().RegisterTickClient(client);
        TEST_ASSERT(result);
        TEST_ASSERT_EQUAL(&client, container.timer_objects[i]);
        ++i;
    }
}

/**
 * \test Test that registering a timer client fails if the client buffer is full
 */
void test_RegisterTickClient_ClientBufferFull_ReturnsFailureResult()
{
    // Construct the tested object
    TestFreeRTOSTickHookTimerContainer container;

    // Register clients to full capacity
    std::array<TickClientStub, container.kTimerObjectCount> tick_clients;
    for (auto &client : tick_clients) {
        container.GetTimer().RegisterTickClient(client);
    }

    // Register an extra one
    TickClientStub client;
    auto result = container.GetTimer().RegisterTickClient(client);
    TEST_ASSERT_FALSE(result);
}

/**
 * \brief Test that registered tick clients are notified of FreeRTOS ticks
 *  - Max possible clients are registered
 *  - Tick event generated
 *  - None of the clients require a task swap
 *  - The stub client should indicate that a tick notification was generated
 */
void test_FreeRTOSTickHook_ClientsRegistered_RegisteredClientsNotifiedOfTick()
{
    // Construct the tested object
    TestFreeRTOSTickHookTimerContainer container;

    // Register clients to full capacity
    std::array<TickClientStub, container.kTimerObjectCount> tick_clients;
    for (auto &client : tick_clients) {
        container.GetTimer().RegisterTickClient(client);
    }

    // Generate the tick event
    vApplicationTickHook();

    // Check that the tick clients are notified
    for (auto &client : tick_clients) {
        TEST_ASSERT_TRUE(client.is_tick_notified);
    }
}

