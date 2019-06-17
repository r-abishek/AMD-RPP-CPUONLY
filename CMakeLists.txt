cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
FIND_PACKAGE(OpenCV REQUIRED)
project(amd_rpp VERSION 0.1)


add_executable(
    rppi_brightness_contrast
    src/imgaug/rppi_brightness_contrast.cpp
)
target_include_directories(rppi_brightness_contrast
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_brightness_contrast
    ${OpenCV_LIBS}
)


add_executable(
    rppi_contrast
    src/imgaug/rppi_contrast.cpp
)
target_include_directories(rppi_contrast
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_contrast
    ${OpenCV_LIBS}
)


add_executable(
    rppi_flip
    src/imgaug/rppi_flip.cpp
)
target_include_directories(rppi_flip
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_flip
    ${OpenCV_LIBS}
)


add_executable(
    rppi_blur
    src/imgaug/rppi_blur.cpp
)
target_include_directories(rppi_blur
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_blur
    ${OpenCV_LIBS}
)


add_executable(
    rppi_rgb2hsv
    src/imgaug/rppi_rgb2hsv.cpp
)
target_include_directories(rppi_rgb2hsv
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


add_executable(
    rppi_hsv2rgb
    src/imgaug/rppi_hsv2rgb.cpp
)
target_include_directories(rppi_hsv2rgb
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


add_executable(
    rppi_hue
    src/imgaug/rppi_hue.cpp
)
target_include_directories(rppi_hue
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


add_executable(
    rppi_saturation
    src/imgaug/rppi_saturation.cpp
)
target_include_directories(rppi_saturation
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


add_executable(
    rppi_gamma_correction
    src/imgaug/rppi_gamma_correction.cpp
)
target_include_directories(rppi_gamma_correction
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


add_executable(
    rppi_warp_affine
    src/imgaug/rppi_warp_affine.cpp
)
target_include_directories(rppi_warp_affine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_warp_affine
    ${OpenCV_LIBS}
)


add_executable(
    rppi_rotate
    src/imgaug/rppi_rotate.cpp
)
target_include_directories(rppi_rotate
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_rotate
    ${OpenCV_LIBS}
)


add_executable(
    rppi_scale
    src/imgaug/rppi_scale.cpp
)
target_include_directories(rppi_scale
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_scale
    ${OpenCV_LIBS}
)


add_executable(
    rppi_resize
    src/imgaug/rppi_resize.cpp
)
target_include_directories(rppi_resize
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_resize
    ${OpenCV_LIBS}
)

add_executable(
    rppi_resizeCrop
    src/imgaug/rppi_resizeCrop.cpp
)
target_include_directories(rppi_resizeCrop
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_resizeCrop
    ${OpenCV_LIBS}
)


add_executable(
    DisplayImage
    src/DisplayImage.cpp
)
include_directories(
    ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    DisplayImage
    ${OpenCV_LIBS}
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

add_executable(
    rppi_bilateral_filter
    src/arithmetic/rppi_bilateral_filter.cpp
)
target_include_directories(rppi_bilateral_filter
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_bilateral_filter
    ${OpenCV_LIBS}
)

















add_executable(
    rppi_min
    src/statistical/rppi_min.cpp
)
target_include_directories(rppi_min
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
    rppi_max
    src/statistical/rppi_max.cpp
)
target_include_directories(rppi_max
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
    rppi_minMax
    src/statistical/rppi_minMax.cpp
)
target_include_directories(rppi_minMax
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
    rppi_minMaxLoc
    src/statistical/rppi_minMaxLoc.cpp
)
target_include_directories(rppi_minMaxLoc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
    rppi_meanStdDev
    src/statistical/rppi_meanStdDev.cpp
)
target_include_directories(rppi_meanStdDev
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
