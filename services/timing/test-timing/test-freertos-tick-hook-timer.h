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

#ifndef TEST_FREERTOS_TICK_HOOK_TIMER_H
#define TEST_FREERTOS_TICK_HOOK_TIMER_H

void test_RegisterTickClient_RegisteringToFullCapacity_SuccessfulResult();
void test_RegisterTickClient_ClientBufferFull_ReturnsFailureResult();
void test_FreeRTOSTickHook_ClientsRegistered_RegisteredClientsNotifiedOfTick();

#endif

