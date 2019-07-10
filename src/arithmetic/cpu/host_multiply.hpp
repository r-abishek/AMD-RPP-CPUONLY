#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus multiply_host(T* srcPtr1, U* srcPtr2, RppiSize srcSize, T* dstPtr,
                                   unsigned int channel)
{
    compute_multiply_host(srcPtr1, srcPtr2, srcSize, dstPtr, channel);

    return RPP_SUCCESS;

}