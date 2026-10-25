cmake_minimum_required(VERSION 3.15)
project(parallel_lights_io)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE DEFS "src/*.def")
file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")

foreach(VARIANT ${BD_BOARD_VARIANTS})
    add_library(parallel_lights_io_${VARIANT} SHARED ${SOURCES})

    target_include_directories(parallel_lights_io_${VARIANT} PUBLIC include)
    bd_board_variant(${VARIANT} VARIANT_OUTPUT_DIR parallel_lights_io_${VARIANT})

    set_target_properties(parallel_lights_io_${VARIANT} PROPERTIES
        OUTPUT_NAME "parallel_lights_io"
        PREFIX ""
        RUNTIME_OUTPUT_DIRECTORY "${VARIANT_OUTPUT_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${VARIANT_OUTPUT_DIR}"
        ARCHIVE_OUTPUT_DIRECTORY "${VARIANT_OUTPUT_DIR}")

    # Generate test executable next to the board DLL

    add_executable(parallel_lights_io_test_${VARIANT} ${TEST_SOURCES})

    set_target_properties(parallel_lights_io_test_${VARIANT} PROPERTIES
        OUTPUT_NAME "parallel_lights_io_test"
        RUNTIME_OUTPUT_DIRECTORY "${VARIANT_OUTPUT_DIR}"
        LINK_FLAGS "-static-libgcc -static-libstdc++")
endforeach()
