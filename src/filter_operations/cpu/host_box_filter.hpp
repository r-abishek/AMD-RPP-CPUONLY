#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus box_filter_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    Rpp32u kernelSize,
                    RppiChnFormat chnFormat, Rpp32u channel)
// Optimized - Separable Kernel
///*
{
    if (kernelSize % 2 == 0)
    {
        return RPP_ERROR;
    }
    
    RppiSize srcSizeMod, rppiKernelSize;

    srcSizeMod.height = srcSize.height + (kernelSize - 1);
    srcSizeMod.width = srcSize.width;
    T *srcPtrMod1 = (T *)calloc(srcSizeMod.height * srcSizeMod.width * channel, sizeof(T));
    generate_opposite_edge_padded_image_host(srcPtr, srcSize, srcPtrMod1, srcSizeMod, 1, chnFormat, channel);

    rppiKernelSize.height = kernelSize;
    rppiKernelSize.width = 1;
    Rpp32f *kernel = (Rpp32f *)calloc(kernelSize, sizeof(Rpp32f));
    generate_box_kernel_host(kernel, rppiKernelSize);
    
    convolve_image_host(srcPtrMod1, srcSizeMod, dstPtr, srcSize, kernel, rppiKernelSize, chnFormat, channel);

    srcSizeMod.height = srcSize.height;
    srcSizeMod.width = srcSize.width + (kernelSize - 1);
    T *srcPtrMod2 = (T *)calloc(srcSizeMod.height * srcSizeMod.width * channel, sizeof(T));
    generate_opposite_edge_padded_image_host(dstPtr, srcSize, srcPtrMod2, srcSizeMod, 2, chnFormat, channel);

    rppiKernelSize.height = 1;
    rppiKernelSize.width = kernelSize;

    convolve_image_host(srcPtrMod2, srcSizeMod, dstPtr, srcSize, kernel, rppiKernelSize, chnFormat, channel);

    return RPP_SUCCESS;
}
//*/

// Un-optimized
/*
{
    if (kernelSize % 2 == 0)
    {
        return RPP_ERROR;
    }
    Rpp32f *kernel = (Rpp32f *)calloc(kernelSize * kernelSize, sizeof(Rpp32f));
    int bound = ((kernelSize - 1) / 2);

    RppiSize rppiKernelSize;
    rppiKernelSize.height = kernelSize;
    rppiKernelSize.width = kernelSize;
    
    generate_box_kernel_host(kernel, rppiKernelSize);

    RppiSize srcSizeMod;
    srcSizeMod.width = srcSize.width + (2 * bound);
    srcSizeMod.height = srcSize.height + (2 * bound);
    T *srcPtrMod = (T *)calloc(srcSizeMod.height * srcSizeMod.width * channel, sizeof(T));

    generate_evenly_padded_image_host(srcPtr, srcSize, srcPtrMod, srcSizeMod, chnFormat, channel);
    
    convolve_image_host(srcPtrMod, srcSizeMod, dstPtr, srcSize, kernel, rppiKernelSize, chnFormat, channel);
    
    return RPP_SUCCESS;
}
*/