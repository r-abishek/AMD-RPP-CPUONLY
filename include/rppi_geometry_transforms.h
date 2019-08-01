#ifndef RPPI_GEOMETRY_TRANSFORMS
#define RPPI_GEOMETRY_TRANSFORMS
 
#include "rppdefs.h"
#ifdef __cplusplus
extern "C" {
#endif


// ----------------------------------------
// Host flip functions declaration 
// ----------------------------------------
/* Flips the image.
param[in] srcPtr input image
*param[in] srcSize dimensions of the image
*param[out] dstPtr output image
*param[in] flipAxis flip axis and value should range beetween 0 and 2
 0 ---> horizontal flip
 1 ---> vertical flip
 2 ---> horizontal + vertical flip
*returns a  RppStatus enumeration. 
*retval RPP_SUCCESS : No error succesful completion
*retval RPP_ERROR : Error 
*/

RppStatus
rppi_flip_u8_pln1_host(RppPtr_t srcPtr,RppiSize srcSize,RppPtr_t dstPtr,RppiAxis flipAxis);

RppStatus
rppi_flip_u8_pln3_host(RppPtr_t srcPtr,RppiSize srcSize,RppPtr_t dstPtr,RppiAxis flipAxis);

RppStatus
rppi_flip_u8_pkd3_host(RppPtr_t srcPtr,RppiSize srcSize,RppPtr_t dstPtr,RppiAxis flipAxis);

// ----------------------------------------
// Host resize functions declaration 
// ----------------------------------------
/* Resizes the input image to the destination dimension.
*param[in] srcPtr input image
*param[in] srcSize dimensions of the images
*param[out] dstPtr output image
*param[in] dstSize dimensions of the output images
*returns a  RppStatus enumeration. 
*retval RPP_SUCCESS : No error succesful completion
*retval RPP_ERROR : Error 
*/

RppStatus
rppi_resize_u8_pln1_host(RppPtr_t srcPtr,RppiSize srcSize,RppPtr_t dstPtr,RppiSize dstSize);

RppStatus
rppi_resize_u8_pln3_host(RppPtr_t srcPtr,RppiSize srcSize,RppPtr_t dstPtr,RppiSize dstSize);

RppStatus
rppi_resize_u8_pkd3_host(RppPtr_t srcPtr,RppiSize srcSize,RppPtr_t dstPtr,RppiSize dstSize);

// ----------------------------------------
// Host resize_crop functions declaration 
// ----------------------------------------
/* Crops the image to the roi area and resizes to the destination size
*param[in] srcPtr input image
*param[in] srcSize dimensions of the images
*param[out] dstPtr output image
*param[in] dstSize dimensions of the output images
*param[in] x1 x1 value of roi
*param[in] y1 y1 value of roi
*param[in] x2 x2 value of roi
*param[in] y2 y2 value of roi
*returns a  RppStatus enumeration. 
*retval RPP_SUCCESS : No error succesful completion
*retval RPP_ERROR : Error 
*/

RppStatus
rppi_resize_crop_u8_pln1_host(RppPtr_t srcPtr,RppiSize srcSize,RppPtr_t dstPtr,RppiSize dstSize,Rpp32u x1,Rpp32u y1,Rpp32u x2,Rpp32u y2);

RppStatus
rppi_resize_crop_u8_pln3_host(RppPtr_t srcPtr,RppiSize srcSize,RppPtr_t dstPtr,RppiSize dstSize,Rpp32u x1,Rpp32u y1,Rpp32u x2,Rpp32u y2);

RppStatus
rppi_resize_crop_u8_pkd3_host(RppPtr_t srcPtr,RppiSize srcSize,RppPtr_t dstPtr,RppiSize dstSize,Rpp32u x1,Rpp32u y1,Rpp32u x2,Rpp32u y2);

// ----------------------------------------
// Host rotate functions declaration 
// ----------------------------------------
/* Rotates the input image according to the angle specified
*param[in] srcPtr input image
*param[in] srcSize dimensions of the input images
*param[out] dstPtr output image where rotated image is stored
*param[in] dstSize dimensions of the output images
*param[in] angleDeg angle for rotation
*returns a  RppStatus enumeration. 
*retval RPP_SUCCESS : No error succesful completion
*retval RPP_ERROR : Error 
*/

RppStatus
rppi_rotate_u8_pln1_host(RppPtr_t srcPtr,RppiSize srcSize,RppPtr_t dstPtr,RppiSize dstSize,Rpp32f angleDeg);

RppStatus
rppi_rotate_u8_pln3_host(RppPtr_t srcPtr,RppiSize srcSize,RppPtr_t dstPtr,RppiSize dstSize,Rpp32f angleDeg);

RppStatus
rppi_rotate_u8_pkd3_host(RppPtr_t srcPtr,RppiSize srcSize,RppPtr_t dstPtr,RppiSize dstSize,Rpp32f angleDeg);

// ----------------------------------------
// Host random_crop_letterbox functions declaration 
// ----------------------------------------
/* Rotates the input image according to the angle specified
*param[in] srcPtr input image
*param[in] srcSize dimensions of the input images
*param[out] dstPtr output image
*param[in] dstSize dimensions of the output images
*param[in] x1 x1 value of roi
*param[in] y1 y1 value of roi
*param[in] x2 x2 value of roi
*param[in] y2 y2 value of roi
*returns a  RppStatus enumeration. 
*retval RPP_SUCCESS : No error succesful completion
*retval RPP_ERROR : Error 
*/

RppStatus
rppi_random_crop_letterbox_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize, 
                                        Rpp32u x1, Rpp32u y1, Rpp32u x2, Rpp32u y2);

