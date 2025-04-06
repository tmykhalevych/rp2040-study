set(BSP_IMPL "")

if (CONFIG_TARGET_PLATFORM STREQUAL HOST)
    set(BSP_IMPL host)
elseif(CONFIG_TARGET_PLATFORM STREQUAL PICO_RP2040)
    set(BSP_IMPL pico)
    set(PICO_BOARD pico_w)
    set(PICO_DEFAULT_FLOAT_IMPL pico_float_default)
    set(PICO_DEFAULT_DOUBLE_IMPL pico_double_default)

    pico_sdk_init()

    pico_find_compiler(PICO_SIZE ${PICO_GCC_TRIPLE}-size)
    set(CMAKE_SIZE_UTIL ${PICO_SIZE} CACHE FILEPATH "")
else()
    message(FATAL_ERROR "Target platform (${CONFIG_TARGET_PLATFORM}) is not supported")
endif()

file(GLOB BSP_SRCS ${BSP_IMPL}/*.cpp)

add_library(bsp STATIC ${BSP_SRCS})
target_include_directories(bsp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (CONFIG_TARGET_PLATFORM STREQUAL PICO_RP2040)
    target_link_libraries(bsp PUBLIC pico_float freertos PRIVATE pico_stdlib pico_cyw43_arch_none)
endif()

function(add_bsp_post_build_steps target artifacts_dir)
    add_custom_command(
        TARGET ${application}
        POST_BUILD
        COMMAND mkdir -p ${artifacts_dir} && cp ${CMAKE_CURRENT_BINARY_DIR}/${application}* ${artifacts_dir}
        COMMENT "Copying artifacts into '${artifacts_dir}'")

    if (CONFIG_TARGET_PLATFORM STREQUAL PICO_RP2040)
        pico_add_extra_outputs(${target})

        add_custom_command(
            TARGET ${application}
            POST_BUILD
            COMMAND ${CMAKE_SIZE_UTIL} --format=SysV ${artifacts_dir}/${application}.elf
            COMMENT "Dumping '${application}.elf' segments sizes")
    endif()
endfunction()
