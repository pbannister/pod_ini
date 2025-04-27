
set(
    INCLUDE_POD
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include_directories(
    ${INCLUDE_POD}
)

add_library(
    LIB_POD_RACER
    base/strings.cpp
    base/hash.cpp
    pod/pod-racer.cpp 
)

add_executable(
    POD_RACER
    tools/reader.cpp    
)
target_link_libraries(
    POD_RACER
    LIB_POD_RACER
)
set_target_properties(
    POD_RACER
    PROPERTIES OUTPUT_NAME pod_racer
)
