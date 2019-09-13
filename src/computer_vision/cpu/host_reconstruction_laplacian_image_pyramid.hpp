#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus reconstruction_laplacian_image_pyramid_host(T* srcPtr1, RppiSize srcSize1, T* srcPtr2, RppiSize srcSize2, T* dstPtr, 
                                                      RppiChnFormat chnFormat, Rpp32u channel)
{
    resize_kernel_host(srcPtr2, srcSize2, dstPtr, srcSize1, chnFormat, channel);

    accumulate_kernel_host(dstPtr, srcPtr1, srcSize1, chnFormat, channel);

    return RPP_SUCCESS;
}