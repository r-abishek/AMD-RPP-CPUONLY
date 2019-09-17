#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus exclusive_OR_host(T* srcPtr1, U* srcPtr2, RppiSize srcSize, T* dstPtr,
                            RppiChnFormat chnFormat, Rpp32u channel)
{
    compute_exclusive_OR_host(srcPtr1, srcPtr2, srcSize, dstPtr, channel);

    return RPP_SUCCESS;

}