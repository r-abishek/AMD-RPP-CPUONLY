cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
FIND_PACKAGE(OpenCV REQUIRED)
project(amd_rpp VERSION 0.1)

add_executable(
    rppi_nonlinear_filter
    src/filter_operations/rppi_nonlinear_filter.cpp
)
target_include_directories(rppi_nonlinear_filter
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_nonlinear_filter
    ${OpenCV_LIBS}
)

add_executable(
    rppi_tensor_convert_bit_depth
    src/computer_vision/rppi_tensor_convert_bit_depth.cpp
)
target_include_directories(rppi_tensor_convert_bit_depth
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_tensor_convert_bit_depth
    ${OpenCV_LIBS}
)

add_executable(
    rppi_fish_eye_effect
    src/image_augmentations/rppi_fish_eye_effect.cpp
)
target_include_directories(rppi_fish_eye_effect
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_fish_eye_effect
    ${OpenCV_LIBS}
)

add_executable(
    rppi_tensor_look_up_table
    src/color_model_conversions/rppi_tensor_look_up_table.cpp
)
target_include_directories(rppi_tensor_look_up_table
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_tensor_look_up_table
    ${OpenCV_LIBS}
)

add_executable(
    rppi_remap
    src/geometry_transforms/rppi_remap.cpp
)
target_include_directories(rppi_remap
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_remap
    ${OpenCV_LIBS}
)


#[[
add_executable(
    rppi_absolute_difference
    src/arithmetic_operations/rppi_absolute_difference.cpp
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
    rppi_accumulate_weighted
    src/arithmetic_operations/rppi_accumulate_weighted.cpp
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
    rppi_accumulate
    src/arithmetic_operations/rppi_accumulate.cpp
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
    rppi_accumulate_squared
    src/arithmetic_operations/rppi_accumulate_squared.cpp
)
target_include_directories(rppi_accumulate_squared
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_accumulate_squared
    ${OpenCV_LIBS}
)

add_executable(
    rppi_add
    src/arithmetic_operations/rppi_add.cpp
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
    src/arithmetic_operations/rppi_subtract.cpp
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
    rppi_magnitude
    src/arithmetic_operations/rppi_magnitude.cpp
)
target_include_directories(rppi_magnitude
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_magnitude
    ${OpenCV_LIBS}
)

add_executable(
    rppi_multiply
    src/arithmetic_operations/rppi_multiply.cpp
)
target_include_directories(rppi_multiply
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_multiply
    ${OpenCV_LIBS}
)

add_executable(
    rppi_phase
    src/arithmetic_operations/rppi_phase.cpp
)
target_include_directories(rppi_phase
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_phase
    ${OpenCV_LIBS}
)

add_executable(
    rppi_tensor_add
    src/arithmetic_operations/rppi_tensor_add.cpp
)
target_include_directories(rppi_tensor_add
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_tensor_add
    ${OpenCV_LIBS}
)

add_executable(
    rppi_tensor_subtract
    src/arithmetic_operations/rppi_tensor_subtract.cpp
)
target_include_directories(rppi_tensor_subtract
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_tensor_subtract
    ${OpenCV_LIBS}
)

add_executable(
    rppi_tensor_multiply
    src/arithmetic_operations/rppi_tensor_multiply.cpp
)
target_include_directories(rppi_tensor_multiply
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_tensor_multiply
    ${OpenCV_LIBS}
)