RppStatus
rppi_random_crop_letterbox_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize, 
                                        Rpp32u x1, Rpp32u y1, Rpp32u x2, Rpp32u y2);

RppStatus
rppi_random_crop_letterbox_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize, 
                                        Rpp32u x1, Rpp32u y1, Rpp32u x2, Rpp32u y2);

// ----------------------------------------
// Host warp_affine functions declaration 
// ----------------------------------------
/* Resizes the input image to the destination dimension.
*param[in] srcPtr input image
*param[in] srcSize dimensions of the images
*param[out] dstPtr output image
*param[in] dstSize dimensions of the output images
*param[in] affine affine transformation matrix
*returns a  RppStatus enumeration. 
*retval RPP_SUCCESS : No error succesful completion
*retval RPP_ERROR : Error 
*/

RppStatus
rppi_warp_affine_output_size_host(RppiSize srcSize, RppiSize *dstSizePtr,
                                  RppPtr_t affine);

RppStatus
rppi_warp_affine_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize,
                              RppPtr_t affine);

RppStatus
rppi_warp_affine_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize,
                              RppPtr_t affine);

RppStatus
rppi_warp_affine_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize,
                              RppPtr_t affine);

// ----------------------------------------
// Host scale functions declaration 
// ----------------------------------------

RppStatus
rppi_scale_output_size_host(RppiSize srcSize, RppiSize *dstSizePtr,
                             Rpp32f percentage);

RppStatus
rppi_scale_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize,
                         Rpp32f percentage);

RppStatus
rppi_scale_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize,
                         Rpp32f percentage);

RppStatus
rppi_scale_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize,
                         Rpp32f percentage);

// ----------------------------------------
// Host transpose functions declaration 
// ----------------------------------------

RppStatus
rppi_transpose_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize);

RppStatus
rppi_transpose_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize);

RppStatus
rppi_transpose_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize);

// ----------------------------------------
// Host pad_image functions declaration 
// ----------------------------------------

RppStatus
rppi_pad_image_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize);

RppStatus
rppi_pad_image_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize);

RppStatus
rppi_pad_image_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize);
 
#ifdef __cplusplus
}
#endif
#endif
