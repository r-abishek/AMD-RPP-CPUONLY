#include "../src/imgaug/cpu/host_planar2packed.hpp"
#include "../src/imgaug/cpu/host_packed2planar.hpp"

RppStatus 
rppi_planar2packed_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    planar2packed_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                     1);
    return RPP_SUCCESS;
}

RppStatus
rppi_planar2packed_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    planar2packed_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                     3);
    return RPP_SUCCESS;
}

RppStatus 
rppi_packed2planar_u8_pkd1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    packed2planar_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                     1);
    return RPP_SUCCESS;
}

RppStatus
rppi_packed2planar_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    packed2planar_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                     3);
    return RPP_SUCCESS;
}