#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus thresholding_host(T* srcPtr, RppiSize srcSize, U* dstPtr, 
                                 U min, U max, 
                                 RppiChnFormat chnFormat, Rpp32u channel)
{
    compute_threshold_host(srcPtr, srcSize, dstPtr, min, max, 1, chnFormat, channel);

    return RPP_SUCCESS;

}