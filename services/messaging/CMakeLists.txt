add_library(messaging STATIC queue-dispatcher.cpp
    freertos-queue.cpp)

target_link_libraries(messaging freertos)
target_link_libraries(messaging freertos_port)

add_subdirectory(test-messaging)

