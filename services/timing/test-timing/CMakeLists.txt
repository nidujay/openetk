include_directories(${UNITY_SOURCE_DIR})

add_executable(test-timing test-timing.cpp
    test-freertos-tick-hook-timer.cpp
    test-main.cpp)
target_link_libraries(test-timing timing unity)
add_test(test-timing test-timing)

