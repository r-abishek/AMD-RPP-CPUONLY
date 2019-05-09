cmake_minimum_required(VERSION 3.5)
project(amd_rpp VERSION 0.1)

add_executable(
    rppi_brightness
    src/imgaug/rppi_brightness.cpp
)

add_executable(
    rppi_contrast
    src/imgaug/rppi_contrast.cpp
)

add_executable(
    rppi_rgb2hsv
    src/imgaug/rppi_rgb2hsv.cpp
)

add_executable(
    rppi_hsv2rgb
    src/imgaug/rppi_hsv2rgb.cpp
)

add_executable(
    rppi_gaussianBlur
    src/imgaug/rppi_gaussianBlur.cpp
)

target_include_directories(rppi_brightness
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(rppi_contrast
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(rppi_rgb2hsv
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(rppi_hsv2rgb
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(rppi_gaussianBlur
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)