#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus resize_crop_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           Rpp32u x1, Rpp32u y1, Rpp32u x2, Rpp32u y2,
                           RppiChnFormat chnFormat, unsigned int channel)
{
    resize_crop_kernel_host(srcPtr, srcSize, dstPtr, dstSize, x1, y1, x2, y2, chnFormat, channel);

    return RPP_SUCCESS;
    
}