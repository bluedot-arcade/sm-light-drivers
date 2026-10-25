cmake_minimum_required(VERSION 3.15)
project(unison)

file(GLOB_RECURSE SOURCES "src/*.c")

add_library(unison STATIC ${SOURCES})

target_include_directories(unison PUBLIC include)
target_link_libraries(unison PRIVATE hidapi)
