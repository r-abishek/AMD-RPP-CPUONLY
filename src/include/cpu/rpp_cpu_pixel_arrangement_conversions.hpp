#include "../src/pixel_arrangement_conversions/cpu/host_planar_to_packed.hpp"
#include "../src/pixel_arrangement_conversions/cpu/host_packed_to_planar.hpp"

RppStatus 
rppi_planar_to_packed_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    planar_to_packed_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                     1);
    return RPP_SUCCESS;
}

RppStatus
rppi_planar_to_packed_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    planar_to_packed_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                     3);
    return RPP_SUCCESS;
}

RppStatus 
rppi_packed_to_planar_u8_pkd1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    packed_to_planar_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                     1);
    return RPP_SUCCESS;
}

RppStatus
rppi_packed_to_planar_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    packed_to_planar_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                     3);
    return RPP_SUCCESS;
}





RppStatus 
rppi_planar_to_packed_s8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    planar_to_packed_host<Rpp8s>(static_cast<Rpp8s*>(srcPtr), srcSize, static_cast<Rpp8s*>(dstPtr),
                     1);
    return RPP_SUCCESS;
}

RppStatus
rppi_planar_to_packed_s8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    planar_to_packed_host<Rpp8s>(static_cast<Rpp8s*>(srcPtr), srcSize, static_cast<Rpp8s*>(dstPtr),
                     3);
    return RPP_SUCCESS;
}

RppStatus 
rppi_packed_to_planar_s8_pkd1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    packed_to_planar_host<Rpp8s>(static_cast<Rpp8s*>(srcPtr), srcSize, static_cast<Rpp8s*>(dstPtr),
                     1);
    return RPP_SUCCESS;
}

RppStatus
rppi_packed_to_planar_s8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    packed_to_planar_host<Rpp8s>(static_cast<Rpp8s*>(srcPtr), srcSize, static_cast<Rpp8s*>(dstPtr),
                     3);
    return RPP_SUCCESS;
}





RppStatus 
rppi_planar_to_packed_u16_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    planar_to_packed_host<Rpp16u>(static_cast<Rpp16u*>(srcPtr), srcSize, static_cast<Rpp16u*>(dstPtr),
                     1);
    return RPP_SUCCESS;
}

RppStatus
rppi_planar_to_packed_u16_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    planar_to_packed_host<Rpp16u>(static_cast<Rpp16u*>(srcPtr), srcSize, static_cast<Rpp16u*>(dstPtr),
                     3);
    return RPP_SUCCESS;
}

RppStatus 
rppi_packed_to_planar_u16_pkd1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    packed_to_planar_host<Rpp16u>(static_cast<Rpp16u*>(srcPtr), srcSize, static_cast<Rpp16u*>(dstPtr),
                     1);
    return RPP_SUCCESS;
}

RppStatus
rppi_packed_to_planar_u16_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    packed_to_planar_host<Rpp16u>(static_cast<Rpp16u*>(srcPtr), srcSize, static_cast<Rpp16u*>(dstPtr),
                     3);
    return RPP_SUCCESS;
}





RppStatus 
rppi_planar_to_packed_s16_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    planar_to_packed_host<Rpp16s>(static_cast<Rpp16s*>(srcPtr), srcSize, static_cast<Rpp16s*>(dstPtr),
                     1);
    return RPP_SUCCESS;
}

RppStatus
rppi_planar_to_packed_s16_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    planar_to_packed_host<Rpp16s>(static_cast<Rpp16s*>(srcPtr), srcSize, static_cast<Rpp16s*>(dstPtr),
                     3);
    return RPP_SUCCESS;
}

RppStatus 
rppi_packed_to_planar_s16_pkd1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    packed_to_planar_host<Rpp16s>(static_cast<Rpp16s*>(srcPtr), srcSize, static_cast<Rpp16s*>(dstPtr),
                     1);
    return RPP_SUCCESS;
}

RppStatus
rppi_packed_to_planar_s16_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    packed_to_planar_host<Rpp16s>(static_cast<Rpp16s*>(srcPtr), srcSize, static_cast<Rpp16s*>(dstPtr),
                     3);
    return RPP_SUCCESS;
}





RppStatus 
rppi_planar_to_packed_s32_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    planar_to_packed_host<Rpp32s>(static_cast<Rpp32s*>(srcPtr), srcSize, static_cast<Rpp32s*>(dstPtr),
                     1);
    return RPP_SUCCESS;
}

RppStatus
rppi_planar_to_packed_s32_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    planar_to_packed_host<Rpp32s>(static_cast<Rpp32s*>(srcPtr), srcSize, static_cast<Rpp32s*>(dstPtr),
                     3);
    return RPP_SUCCESS;
}

RppStatus 
rppi_packed_to_planar_s32_pkd1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    packed_to_planar_host<Rpp32s>(static_cast<Rpp32s*>(srcPtr), srcSize, static_cast<Rpp32s*>(dstPtr),
                     1);
    return RPP_SUCCESS;
}

RppStatus
rppi_packed_to_planar_s32_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    packed_to_planar_host<Rpp32s>(static_cast<Rpp32s*>(srcPtr), srcSize, static_cast<Rpp32s*>(dstPtr),
                     3);
    return RPP_SUCCESS;
}