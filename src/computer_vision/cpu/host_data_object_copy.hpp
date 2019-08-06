#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus data_object_copy_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    compute_data_object_copy_host<Rpp8u>(srcPtr, srcSize, dstPtr, chnFormat, channel);
    
    return RPP_SUCCESS;
}
