cmake_minimum_required(VERSION 3.5)
project(amd_rpp VERSION 0.1)

add_executable(
    rppi_brightness_contrast
    src/imgaug/rppi_brightness_contrast.cpp
)

target_include_directories(rppi_brightness_contrast
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
