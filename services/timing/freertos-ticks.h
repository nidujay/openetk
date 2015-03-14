#ifndef FREERTOS_TICKS_H
#define FREERTOS_TICKS_H

#include <cstdint>
#include <limits>
#include <FreeRTOS/Source/include/FreeRTOS.h>

static constexpr uint32_t infinite_ms = std::numeric_limits<uint32_t>::max();

inline portTickType ms_to_FreeRTOSTicks(uint32_t ms)
{
    static_assert(sizeof(uint32_t) == sizeof(portTickType),
        "Don't support 16 bit ticks yet");

    if (ms == infinite_ms) {
        return portMAX_DELAY;
    }

    // portTICK_RATE_MS gives the number of milliseconds per clock tick
    // Therefore ms milliseconds gives us ms / portTICK_RATE_MS ticls;
    return ms / portTICK_RATE_MS;
}

#endif

