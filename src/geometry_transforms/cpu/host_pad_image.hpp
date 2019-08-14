#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus pad_image_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                                     RppiChnFormat chnFormat, Rpp32u channel)
{
    generate_evenly_padded_image_host(srcPtr, srcSize, dstPtr, dstSize, chnFormat, channel);
    
    return RPP_SUCCESS;
    
}