cmake_minimum_required(VERSION 3.15)
project(lightDrivers VERSION 0.5)

# Link hidapi statically
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(libs/hidapi)
add_subdirectory(libs/unilight)
add_subdirectory(driver_pacdrive)
add_subdirectory(driver_parallel_lights_io)

