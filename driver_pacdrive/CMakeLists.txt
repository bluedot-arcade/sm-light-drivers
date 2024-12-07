cmake_minimum_required(VERSION 3.15)
project(pacdrive)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(pacdrive SHARED ${SOURCES})

target_include_directories(pacdrive PUBLIC include)
target_link_libraries(pacdrive PRIVATE unilight)

set_target_properties(pacdrive PROPERTIES OUTPUT_NAME "PacDrive32")
set_target_properties(pacdrive PROPERTIES PREFIX "")

# Generate test executable

file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")
add_executable(pacdrive_test ${TEST_SOURCES})

set_target_properties(pacdrive_test PROPERTIES
    LINK_FLAGS "-static-libgcc -static-libstdc++")



