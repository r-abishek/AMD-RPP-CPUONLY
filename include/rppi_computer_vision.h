#ifndef RPPI_COMPUTER_VISION
#define RPPI_COMPUTER_VISION
 
#include "rppdefs.h"
#ifdef __cplusplus
extern "C" {
#endif


// ----------------------------------------
// Host local_binary_pattern functions declaration 
// ----------------------------------------

RppStatus
rppi_local_binary_pattern_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_local_binary_pattern_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_local_binary_pattern_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);


// ----------------------------------------
// Host data_object_copy functions declaration 
// ----------------------------------------

RppStatus
rppi_data_object_copy_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_data_object_copy_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_data_object_copy_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);


// ----------------------------------------
// Host gaussian_image_pyramid functions declaration 
// ----------------------------------------

RppStatus
rppi_gaussian_image_pyramid_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
                          Rpp32f stdDev, Rpp32u kernelSize);

RppStatus
rppi_gaussian_image_pyramid_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
                          Rpp32f stdDev, Rpp32u kernelSize);

RppStatus
rppi_gaussian_image_pyramid_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
                          Rpp32f stdDev, Rpp32u kernelSize);


// ----------------------------------------
// Host laplacian_image_pyramid functions declaration
// ----------------------------------------

RppStatus
rppi_laplacian_image_pyramid_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
                          Rpp32f stdDev, Rpp32u kernelSize);

RppStatus
rppi_laplacian_image_pyramid_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
                          Rpp32f stdDev, Rpp32u kernelSize);

RppStatus
rppi_laplacian_image_pyramid_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
                          Rpp32f stdDev, Rpp32u kernelSize);


// ----------------------------------------
// Host canny_edge_detector functions declaration
// ----------------------------------------

RppStatus
rppi_canny_edge_detector_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, Rpp32u max, Rpp32u min);

RppStatus
rppi_canny_edge_detector_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, Rpp32u max, Rpp32u min);

RppStatus
rppi_canny_edge_detector_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, Rpp32u max, Rpp32u min);


// ----------------------------------------
// Host convert_bit_depth functions declaration
// ----------------------------------------

RppStatus
rppi_convert_bit_depth_u8s8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_convert_bit_depth_u8s8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_convert_bit_depth_u8s8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);



RppStatus
rppi_convert_bit_depth_u8u16_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_convert_bit_depth_u8u16_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_convert_bit_depth_u8u16_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);



RppStatus
rppi_convert_bit_depth_u8s16_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_convert_bit_depth_u8s16_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_convert_bit_depth_u8s16_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);


// ----------------------------------------
// Host harris_corners_detector functions declaration
// ----------------------------------------

RppStatus
rppi_harris_corner_detector_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, 
                                         Rpp32u gaussianKernelSize, Rpp32f stdDev, 
                                         Rpp32u kernelSize, Rpp32f kValue, Rpp32f threshold, 
                                         Rpp32u nonmaxKernelSize);

RppStatus
rppi_harris_corner_detector_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, 
                                         Rpp32u gaussianKernelSize, Rpp32f stdDev, 
                                         Rpp32u kernelSize, Rpp32f kValue, Rpp32f threshold, 
                                         Rpp32u nonmaxKernelSize);

RppStatus
rppi_harris_corner_detector_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, 
                                         Rpp32u gaussianKernelSize, Rpp32f stdDev, 
                                         Rpp32u kernelSize, Rpp32f kValue, Rpp32f threshold, 
                                         Rpp32u nonmaxKernelSize);


// ----------------------------------------
// Host tensor_convert_bit_depth functions declaration
// ----------------------------------------

RppStatus
rppi_tensor_convert_bit_depth_u8s8_host(RppPtr_t srcPtr, RppPtr_t dstPtr, 
                                        Rpp32u tensorDimension, RppPtr_t tensorDimensionValues);

RppStatus
rppi_tensor_convert_bit_depth_u8u16_host(RppPtr_t srcPtr, RppPtr_t dstPtr, 
                                        Rpp32u tensorDimension, RppPtr_t tensorDimensionValues);

RppStatus
rppi_tensor_convert_bit_depth_u8s16_host(RppPtr_t srcPtr, RppPtr_t dstPtr, 
                                        Rpp32u tensorDimension, RppPtr_t tensorDimensionValues);


// ----------------------------------------
// Host match_template functions declaration
// ----------------------------------------

RppStatus
rppi_match_template_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize, 
                                 RppPtr_t templateImage, RppiSize templateImageSize);

RppStatus
rppi_match_template_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize, 
                                 RppPtr_t templateImage, RppiSize templateImageSize);

RppStatus
rppi_match_template_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize, 
                                 RppPtr_t templateImage, RppiSize templateImageSize);


#ifdef __cplusplus
}
#endif
#endif
