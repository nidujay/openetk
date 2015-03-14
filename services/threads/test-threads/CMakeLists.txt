add_executable(test-threads test-threads.cpp)

include_directories(${UNITY_SOURCE_DIR})
target_link_libraries(test-threads threads messaging unity)

add_test(test-threads test-threads)
