cmake_minimum_required (VERSION 2.8)
project (djetk)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # We only have C++ source files. However, there are third party C libraries
    # we use (e.g. FreeRTOS). Using -std=c++ on those sources generate warnings.
    # So we specific compiler flags for C++ sources
    set(cc_compiler_flags "-std=c++11 -Wall -Wextra -pedantic")
endif()

if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${cc_compiler_flags}"
    CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

# TODO use -isystem option to include third party directories
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party)
include_directories(${THIRD_PARTY_DIR})

# TODO This belongs in the App area
set(FREERTOS_PORT_DIR ${THIRD_PARTY_DIR}/Posix_GCC_Simulator/FreeRTOS_Posix/FreeRTOS_Kernel/portable/GCC/Posix)
include_directories(${FREERTOS_PORT_DIR})
include_directories(${THIRD_PARTY_DIR}/Posix_GCC_Simulator/FreeRTOS_Posix)

# Create a variable to where unity is located. This is global to the
# whole framework
# TODO Use the third party only
set(UNITY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party/Unity/src)
enable_testing()

add_subdirectory(cpu)
add_subdirectory(ecu)
add_subdirectory(services)
add_subdirectory(third-party)
add_subdirectory(doc)

