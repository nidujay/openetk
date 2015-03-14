add_library(timing STATIC timer-object.cpp
    freertos-tick-hook-timer.cpp)

target_link_libraries(timing freertos)
target_link_libraries(timing freertos_port)

add_subdirectory(test-timing)

