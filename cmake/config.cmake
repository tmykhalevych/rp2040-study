# Project configuration
## Application
set(CONFIG_APP_NAME rp2040-study)
## Platform
set(CONFIG_TARGET_PLATFORM PICO_RP2040)
## Debug utils
set(CONFIG_INCLUDE_SYSVIEW TRUE)

if(CMAKE_BUILD_TYPE STREQUAL Host)
    set(CONFIG_INCLUDE_SYSVIEW FALSE)
    set(CONFIG_TARGET_PLATFORM HOST)
    add_compile_definitions(BUILD_FOR_HOST=1)
endif()

if (CONFIG_INCLUDE_SYSVIEW)
    add_compile_definitions(CONFIG_INCLUDE_SYSVIEW=1)
endif()

set(PREVIOUS_CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE FILEPATH "")

if ((NOT ${PREVIOUS_CMAKE_BUILD_TYPE} STREQUAL ${CMAKE_BUILD_TYPE}) AND
    ((${PREVIOUS_CMAKE_BUILD_TYPE} STREQUAL "Host") OR (${CMAKE_BUILD_TYPE} STREQUAL "Host")))
    message(FATAL_ERROR "Target platform changed, please clear CMake cache")
endif()

message(STATUS "Build configuration")
message(STATUS "  CONFIG_APP_NAME              : ${CONFIG_APP_NAME}")
message(STATUS "  CONFIG_TARGET_PLATFORM       : ${CONFIG_TARGET_PLATFORM}")
message(STATUS "  CONFIG_INCLUDE_SYSVIEW       : ${CONFIG_INCLUDE_SYSVIEW}")
