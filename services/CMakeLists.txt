# Sources in services can only include files within the services directory.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(messaging)
add_subdirectory(threads)
add_subdirectory(timing)
