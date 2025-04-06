set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "")

add_library(freertos_config INTERFACE)
target_include_directories(freertos_config INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(freertos_config INTERFACE projCOVERAGE_TEST=0)

add_library(freertos INTERFACE)
target_sources(freertos INTERFACE tracer.cpp)
target_include_directories(freertos INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(freertos INTERFACE common bsp)

if (CONFIG_TARGET_PLATFORM STREQUAL HOST)
    set(FREERTOS_HEAP "3" CACHE STRING "")
    set(FREERTOS_PORT "GCC_POSIX" CACHE STRING "")

    add_subdirectory(${FREERTOS_PATH} freertos)
    target_link_libraries(freertos INTERFACE freertos_kernel)
else()
    add_subdirectory(${FREERTOS_PATH}/portable/ThirdParty/GCC/RP2040 freertos)
    target_link_libraries(freertos INTERFACE FreeRTOS-Kernel FreeRTOS-Kernel-Heap4)
endif()

if (CONFIG_INCLUDE_SYSVIEW)
    if (CONFIG_TARGET_PLATFORM STREQUAL HOST)
        message(FATAL_ERROR "The build is misconfigured: cannot enable CONFIG_INCLUDE_SYSVIEW for HOST platform")
    endif()

    message(STATUS "FreeRTOS will be compiled TOGETHER with Segger SystemView")
    add_subdirectory(sysview)
    file(GLOB SYSVIEW_SRC_FILES "${SYSVIEW_SRC_DIR}/*.c")
    target_sources(freertos INTERFACE ${SYSVIEW_SRC_FILES})
    target_include_directories(freertos INTERFACE ${SYSVIEW_SRC_DIR})
endif()
