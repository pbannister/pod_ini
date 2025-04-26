cmake_minimum_required(VERSION 3.20)

project(
    POD_CONFIG
    DESCRIPTION "Example reader / generator for INI and PROPERTIES files."
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

file(MAKE_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
file(MAKE_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

message("
=================================================================
CMAKE_CURRENT_LIST_FILE         : ${CMAKE_CURRENT_LIST_FILE}
CMAKE_RUNTIME_OUTPUT_DIRECTORY  : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
=================================================================
")

# Each option amps up warnings.
# Should have these on for all compiles.
# The old rccop code fares poorly as far too many warnings.
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Wpedantic)
add_compile_options(-Werror)

# Allow for debugging
add_compile_options(-g)
add_compile_options(-Og)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(
    POD_RACER
    base/strings.cpp
    base/hash.cpp
    pod/pod_racer.cpp 
    tools/reader.cpp    
)
set_target_properties(
    POD_RACER
    PROPERTIES OUTPUT_NAME pod_racer
)