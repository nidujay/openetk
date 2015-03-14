add_library(threads STATIC freertos-task-base.cpp 
    freertos-queue-task.cpp
    freertos-scheduler.cpp)

target_link_libraries(threads freertos)
target_link_libraries(threads freertos_port)

add_subdirectory(test-threads)

