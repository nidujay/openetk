/**
    \file
    \brief Timing tests main

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

extern "C"
{
#include <unity.h>
}

#include <timing/test-timing/test-freertos-tick-hook-timer.h>
#include <timing/test-timing/test-timing.h>

/**
 * \brief Timer test entry point
 */
int main()
{
    UnityBegin(__FILE__);

    // Test cases in test-timing
    RUN_TEST(test_Constructor_NoContructorErrors_RegistersToTickSource);
    RUN_TEST(test_Constructor_TickSourceRegistrationFailure_NotifiesCriticalErrorHandler);
    RUN_TEST(test_OntickFromISR_StartedTimerExpires_PostsATimeoutMessageToTheQueue);
    RUN_TEST(test_OntickFromISR_TimerNotStarted_NoMessagesArePosted);
    RUN_TEST(test_OntickFromISR_TimerExpiresAfterManyTicks_PostsATimeoutMessageToTheQueue);
    RUN_TEST(test_OntickFromISR_StartedTimerExpires_PostsATimeoutMessageToTheQueue);
    RUN_TEST(test_Start_TimeoutTranslatesToZeroTicks_PostsTimeoutMsgToQueueAfter1Tick);

    // Test cases in test-freertos-tick-hook-timer
    RUN_TEST(test_RegisterTickClient_RegisteringToFullCapacity_SuccessfulResult);
    RUN_TEST(test_RegisterTickClient_ClientBufferFull_ReturnsFailureResult);
    RUN_TEST(test_FreeRTOSTickHook_ClientsRegistered_RegisteredClientsNotifiedOfTick);

    return UnityEnd();
}

