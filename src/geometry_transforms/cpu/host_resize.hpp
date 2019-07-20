#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus resize_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           RppiChnFormat chnFormat, Rpp32u channel)
{
    resize_kernel_host(srcPtr, srcSize, dstPtr, dstSize, chnFormat, channel);

    return RPP_SUCCESS;
}