#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus data_object_copy_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    memcpy(dstPtr, srcPtr, srcSize.height * srcSize.width * channel * sizeof(T));
    
    return RPP_SUCCESS;
}
