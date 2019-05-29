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


add_executable(
    rppi_contrast
    src/imgaug/rppi_contrast.cpp
)

target_include_directories(rppi_contrast
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


add_executable(
    rppi_flip
    src/imgaug/rppi_flip.cpp
)

target_include_directories(rppi_flip
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
    rppi_blur
    src/imgaug/rppi_blur.cpp
)

target_include_directories(rppi_blur
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
