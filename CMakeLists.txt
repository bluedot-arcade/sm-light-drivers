cmake_minimum_required(VERSION 3.15)
project(lightDrivers VERSION 0.5)

# Link hidapi statically
set(BUILD_SHARED_LIBS OFF)

# Link the MinGW runtime statically so the DLLs do not depend on libgcc/libstdc++ DLLs
if(MINGW)
    add_link_options(-static-libgcc -static-libstdc++)
endif()

# Boards to build the drivers for. Each board variant gets its own set of DLLs
# in the <build>/<board>[/<player>] directory, e.g. build/unilight/PacDrive32.dll
# (PacDrive64.dll on 64-bit). The UNISON board drives a single pad, so it is
# built for both players: build/unison/p1 and build/unison/p2.
set(BD_BOARDS "unilight;unison" CACHE STRING "Target boards (unilight, unison)")

set(BD_BOARD_VARIANTS "")
foreach(BOARD ${BD_BOARDS})
    if(BOARD STREQUAL "unison")
        list(APPEND BD_BOARD_VARIANTS unison_p1 unison_p2)
    else()
        list(APPEND BD_BOARD_VARIANTS ${BOARD})
    endif()
endforeach()

# Sets the board variant output directory in OUTPUT_DIR_VAR and, when a target
# is passed as third argument, its board compile definitions and board library.
function(bd_board_variant VARIANT OUTPUT_DIR_VAR)
    string(REGEX MATCH "^([a-z]+)(_p([12]))?$" VARIANT_MATCH ${VARIANT})
    if(NOT VARIANT_MATCH)
        message(FATAL_ERROR "Unknown board variant: ${VARIANT}")
    endif()
    set(BOARD ${CMAKE_MATCH_1})
    set(PLAYER ${CMAKE_MATCH_3})

    if(PLAYER)
        set(${OUTPUT_DIR_VAR} "${CMAKE_BINARY_DIR}/${BOARD}/p${PLAYER}" PARENT_SCOPE)
    else()
        set(${OUTPUT_DIR_VAR} "${CMAKE_BINARY_DIR}/${BOARD}" PARENT_SCOPE)
    endif()

    if(ARGC GREATER 2)
        set(TARGET ${ARGV2})
        string(TOUPPER ${BOARD} BOARD_UPPER)
        target_compile_definitions(${TARGET} PRIVATE BD_BOARD_${BOARD_UPPER})
        if(PLAYER)
            target_compile_definitions(${TARGET} PRIVATE BD_${BOARD_UPPER}_PLAYER=${PLAYER})
        endif()
        target_link_libraries(${TARGET} PRIVATE ${BOARD})
    endif()
endfunction()

add_subdirectory(libs/hidapi)
add_subdirectory(libs/unilight)
add_subdirectory(libs/unison)
add_subdirectory(driver_pacdrive)
add_subdirectory(driver_parallel_lights_io)

