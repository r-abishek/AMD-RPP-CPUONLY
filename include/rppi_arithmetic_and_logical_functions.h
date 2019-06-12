#ifndef RPPI_ARITHMATIC_AND_LOGICAL_FUNCTIONS_H
#define RPPI_ARITHMATIC_AND_LOGICAL_FUNCTIONS_H
#include "rppdefs.h"
#ifdef __cplusplus
extern "C" {
#endif



//------------------------- Basic Arithmetic -------------------------



// --------------------
// Absolute Difference
// --------------------

// Host function declarations

RppStatus
rppi_absolute_difference_u8_pln1_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_absolute_difference_u8_pln3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_absolute_difference_u8_pkd3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);



// --------------------
// Accumulate Weighted
// --------------------

// Host function declarations

RppStatus
rppi_accumulate_weighted_u8_pln1_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, Rpp32f alpha);

RppStatus
rppi_accumulate_weighted_u8_pln3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, Rpp32f alpha);

RppStatus
rppi_accumulate_weighted_u8_pkd3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, Rpp32f alpha);



// --------------------
// Accumulate
// --------------------

// Host function declarations

RppStatus
rppi_accumulate_u8_pln1_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize);

RppStatus
rppi_accumulate_u8_pln3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize);

RppStatus
rppi_accumulate_u8_pkd3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize);



// --------------------
// Add
// --------------------

// Host function declarations

RppStatus
rppi_add_u8_pln1_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_add_u8_pln3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_add_u8_pkd3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);



// --------------------
// And
// --------------------

// Host function declarations

RppStatus
rppi_and_u8_pln1_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_and_u8_pln3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_and_u8_pkd3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);



// --------------------
// Bilateral Filter
// --------------------

// Host function declarations

RppStatus
rppi_bilateral_filter_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
                             Rpp32s diameter, Rpp64f sigmaI, Rpp64f sigmaS);

RppStatus
rppi_bilateral_filter_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
                             Rpp32s diameter, Rpp64f sigmaI, Rpp64f sigmaS);

RppStatus
rppi_bilateral_filter_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
                             Rpp32s diameter, Rpp64f sigmaI, Rpp64f sigmaS);



// --------------------
// Not
// --------------------

// Host function declarations

RppStatus
rppi_not_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_not_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_not_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr);



// --------------------
// Or
// --------------------

// Host function declarations

RppStatus
rppi_or_u8_pln1_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_or_u8_pln3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_or_u8_pkd3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);



// --------------------
// Sub
// --------------------

// Host function declarations

RppStatus
rppi_sub_u8_pln1_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_sub_u8_pln3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_sub_u8_pkd3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);



// --------------------
// Xor
// --------------------

// Host function declarations

RppStatus
rppi_xor_u8_pln1_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_xor_u8_pln3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);

RppStatus
rppi_xor_u8_pkd3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr);




#ifdef __cplusplus
}
#endif
#endif /* RPPI_ARITHMATIC_AND_LOGICAL_FUNCTIONS_H */