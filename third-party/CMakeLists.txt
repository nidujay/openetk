# This line is needed for the ExternalProject command to work.
# It references the the Module in the cmake distro
include(ExternalProject)

add_library(unity STATIC ${UNITY_SOURCE_DIR}/unity.c)

# Download and extract the FreeRTOS core sources
set(FREERTOS_DIR    "${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS")
set(FREERTOS_URL    "http://downloads.sourceforge.net/project/freertos/FreeRTOS/V6.0.4/FreeRTOSV6.0.4.zip")
set(FREERTOS_CORE_DIR ${FREERTOS_DIR}/Source)

set(FREERTOS_SOURCES "${FREERTOS_CORE_DIR}/croutine.c"
    ${FREERTOS_CORE_DIR}/list.c 
    ${FREERTOS_CORE_DIR}/queue.c 
    ${FREERTOS_CORE_DIR}/tasks.c
    ${FREERTOS_CORE_DIR}/portable/MemMang/heap_3.c)

# Mark the FreeRTOS sources are generated because they're downloaded
# in the build stage
set_source_files_properties(${FREERTOS_SOURCES} PROPERTIES GENERATED TRUE)
ExternalProject_Add(freertos_download
    DOWNLOAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}
    SOURCE_DIR ${FREERTOS_DIR}
    URL ${FREERTOS_URL}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "")

# Define the freeRTOS core library (port will be added separately).
include_directories(${FREERTOS_CORE_DIR}/include)
add_library(freertos STATIC ${FREERTOS_SOURCES})

# The FreeRTOS library is dependent on the sources being downloaded
add_dependencies(freertos freertos_download)

# Download an extract the FreeRTOS Posix port
set(FREERTOS_POSIX_DIR      "${CMAKE_CURRENT_SOURCE_DIR}/Posix_GCC_Simulator")
set(FREERTOS_POSIX_URL      "http://interactive.freertos.org/attachments/token/r6d5gt3998niuc4/?name=Posix_GCC_Simulator_6.0.4.zip")
set(FREERTOS_POSIX_SRC_DIR  "${FREERTOS_POSIX_DIR}/FreeRTOS_Posix/FreeRTOS_Kernel/portable/GCC/Posix/")

ExternalProject_Add(freertos_posix_download
    DOWNLOAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}
    SOURCE_DIR ${FREERTOS_POSIX_DIR}
    URL ${FREERTOS_POSIX_URL}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "")

set_source_files_properties(${FREERTOS_POSIX_SRC_DIR}/port.c PROPERTIES GENERATED TRUE)
add_library(freertos_port ${FREERTOS_POSIX_SRC_DIR}/port.c)
add_dependencies(freertos freertos_download)

# The posix port requires pthreads to be available
find_package (Threads)
target_link_libraries (freertos_port ${CMAKE_THREAD_LIBS_INIT})

