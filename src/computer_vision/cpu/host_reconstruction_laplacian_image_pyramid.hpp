#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus reconstruction_laplacian_image_pyramid_host(T* srcPtr1, RppiSize srcSize1, T* srcPtr2, RppiSize srcSize2, T* dstPtr, 
                                                      Rpp32f stdDev, Rpp32u kernelSize, 
                                                      RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtr2Upsampled = (T *)calloc(srcSize1.height * srcSize1.width * channel, sizeof(T));
    
    resize_kernel_host(srcPtr2, srcSize2, srcPtr2Upsampled, srcSize1, chnFormat, channel);

    if (kernelSize % 2 == 0)
    {
        return RPP_ERROR;
    }
    Rpp32f *kernel = (Rpp32f *)calloc(kernelSize * kernelSize, sizeof(Rpp32f));
    int bound = ((kernelSize - 1) / 2);

    generate_gaussian_kernel_host(stdDev, kernel, kernelSize);

    RppiSize srcSize1Mod;
    srcSize1Mod.width = srcSize1.width + (2 * bound);
    srcSize1Mod.height = srcSize1.height + (2 * bound);
    T *srcPtr2UpsampledMod = (T *)calloc(srcSize1Mod.height * srcSize1Mod.width * channel, sizeof(T));

    generate_evenly_padded_image_host(srcPtr2Upsampled, srcSize1, srcPtr2UpsampledMod, srcSize1Mod, chnFormat, channel);
    
    RppiSize rppiKernelSize;
    rppiKernelSize.height = kernelSize;
    rppiKernelSize.width = kernelSize;
    convolve_image_host(srcPtr2UpsampledMod, srcSize1Mod, dstPtr, srcSize1, kernel, rppiKernelSize, chnFormat, channel);

    accumulate_kernel_host(dstPtr, srcPtr1, srcSize1, chnFormat, channel);

    return RPP_SUCCESS;
}