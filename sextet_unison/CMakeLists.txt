cmake_minimum_required(VERSION 3.15)
project(sextet_unison C)

file(GLOB_RECURSE SOURCES "src/*.c")

# WIN32 builds a GUI app without a console window, it lives in the tray.
add_executable(sextet_unison WIN32 ${SOURCES})

target_link_libraries(sextet_unison PRIVATE unison shell32)

set_target_properties(sextet_unison PROPERTIES
    OUTPUT_NAME "UnisonSextet"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/unison")
