#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus rgb_to_hsv_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                    RppiChnFormat chnFormat, unsigned channel)
{
    compute_rgb_to_hsv_host(srcPtr, srcSize, dstPtr, chnFormat, channel);
    
    return RPP_SUCCESS;
}