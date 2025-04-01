set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/build/dependencies CACHE STRING "")

macro(fetch_dependency_git NAME GIT_REPO GIT_TAG)
    string(TOUPPER ${NAME} NAME_UPPER)
    set(DEP_PATH "${NAME_UPPER}_PATH")
    
    if (NOT DEFINED ${${DEP_PATH}})
        message(STATUS "${NAME_UPPER}_PATH not specified, fetching ${NAME} from ${GIT_REPO}")
        
        include(FetchContent)
        FetchContent_Declare(${NAME}
            GIT_REPOSITORY ${GIT_REPO}
            GIT_TAG ${GIT_TAG}
            GIT_PROGRESS TRUE)
        
        FetchContent_GetProperties(${NAME})
        if (NOT ${NAME}_POPULATED)
            set(FETCHCONTENT_QUIET FALSE)
            FetchContent_Populate(${NAME})
        endif()
        
        string(TOLOWER ${NAME} NAME_LOWER)
        set(${${DEP_PATH}} ${${NAME_LOWER}_SOURCE_DIR} CACHE PATH "Path to downloaded ${NAME}")
    endif()

    message(STATUS "${DEP_PATH} set to '${${DEP_PATH}}'")
endmacro()

macro(fetch_dependency_zip NAME LINK)
    string(TOUPPER ${NAME} NAME_UPPER)
    set(DEP_PATH "${NAME_UPPER}_PATH")
    set(DOWNLOAD_DIR "${CMAKE_BINARY_DIR}/dependencies/${NAME}")
    set(ZIP_FILE "${DOWNLOAD_DIR}/${NAME}.zip")
    set(EXTRACT_DIR "${DOWNLOAD_DIR}/src")

    if (NOT DEFINED ${${DEP_PATH}})
        message(STATUS "${DEP_PATH} not specified, downloading ${NAME} from ${LINK}")

        file(MAKE_DIRECTORY "${DOWNLOAD_DIR}")
        file(DOWNLOAD ${LINK} ${ZIP_FILE} SHOW_PROGRESS STATUS download_status)
        
        list(GET download_status 0 result_code)
        if(NOT result_code EQUAL 0)
            message(FATAL_ERROR "Download of ${LINK} failed with status ${result_code}")
        endif()

        execute_process(
            COMMAND unzip -o ${ZIP_FILE} -d ${EXTRACT_DIR}
            RESULT_VARIABLE unzip_result
        )

        if(NOT unzip_result EQUAL 0)
            message(FATAL_ERROR "Unzip failed with error ${unzip_result}")
        endif()

        set(${${DEP_PATH}} ${EXTRACT_DIR} CACHE PATH "Path to downloaded ${NAME}")
    endif()

    message(STATUS "${DEP_PATH} set to '${${DEP_PATH}}'")
endmacro()
