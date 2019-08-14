#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus hsv_to_rgb_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    compute_hsv_to_rgb_host(srcPtr, srcSize, dstPtr, chnFormat, channel);

    return RPP_SUCCESS;
}