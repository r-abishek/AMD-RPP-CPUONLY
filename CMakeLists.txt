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
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
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
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_contrast
    ${OpenCV_LIBS}
)















add_executable(
    rppi_rgb2hsv
    src/imgaug/rppi_rgb2hsv.cpp
)
target_include_directories(rppi_rgb2hsv
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_rgb2hsv
    ${OpenCV_LIBS}
)


add_executable(
    rppi_hsv2rgb
    src/imgaug/rppi_hsv2rgb.cpp
)
target_include_directories(rppi_hsv2rgb
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_hsv2rgb
    ${OpenCV_LIBS}
)


add_executable(
    rppi_hue
    src/imgaug/rppi_hue.cpp
)
target_include_directories(rppi_hue
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_hue
    ${OpenCV_LIBS}
)


add_executable(
    rppi_saturation
    src/imgaug/rppi_saturation.cpp
)
target_include_directories(rppi_saturation
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_saturation
    ${OpenCV_LIBS}
)


add_executable(
    rppi_gamma_correction
    src/imgaug/rppi_gamma_correction.cpp
)
target_include_directories(rppi_gamma_correction
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_gamma_correction
    ${OpenCV_LIBS}
)

#[[
add_executable(
    rppi_packed2planar
    src/imgaug/rppi_packed2planar.cpp
)
target_include_directories(rppi_packed2planar
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_packed2planar
    ${OpenCV_LIBS}
)


add_executable(
    rppi_planar2packed
    src/imgaug/rppi_planar2packed.cpp
)
target_include_directories(rppi_planar2packed
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_planar2packed
    ${OpenCV_LIBS}
)
]]














add_executable(
    rppi_flip
    src/imgaug/rppi_flip.cpp
)
target_include_directories(rppi_flip
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_flip
    ${OpenCV_LIBS}
)


add_executable(
    rppi_warp_affine
    src/imgaug/rppi_warp_affine.cpp
)
target_include_directories(rppi_warp_affine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
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
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
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
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
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
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_resize
    ${OpenCV_LIBS}
)


add_executable(
    rppi_resize_crop
    src/imgaug/rppi_resize_crop.cpp
)
target_include_directories(rppi_resize_crop
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_resize_crop
    ${OpenCV_LIBS}
)















add_executable(
    rppi_median_filter
    src/imgaug/rppi_median_filter.cpp
)
target_include_directories(rppi_median_filter
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_median_filter
    ${OpenCV_LIBS}
)


add_executable(
    rppi_blur
    src/imgaug/rppi_blur.cpp
)
target_include_directories(rppi_blur
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_blur
    ${OpenCV_LIBS}
)


add_executable(
    rppi_box_filter
    src/imgaug/rppi_box_filter.cpp
)
target_include_directories(rppi_box_filter
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_box_filter
    ${OpenCV_LIBS}
)


add_executable(
    rppi_bilateral_filter
    src/arithmetic/rppi_bilateral_filter.cpp
)
target_include_directories(rppi_bilateral_filter
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_bilateral_filter
    ${OpenCV_LIBS}
)















add_executable(
    rppi_blend
    src/imgaug/rppi_blend.cpp
)
target_include_directories(rppi_blend
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_blend
    ${OpenCV_LIBS}
)















add_executable(
    rppi_absolute_difference
    src/arithmetic/rppi_absolute_difference.cpp
)
target_include_directories(rppi_absolute_difference
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_absolute_difference
    ${OpenCV_LIBS}
)


add_executable(
    rppi_accumulate
    src/arithmetic/rppi_accumulate.cpp
)
target_include_directories(rppi_accumulate
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_accumulate
    ${OpenCV_LIBS}
)


add_executable(
    rppi_accumulate_weighted
    src/arithmetic/rppi_accumulate_weighted.cpp
)
target_include_directories(rppi_accumulate_weighted
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_accumulate_weighted
    ${OpenCV_LIBS}
)


add_executable(
    rppi_add
    src/arithmetic/rppi_add.cpp
)
target_include_directories(rppi_add
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_add
    ${OpenCV_LIBS}
)


add_executable(
    rppi_subtract
    src/arithmetic/rppi_subtract.cpp
)
target_include_directories(rppi_subtract
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_subtract
    ${OpenCV_LIBS}
)


add_executable(
    rppi_bitwise_AND
    src/arithmetic/rppi_bitwise_AND.cpp
)
target_include_directories(rppi_bitwise_AND
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_bitwise_AND
    ${OpenCV_LIBS}
)


add_executable(
    rppi_inclusive_OR
    src/arithmetic/rppi_inclusive_OR.cpp
)
target_include_directories(rppi_inclusive_OR
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_inclusive_OR
    ${OpenCV_LIBS}
)


add_executable(
    rppi_exclusive_OR
    src/arithmetic/rppi_exclusive_OR.cpp
)
target_include_directories(rppi_exclusive_OR
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_exclusive_OR
    ${OpenCV_LIBS}
)


add_executable(
    rppi_bitwise_NOT
    src/arithmetic/rppi_bitwise_NOT.cpp
)
target_include_directories(rppi_bitwise_NOT
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_bitwise_NOT
    ${OpenCV_LIBS}
)














#[[
add_executable(
    rppi_min
    src/statistical/rppi_min.cpp
)
target_include_directories(rppi_min
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_min
    ${OpenCV_LIBS}
)


add_executable(
    rppi_max
    src/statistical/rppi_max.cpp
)
target_include_directories(rppi_max
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_max
    ${OpenCV_LIBS}
)


add_executable(
    rppi_minMax
    src/statistical/rppi_minMax.cpp
)
target_include_directories(rppi_minMax
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_minMax
    ${OpenCV_LIBS}
)


add_executable(
    rppi_minMaxLoc
    src/statistical/rppi_minMaxLoc.cpp
)
target_include_directories(rppi_minMaxLoc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_minMaxLoc
    ${OpenCV_LIBS}
)


add_executable(
    rppi_meanStdDev
    src/statistical/rppi_meanStdDev.cpp
)
target_include_directories(rppi_meanStdDev
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_meanStdDev
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
]]