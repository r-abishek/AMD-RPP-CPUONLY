#ifndef RPPI_PIXEL_ARRANGEMENT_CONVERSIONS
#define RPPI_PIXEL_ARRANGEMENT_CONVERSIONS
 
#include "rppdefs.h"
#ifdef __cplusplus
extern "C" {
#endif


// ----------------------------------------
// Host planar_to_packed functions declaration 
// ----------------------------------------
/* Converts planar image to packed image.
param[in] srcPtr input image
*param[in] srcSize dimensions of the image
*param[out] dstPtr output image
*returns a  RppStatus enumeration. 
*retval RPP_SUCCESS : No error succesful completion
*retval RPP_ERROR : Error 
*/

RppStatus 
rppi_planar_to_packed_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_planar_to_packed_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus 
rppi_planar_to_packed_s32_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_planar_to_packed_s32_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

// ----------------------------------------
// Host packed_to_planar functions declaration 
// ----------------------------------------
/* Converts planar image to packed image.
param[in] srcPtr input image
*param[in] srcSize dimensions of the image
*param[out] dstPtr output image
*returns a  RppStatus enumeration. 
*retval RPP_SUCCESS : No error succesful completion
*retval RPP_ERROR : Error 
*/

RppStatus 
rppi_packed_to_planar_u8_pkd1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_packed_to_planar_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus 
rppi_packed_to_planar_s32_pkd1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_packed_to_planar_s32_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

#ifdef __cplusplus
}
#endif
#endif
