# List of examples used in documentation. They are compiled to test for
# errors.

# include all the directories required to build the examples
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../services)
add_executable(isr-service isr-service-example.cpp)
