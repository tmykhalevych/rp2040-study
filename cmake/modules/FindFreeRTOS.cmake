set(FREERTOS_BASE_VERSION 11 CACHE STRING "FreeRTOS magor version")

fetch_dependency_git(FreeRTOS https://github.com/FreeRTOS/FreeRTOS-Kernel.git V${FREERTOS_BASE_VERSION}.1.0)
