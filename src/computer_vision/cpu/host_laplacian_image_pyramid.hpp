#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus laplacian_image_pyramid_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    Rpp32f stdDev, Rpp32u kernelSize,
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    if (kernelSize % 2 == 0)
    {
        return RPP_ERROR;
    }
    Rpp32f *kernel = (Rpp32f *)calloc(kernelSize * kernelSize, sizeof(Rpp32f));
    int bound = ((kernelSize - 1) / 2);

    generate_gaussian_kernel_host(stdDev, kernel, kernelSize);

    RppiSize srcSizeMod;
    srcSizeMod.width = srcSize.width + (2 * bound);
    srcSizeMod.height = srcSize.height + (2 * bound);
    T *srcPtrMod = (T *)calloc(srcSizeMod.height * srcSizeMod.width * channel, sizeof(T));

    generate_evenly_padded_image_host(srcPtr, srcSize, srcPtrMod, srcSizeMod, chnFormat, channel);
    
    RppiSize rppiKernelSize;
    rppiKernelSize.height = kernelSize;
    rppiKernelSize.width = kernelSize;
    T *srcPtrConvolved = (T *)calloc(channel * srcSize.height * srcSize.width, sizeof(T));
    convolve_image_host(srcPtrMod, srcSizeMod, srcPtrConvolved, srcSize, kernel, rppiKernelSize, chnFormat, channel);

    RppiSize srcSize1;
    srcSize1.height = (srcSize.height + 1) / 2;
    srcSize1.width = (srcSize.width + 1) / 2;
    T *srcPtr1 = (T *)calloc(channel * srcSize1.height * srcSize1.width, sizeof(T));
    compute_downsampled_image_host(srcPtrConvolved, srcSize, srcPtr1, srcSize1, chnFormat, channel);

    RppiSize srcSize1Mod;
    srcSize1Mod.width = srcSize1.width + (2 * bound);
    srcSize1Mod.height = srcSize1.height + (2 * bound);
    T *srcPtr1Mod = (T *)calloc(srcSize1Mod.height * srcSize1Mod.width * channel, sizeof(T));

    generate_evenly_padded_image_host(srcPtr1, srcSize1, srcPtr1Mod, srcSize1Mod, chnFormat, channel);

    T *srcPtr1Convolved = (T *)calloc(channel * srcSize1.height * srcSize1.width, sizeof(T));
    convolve_image_host(srcPtr1Mod, srcSize1Mod, srcPtr1Convolved, srcSize1, kernel, rppiKernelSize, chnFormat, channel);

    compute_subtract_host(srcPtr1, srcPtr1Convolved, srcSize1, dstPtr, channel);
        
    return RPP_SUCCESS;
}
