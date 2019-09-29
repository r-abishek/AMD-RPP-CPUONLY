#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus custom_convolution_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                                  Rpp32f *kernel, RppiSize rppiKernelSize, 
                                  RppiChnFormat chnFormat, Rpp32u channel)
{
    custom_convolve_image_host(srcPtr, srcSize, dstPtr, kernel, rppiKernelSize, chnFormat, channel);
    
    return RPP_SUCCESS;
}
