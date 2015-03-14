/**
    \file
    \brief Unit tests for timer objects

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

#ifndef TEST_TIMER_OBJECT_H
#define TEST_TIMER_OBJECT_H

/**
 * \test Test that timer object registers with an injected tick source during construction
 */
void test_Constructor_NoContructorErrors_RegistersToTickSource();


/**
 * \test Test that registration failure in timer object construction notifies error handler
 */
void test_Constructor_TickSourceRegistrationFailure_NotifiesCriticalErrorHandler();

/**
 * \brief Test timeout of started timer on ticks in ISR context
 */
void test_OntickFromISR_StartedTimerExpires_PostsATimeoutMessageToTheQueue();

/**
 * \brief Test timeout events aren't generated if stopped
 */
void test_OntickFromISR_TimerNotStarted_NoMessagesArePosted();

/**
 * \brief Test that the timer expires after the expected number of ticks
 */
void test_OntickFromISR_TimerExpiresAfterManyTicks_PostsATimeoutMessageToTheQueue();

/**
 * \brief Test that minimum timeout value resolves to 1 tick
 */
void test_Start_TimeoutTranslatesToZeroTicks_PostsTimeoutMsgToQueueAfter1Tick();

#endif

