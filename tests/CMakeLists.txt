#
#   The rules are not quite right here, as we depend on generated files.
#   Generated files should not be checked in.
#

set(D_IN ${CMAKE_CURRENT_SOURCE_DIR}/in)
set(D_OUT ${CMAKE_CURRENT_SOURCE_DIR}/out)
set(IN_EXAMPLE1 in/example-big.ini)

include_directories(${PROJECT_SOURCE_DIR}/sources/include)

add_test(
    NAME POD_RACER0
    COMMAND POD_RACER -v
)
add_test(
    NAME POD_RACER1
    COMMAND POD_RACER -u ${IN_EXAMPLE1}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_test(
    NAME POD_RACER2
    COMMAND POD_RACER -s ${IN_EXAMPLE1}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_test(
    NAME POD_RACER3
    COMMAND POD_RACER -i ${IN_EXAMPLE1}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_test(
    NAME POD_RACER4
    COMMAND POD_RACER -d ${IN_EXAMPLE1}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_test(
    NAME POD_RACER5
    COMMAND POD_RACER -D ${IN_EXAMPLE1}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

if (1) 
    add_executable(
        TEST_CONFIG
        config.cpp
        out/config-reader.cpp
    )
    target_link_libraries(
        TEST_CONFIG
        LIB_POD_RACER
    )
    set_target_properties(
        TEST_CONFIG
        PROPERTIES OUTPUT_NAME test_config
    )

    add_test(
        NAME TEST_CONFIG0
        COMMAND TEST_CONFIG
    )
endif()
