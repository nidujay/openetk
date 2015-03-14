add_library(unity STATIC ${UNITY_SOURCE_DIR}/unity.c)

set(FREERTOS_CORE_DIR FreeRTOS/Source)
include_directories(${FREERTOS_CORE_DIR}/include)

add_library(freertos STATIC ${FREERTOS_CORE_DIR}/croutine.c 
    ${FREERTOS_CORE_DIR}/list.c 
    ${FREERTOS_CORE_DIR}/queue.c 
    ${FREERTOS_CORE_DIR}/tasks.c
    ${FREERTOS_CORE_DIR}/portable/MemMang/heap_3.c)

add_library(freertos_port ${FREERTOS_PORT_DIR}/port.c)

find_package (Threads)
target_link_libraries (freertos_port ${CMAKE_THREAD_LIBS_INIT})

