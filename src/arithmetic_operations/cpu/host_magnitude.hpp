#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus magnitude_host(T* srcPtr1, T* srcPtr2, RppiSize srcSize, T* dstPtr,
                         RppiChnFormat chnFormat, Rpp32u channel)
{
    compute_magnitude_host(srcPtr1, srcPtr2, srcSize, dstPtr, chnFormat, channel);

    return RPP_SUCCESS;

}