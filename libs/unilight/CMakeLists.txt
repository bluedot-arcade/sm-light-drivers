cmake_minimum_required(VERSION 3.15)
project(unilight)

file(GLOB_RECURSE SOURCES "src/*.c") 

add_library(unilight STATIC ${SOURCES})

target_include_directories(unilight PUBLIC include)
target_link_libraries(unilight PRIVATE hidapi)