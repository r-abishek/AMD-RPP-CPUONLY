#ifndef RPPI_STATISTICAL_OPERATIONS_H
#define RPPI_STATISTICAL_OPERATIONS_H
 
#include "rppdefs.h"
#ifdef __cplusplus
extern "C" {
#endif


// ----------------------------------------
// Host histogram functions declaration 
// ----------------------------------------
/* Computes histogrma of image
param[in] srcPtr input image
*param[in] srcSize dimensions of the image
*param[out] outputHistogram output array of histogram
*param[in] bins number of bins
*returns a  RppStatus enumeration. 
*retval RPP_SUCCESS : No error succesful completion
*retval RPP_ERROR : Error 
*/

RppStatus
rppi_histogram_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, Rpp32u* outputHistogram, Rpp32u bins);

RppStatus
rppi_histogram_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, Rpp32u* outputHistogram, Rpp32u bins);

RppStatus
rppi_histogram_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, Rpp32u* outputHistogram, Rpp32u bins);

// ----------------------------------------
// Host equalize_histogram functions declaration 
// ----------------------------------------
/* Equalizes histogram of image
param[in] srcPtr input image
*param[in] srcSize dimensions of the image
*param[out] dstPtr output image
*param[in] newMin minimum pixel value for contrast stretch
*param[in] newMax maxium pixel value for contrast stretch
*returns a  RppStatus enumeration. 
*retval RPP_SUCCESS : No error succesful completion
*retval RPP_ERROR : Error 
*/

RppStatus
rppi_equalize_histogram_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_equalize_histogram_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_equalize_histogram_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

// ----------------------------------------
// Host min functions declaration 
// ----------------------------------------

RppStatus
rppi_min_u8_pln1_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_min_u8_pln3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_min_u8_pkd3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

// ----------------------------------------
// Host max functions declaration 
// ----------------------------------------

RppStatus
rppi_max_u8_pln1_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_max_u8_pln3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_max_u8_pkd3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

// ----------------------------------------
// Host minMax functions declaration 
// ----------------------------------------

RppStatus
rppi_minMax_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t maskPtr, Rpp8u* min, Rpp8u* max);

RppStatus
rppi_minMax_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t maskPtr, Rpp8u* min, Rpp8u* max);

RppStatus
rppi_minMax_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t maskPtr, Rpp8u* min, Rpp8u* max);

// ----------------------------------------
// Host minMaxLoc functions declaration 
// ----------------------------------------

RppStatus
rppi_minMaxLoc_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t maskPtr, Rpp8u* min, Rpp8u* max, Rpp8u** minLoc, Rpp8u** maxLoc);

RppStatus
rppi_minMaxLoc_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t maskPtr, Rpp8u* min, Rpp8u* max, Rpp8u** minLoc, Rpp8u** maxLoc);

RppStatus
rppi_minMaxLoc_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t maskPtr, Rpp8u* min, Rpp8u* max, Rpp8u** minLoc, Rpp8u** maxLoc);

// ----------------------------------------
// Host meanStdDev functions declaration 
// ----------------------------------------

RppStatus
rppi_meanStdDev_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, Rpp32f* mean, Rpp32f* stdDev);

RppStatus
rppi_meanStdDev_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, Rpp32f* mean, Rpp32f* stdDev);

RppStatus
rppi_meanStdDev_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, Rpp32f* mean, Rpp32f* stdDev);

// ----------------------------------------
// Host integral functions declaration 
// ----------------------------------------

RppStatus
rppi_integral_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_integral_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_integral_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

// ----------------------------------------
// Host histogram_perChannel functions declaration 
// ----------------------------------------

RppStatus
rppi_histogram_perChannel_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, Rpp32u* outputHistogram, Rpp32u bins);

RppStatus
rppi_histogram_perChannel_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, Rpp32u* outputHistogram, Rpp32u bins);

RppStatus
rppi_histogram_perChannel_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, Rpp32u* outputHistogram, Rpp32u bins);

// ----------------------------------------
// Host histogram_subimage_perChannel functions declaration 
// ----------------------------------------

RppStatus
rppi_histogram_subimage_perChannel_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, Rpp32u* outputHistogram, Rpp32u bins, 
                                     unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

RppStatus
rppi_histogram_subimage_perChannel_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, Rpp32u* outputHistogram, Rpp32u bins, 
                                     unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

RppStatus
rppi_histogram_subimage_perChannel_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, Rpp32u* outputHistogram, Rpp32u bins, 
                                     unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

// ----------------------------------------
// Host equalize_histogram_perChannel functions declaration 
// ----------------------------------------

RppStatus
rppi_equalize_histogram_perChannel_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_equalize_histogram_perChannel_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_equalize_histogram_perChannel_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);
 
#ifdef __cplusplus
}
#endif
#endif
