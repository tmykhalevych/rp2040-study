set(SYSVIEW_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src CACHE STRING "Segger SystemView sources directory")

if (NOT SYSVIEW_SRC_CONFIGURED)
    message(STATUS "Configuring Segger SystemView sources")

    file(
        COPY
            ${SYSTEMVIEW_PATH}/Config/Global.h
            ${SYSTEMVIEW_PATH}/Config/SEGGER_RTT_Conf.h
            ${SYSTEMVIEW_PATH}/Config/SEGGER_SYSVIEW_Conf.h
            ${SYSTEMVIEW_PATH}/Sample/FreeRTOSV${FREERTOS_BASE_VERSION}/Config/Cortex-M/SEGGER_SYSVIEW_Config_FreeRTOS.c
            ${SYSTEMVIEW_PATH}/Sample/FreeRTOSV${FREERTOS_BASE_VERSION}/SEGGER_SYSVIEW_FreeRTOS.c
            ${SYSTEMVIEW_PATH}/Sample/FreeRTOSV${FREERTOS_BASE_VERSION}/SEGGER_SYSVIEW_FreeRTOS.h
            ${SYSTEMVIEW_PATH}/SEGGER/SEGGER.h
            ${SYSTEMVIEW_PATH}/SEGGER/SEGGER_RTT.c
            ${SYSTEMVIEW_PATH}/SEGGER/SEGGER_RTT.h
            ${SYSTEMVIEW_PATH}/SEGGER/SEGGER_SYSVIEW.c
            ${SYSTEMVIEW_PATH}/SEGGER/SEGGER_SYSVIEW.h
            ${SYSTEMVIEW_PATH}/SEGGER/SEGGER_SYSVIEW_ConfDefaults.h
            ${SYSTEMVIEW_PATH}/SEGGER/SEGGER_SYSVIEW_Int.h
        DESTINATION
            ${SYSVIEW_SRC_DIR})

        set(SYSVIEW_CONFIG ${SYSVIEW_SRC_DIR}/SEGGER_SYSVIEW_Config_FreeRTOS.c)
        file(READ ${SYSVIEW_CONFIG} FILE_CONTENTS)
            
        string(REGEX REPLACE
            "#define SYSVIEW_APP_NAME[ \t]+\"[^\"]*\""
            "#define SYSVIEW_APP_NAME \"${CONFIG_APP_NAME}\""
            FILE_CONTENTS "${FILE_CONTENTS}")
        
        string(REGEX REPLACE
            "#define SYSVIEW_DEVICE_NAME[ \t]+\"[^\"]*\""
            "#define SYSVIEW_DEVICE_NAME \"${CONFIG_TARGET_PLATFORM}\""
            FILE_CONTENTS "${FILE_CONTENTS}")

        string(REGEX REPLACE
            "#define[ \t]+SYSVIEW_TIMESTAMP_FREQ[ \t]+\\(configCPU_CLOCK_HZ\\)"
            "#define SYSVIEW_TIMESTAMP_FREQ (CONFIG_SYSVIEW_TIMESTAMP_FREQ)"
            FILE_CONTENTS "${FILE_CONTENTS}")


        string(REGEX REPLACE
            "#define[ \t]+SYSVIEW_CPU_FREQ[ \t]+configCPU_CLOCK_HZ"
            "#define SYSVIEW_CPU_FREQ CONFIG_SYSVIEW_CPU_FREQ"
            FILE_CONTENTS "${FILE_CONTENTS}")
        
        string(REGEX REPLACE
            "#define SYSVIEW_RAM_BASE[ \t]+\\([^)]*\\)"
            "#define SYSVIEW_RAM_BASE (CONFIG_SYSVIEW_RAM_BASE)"
            FILE_CONTENTS "${FILE_CONTENTS}")
        
        file(WRITE ${SYSVIEW_CONFIG} "${FILE_CONTENTS}")

    set(SYSVIEW_SRC_CONFIGURED TRUE CACHE BOOL "Segger SystemView sources are configured")
else()
    message(STATUS "Skip configuring Segger SystemView sources")
endif()
