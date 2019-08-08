#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus opposite_edge_pad_image_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize, Rpp32u padType, 
                                     RppiChnFormat chnFormat, Rpp32u channel)
{
    generate_opposite_edge_padded_image_host(srcPtr, srcSize, dstPtr, dstSize, padType, chnFormat, channel);
    
    return RPP_SUCCESS;
    
}