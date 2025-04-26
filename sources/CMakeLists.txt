
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
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
