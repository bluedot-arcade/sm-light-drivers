cmake_minimum_required(VERSION 3.15)
project(pacdrive)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")

foreach(VARIANT ${BD_BOARD_VARIANTS})
    add_library(pacdrive_${VARIANT} SHARED ${SOURCES})

    target_include_directories(pacdrive_${VARIANT} PUBLIC include)
    bd_board_variant(${VARIANT} VARIANT_OUTPUT_DIR pacdrive_${VARIANT})

    set_target_properties(pacdrive_${VARIANT} PROPERTIES
        OUTPUT_NAME "PacDrive32"
        PREFIX ""
        RUNTIME_OUTPUT_DIRECTORY "${VARIANT_OUTPUT_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${VARIANT_OUTPUT_DIR}"
        ARCHIVE_OUTPUT_DIRECTORY "${VARIANT_OUTPUT_DIR}")

    # Generate test executable next to the board DLL

    add_executable(pacdrive_test_${VARIANT} ${TEST_SOURCES})

    set_target_properties(pacdrive_test_${VARIANT} PROPERTIES
        OUTPUT_NAME "pacdrive_test"
        RUNTIME_OUTPUT_DIRECTORY "${VARIANT_OUTPUT_DIR}"
        LINK_FLAGS "-static-libgcc -static-libstdc++")
endforeach()
