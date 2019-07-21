#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus transpose_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           RppiChnFormat chnFormat, Rpp32u channel)
{
    compute_transpose_host<T>(static_cast<T*>(srcPtr), srcSize, static_cast<T*>(dstPtr), dstSize, 
                           chnFormat, channel);

    return RPP_SUCCESS;
}






















                    