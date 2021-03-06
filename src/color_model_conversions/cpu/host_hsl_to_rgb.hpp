#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus hsl_to_rgb_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    compute_hsl_to_rgb_host(srcPtr, srcSize, dstPtr, chnFormat, channel);

    return RPP_SUCCESS;
}