add_executable(
    rppi_bitwise_AND
    src/logical_operations/rppi_bitwise_AND.cpp
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
    rppi_bitwise_NOT
    src/logical_operations/rppi_bitwise_NOT.cpp
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

add_executable(
    rppi_exclusive_OR
    src/logical_operations/rppi_exclusive_OR.cpp
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
    rppi_inclusive_OR
    src/logical_operations/rppi_inclusive_OR.cpp
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
    rppi_box_filter
    src/filter_operations/rppi_box_filter.cpp
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
    src/filter_operations/rppi_bilateral_filter.cpp
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
    rppi_median_filter
    src/filter_operations/rppi_median_filter.cpp
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
    rppi_sobel_filter
    src/filter_operations/rppi_sobel_filter.cpp
)
target_include_directories(rppi_sobel_filter
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_sobel_filter
    ${OpenCV_LIBS}
)

add_executable(
    rppi_custom_convolution
    src/filter_operations/rppi_custom_convolution.cpp
)
target_include_directories(rppi_custom_convolution
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_custom_convolution
    ${OpenCV_LIBS}
)

add_executable(
    rppi_non_max_suppression
    src/filter_operations/rppi_non_max_suppression.cpp
)
target_include_directories(rppi_non_max_suppression
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_non_max_suppression
    ${OpenCV_LIBS}
)

add_executable(
    rppi_nonlinear_filter
    src/filter_operations/rppi_nonlinear_filter.cpp
)
target_include_directories(rppi_nonlinear_filter
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_nonlinear_filter
    ${OpenCV_LIBS}
)










add_executable(
    rppi_histogram
    src/statistical_operations/rppi_histogram.cpp
)
target_include_directories(rppi_histogram
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_histogram
    ${OpenCV_LIBS}
)

add_executable(
    rppi_equalize_histogram
    src/statistical_operations/rppi_equalize_histogram.cpp
)
target_include_directories(rppi_equalize_histogram
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_equalize_histogram
    ${OpenCV_LIBS}
)

add_executable(
    rppi_min
    src/statistical_operations/rppi_min.cpp
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
    src/statistical_operations/rppi_max.cpp
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
    src/statistical_operations/rppi_minMax.cpp
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
    src/statistical_operations/rppi_minMaxLoc.cpp
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
    src/statistical_operations/rppi_meanStdDev.cpp
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
    rppi_histogram_perChannel
    src/statistical_operations/rppi_histogram_perChannel.cpp
)
target_include_directories(rppi_histogram_perChannel
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_histogram_perChannel
    ${OpenCV_LIBS}
)

add_executable(
    rppi_equalize_histogram_perChannel
    src/statistical_operations/rppi_equalize_histogram_perChannel.cpp
)
target_include_directories(rppi_equalize_histogram_perChannel
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_equalize_histogram_perChannel
    ${OpenCV_LIBS}
)

add_executable(
    rppi_histogram_subimage_perChannel
    src/statistical_operations/rppi_histogram_subimage_perChannel.cpp
)
target_include_directories(rppi_histogram_subimage_perChannel
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_histogram_subimage_perChannel
    ${OpenCV_LIBS}
)

add_executable(
    rppi_thresholding
    src/statistical_operations/rppi_thresholding.cpp
)
target_include_directories(rppi_thresholding
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_thresholding
    ${OpenCV_LIBS}
)

add_executable(
    rppi_integral
    src/statistical_operations/rppi_integral.cpp
)
target_include_directories(rppi_integral
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_integral
    ${OpenCV_LIBS}
)










add_executable(
    rppi_flip
    src/geometry_transforms/rppi_flip.cpp
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
    rppi_resize
    src/geometry_transforms/rppi_resize.cpp
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
    src/geometry_transforms/rppi_resize_crop.cpp
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
    rppi_rotate
    src/geometry_transforms/rppi_rotate.cpp
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
    rppi_random_crop_letterbox
    src/geometry_transforms/rppi_random_crop_letterbox.cpp
)
target_include_directories(rppi_random_crop_letterbox
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_random_crop_letterbox
    ${OpenCV_LIBS}
)

add_executable(
    rppi_warp_affine
    src/geometry_transforms/rppi_warp_affine.cpp
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
    rppi_scale
    src/geometry_transforms/rppi_scale.cpp
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
    rppi_transpose
    src/geometry_transforms/rppi_transpose.cpp
)
target_include_directories(rppi_transpose
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_transpose
    ${OpenCV_LIBS}
)

add_executable(
    rppi_pad_image
    src/geometry_transforms/rppi_pad_image.cpp
)
target_include_directories(rppi_pad_image
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_pad_image
    ${OpenCV_LIBS}
)

add_executable(
    rppi_corner_pad_image
    src/geometry_transforms/rppi_corner_pad_image.cpp
)
target_include_directories(rppi_corner_pad_image
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_corner_pad_image
    ${OpenCV_LIBS}
)


add_executable(
    rppi_opposite_edge_pad_image
    src/geometry_transforms/rppi_opposite_edge_pad_image.cpp
)
target_include_directories(rppi_opposite_edge_pad_image
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_opposite_edge_pad_image
    ${OpenCV_LIBS}
)

add_executable(
    rppi_remap
    src/geometry_transforms/rppi_remap.cpp
)
target_include_directories(rppi_remap
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_remap
    ${OpenCV_LIBS}
)

add_executable(
    rppi_warp_perspective
    src/geometry_transforms/rppi_warp_perspective.cpp
)
target_include_directories(rppi_warp_perspective
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_warp_perspective
    ${OpenCV_LIBS}
)










add_executable(
    rppi_rgb_to_hsv
    src/color_model_conversions/rppi_rgb_to_hsv.cpp
)
target_include_directories(rppi_rgb_to_hsv
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_rgb_to_hsv
    ${OpenCV_LIBS}
)

add_executable(
    rppi_hsv_to_rgb
    src/color_model_conversions/rppi_hsv_to_rgb.cpp
)
target_include_directories(rppi_hsv_to_rgb
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_hsv_to_rgb
    ${OpenCV_LIBS}
)

add_executable(
    rppi_hue
    src/color_model_conversions/rppi_hue.cpp
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
    src/color_model_conversions/rppi_saturation.cpp
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
    rppi_rgb_to_hsl
    src/color_model_conversions/rppi_rgb_to_hsl.cpp
)
target_include_directories(rppi_rgb_to_hsl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_rgb_to_hsl
    ${OpenCV_LIBS}
)

add_executable(
    rppi_hsl_to_rgb
    src/color_model_conversions/rppi_hsl_to_rgb.cpp
)
target_include_directories(rppi_hsl_to_rgb
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_hsl_to_rgb
    ${OpenCV_LIBS}
)

add_executable(
    rppi_exposure
    src/color_model_conversions/rppi_exposure.cpp
)
target_include_directories(rppi_exposure
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_exposure
    ${OpenCV_LIBS}
)

add_executable(
    rppi_channel_extract
    src/color_model_conversions/rppi_channel_extract.cpp
)
target_include_directories(rppi_channel_extract
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_channel_extract
    ${OpenCV_LIBS}
)

add_executable(
    rppi_channel_combine
    src/color_model_conversions/rppi_channel_combine.cpp
)
target_include_directories(rppi_channel_combine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_channel_combine
    ${OpenCV_LIBS}
)

add_executable(
    rppi_look_up_table
    src/color_model_conversions/rppi_look_up_table.cpp
)
target_include_directories(rppi_look_up_table
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_look_up_table
    ${OpenCV_LIBS}
)

add_executable(
    rppi_tensor_look_up_table
    src/color_model_conversions/rppi_tensor_look_up_table.cpp
)
target_include_directories(rppi_tensor_look_up_table
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_tensor_look_up_table
    ${OpenCV_LIBS}
)










add_executable(
    rppi_blur
    src/image_augmentations/rppi_blur.cpp
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
    rppi_contrast
    src/image_augmentations/rppi_contrast.cpp
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
    rppi_brightness
    src/image_augmentations/rppi_brightness.cpp
)
target_include_directories(rppi_brightness
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_brightness
    ${OpenCV_LIBS}
)

add_executable(
    rppi_gamma_correction
    src/image_augmentations/rppi_gamma_correction.cpp
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

add_executable(
    rppi_color_temperature
    src/image_augmentations/rppi_color_temperature.cpp
)
target_include_directories(rppi_color_temperature
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_color_temperature
    ${OpenCV_LIBS}
)

add_executable(
    rppi_pixelate
    src/image_augmentations/rppi_pixelate.cpp
)
target_include_directories(rppi_pixelate
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_pixelate
    ${OpenCV_LIBS}
)

add_executable(
    rppi_jitter
    src/image_augmentations/rppi_jitter.cpp
)
target_include_directories(rppi_jitter
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_jitter
    ${OpenCV_LIBS}
)

add_executable(
    rppi_vignette
    src/image_augmentations/rppi_vignette.cpp
)
target_include_directories(rppi_vignette
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_vignette
    ${OpenCV_LIBS}
)

add_executable(
    rppi_fish_eye_effect
    src/image_augmentations/rppi_fish_eye_effect.cpp
)
target_include_directories(rppi_fish_eye_effect
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_fish_eye_effect
    ${OpenCV_LIBS}
)

add_executable(
    rppi_lens_correction
    src/image_augmentations/rppi_lens_correction.cpp
)
target_include_directories(rppi_lens_correction
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_lens_correction
    ${OpenCV_LIBS}
)

add_executable(
    rppi_occlusion
    src/image_augmentations/rppi_occlusion.cpp
)
target_include_directories(rppi_occlusion
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_occlusion
    ${OpenCV_LIBS}
)

add_executable(
    rppi_snow
    src/image_augmentations/rppi_snow.cpp
)
target_include_directories(rppi_snow
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_snow
    ${OpenCV_LIBS}
)

add_executable(
    rppi_random_shadow
    src/image_augmentations/rppi_random_shadow.cpp
)
target_include_directories(rppi_random_shadow
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_random_shadow
    ${OpenCV_LIBS}
)

add_executable(
    rppi_blend
    src/image_augmentations/rppi_blend.cpp
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
    rppi_crop_replace_subimage
    src/image_augmentations/rppi_crop_replace_subimage.cpp
)
target_include_directories(rppi_crop_replace_subimage
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_crop_replace_subimage
    ${OpenCV_LIBS}
)










add_executable(
    rppi_packed_to_planar
    src/pixel_arrangement_conversions/rppi_packed_to_planar.cpp
)
target_include_directories(rppi_packed_to_planar
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_packed_to_planar
    ${OpenCV_LIBS}
)

add_executable(
    rppi_planar_to_packed
    src/pixel_arrangement_conversions/rppi_planar_to_packed.cpp
)
target_include_directories(rppi_planar_to_packed
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_planar_to_packed
    ${OpenCV_LIBS}
)










add_executable(
    rppi_erode
    src/morphological_transforms/rppi_erode.cpp
)
target_include_directories(rppi_erode
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_erode
    ${OpenCV_LIBS}
)

add_executable(
    rppi_dilate
    src/morphological_transforms/rppi_dilate.cpp
)
target_include_directories(rppi_dilate
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_dilate
    ${OpenCV_LIBS}
)
]]









