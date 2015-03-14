include_directories(${UNITY_SOURCE_DIR})

add_executable(test-messaging test-messaging.cpp)
target_link_libraries(test-messaging messaging unity)
add_test(test-messaging test-messaging)

add_executable(test-freertos-message-queue test-freertos-message-queue.cpp)
target_link_libraries(test-freertos-message-queue threads messaging unity)
add_test(test-freertos-message-queue test-freertos-message-queue)
