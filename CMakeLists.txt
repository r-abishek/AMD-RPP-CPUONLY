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

add_executable(
    rppi_absolute_difference
    src/arithmetic/rppi_absolute_difference.cpp
)
target_include_directories(rppi_absolute_difference
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
    rppi_accumulate
    src/arithmetic/rppi_accumulate.cpp
)
target_include_directories(rppi_accumulate
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
    rppi_accumulate_weighted
    src/arithmetic/rppi_accumulate_weighted.cpp
)
target_include_directories(rppi_accumulate_weighted
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
    rppi_add
    src/arithmetic/rppi_add.cpp
)
target_include_directories(rppi_add
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
    rppi_sub
    src/arithmetic/rppi_sub.cpp
)
target_include_directories(rppi_sub
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
    rppi_and
    src/arithmetic/rppi_and.cpp
)
target_include_directories(rppi_and
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
    rppi_or
    src/arithmetic/rppi_or.cpp
)
target_include_directories(rppi_or
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
    rppi_xor
    src/arithmetic/rppi_xor.cpp
)
target_include_directories(rppi_xor
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
    rppi_not
    src/arithmetic/rppi_not.cpp
)
target_include_directories(rppi_not
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)