add_executable(
    rppi_local_binary_pattern
    src/computer_vision/rppi_local_binary_pattern.cpp
)
target_include_directories(rppi_local_binary_pattern
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_local_binary_pattern
    ${OpenCV_LIBS}
)

add_executable(
    rppi_gaussian_image_pyramid
    src/computer_vision/rppi_gaussian_image_pyramid.cpp
)
target_include_directories(rppi_gaussian_image_pyramid
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_gaussian_image_pyramid
    ${OpenCV_LIBS}
)

add_executable(
    rppi_laplacian_image_pyramid
    src/computer_vision/rppi_laplacian_image_pyramid.cpp
)
target_include_directories(rppi_laplacian_image_pyramid
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_laplacian_image_pyramid
    ${OpenCV_LIBS}
)

add_executable(
    rppi_data_object_copy
    src/computer_vision/rppi_data_object_copy.cpp
)
target_include_directories(rppi_data_object_copy
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_data_object_copy
    ${OpenCV_LIBS}
)

add_executable(
    rppi_canny_edge_detector
    src/computer_vision/rppi_canny_edge_detector.cpp
)
target_include_directories(rppi_canny_edge_detector
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_canny_edge_detector
    ${OpenCV_LIBS}
)

add_executable(
    rppi_convert_bit_depth
    src/computer_vision/rppi_convert_bit_depth.cpp
)
target_include_directories(rppi_convert_bit_depth
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_convert_bit_depth
    ${OpenCV_LIBS}
)

add_executable(
    rppi_harris_corner_detector
    src/computer_vision/rppi_harris_corner_detector.cpp
)
target_include_directories(rppi_harris_corner_detector
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_harris_corner_detector
    ${OpenCV_LIBS}
)

add_executable(
    rppi_tensor_convert_bit_depth
    src/computer_vision/rppi_tensor_convert_bit_depth.cpp
)
target_include_directories(rppi_tensor_convert_bit_depth
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
        ${OPENCV_INCLUDE_DIRS}
)
target_link_libraries(
    rppi_tensor_convert_bit_depth
    ${OpenCV_LIBS}
)
