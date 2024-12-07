cmake_minimum_required(VERSION 3.15)
project(PacDrive)

add_subdirectory(libs/hidapi)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(PacDrive SHARED ${SOURCES})

target_include_directories(PacDrive PUBLIC src)

target_link_libraries(PacDrive PRIVATE hidapi::hidapi)
set_target_properties(PacDrive PROPERTIES
    LINK_FLAGS "-static-libgcc -static-libstdc++")

target_compile_options(PacDrive PRIVATE -fvisibility=hidden)

set_target_properties(PacDrive PROPERTIES OUTPUT_NAME "PacDrive32")
set_target_properties(PacDrive PROPERTIES PREFIX "")

# Generate test executable

file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")
add_executable(PacDriveTest ${TEST_SOURCES})

set_target_properties(PacDriveTest PROPERTIES
    LINK_FLAGS "-static-libgcc -static-libstdc++")



