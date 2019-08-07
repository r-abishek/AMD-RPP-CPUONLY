#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus sobel_filter_host(T* srcPtr, RppiSize srcSize, T* dstPtr, 
                            Rpp32u sobelType, 
                            RppiChnFormat chnFormat, Rpp32u channel)
{
    RppiSize srcSizeMod;
    srcSizeMod.width = srcSize.width + 2;
    srcSizeMod.height = srcSize.height + 2;

    T *srcPtrMod = (T *)calloc(srcSizeMod.height * srcSizeMod.width * channel, sizeof(T));
    generate_evenly_padded_image_host(srcPtr, srcSize, srcPtrMod, srcSizeMod, chnFormat, channel);

    RppiSize rppiKernelSize;
    rppiKernelSize.height = 3;
    rppiKernelSize.width = 3;

    if (sobelType == 0)
    {
        Rpp32f *kernelX = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));
        generate_sobel_kernel_host(kernelX, 1);
        convolve_image_host(srcPtrMod, srcSizeMod, dstPtr, srcSize, kernelX, rppiKernelSize, chnFormat, channel);
    }
    else if (sobelType == 1)
    {
        Rpp32f *kernelY = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));
        generate_sobel_kernel_host(kernelY, 2);
        convolve_image_host(srcPtrMod, srcSizeMod, dstPtr, srcSize, kernelY, rppiKernelSize, chnFormat, channel);
    }
    else if (sobelType == 2)
    {
        Rpp32f *kernelX = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));
        generate_sobel_kernel_host(kernelX, 1);
        Rpp32s *dstPtrIntermediateX = (Rpp32s *)calloc(srcSize.height * srcSize.width * channel, sizeof(Rpp32s));
        convolve_image_host(srcPtrMod, srcSizeMod, dstPtrIntermediateX, srcSize, kernelX, rppiKernelSize, chnFormat, channel);

        Rpp32f *kernelY = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));
        generate_sobel_kernel_host(kernelY, 2);
        Rpp32s *dstPtrIntermediateY = (Rpp32s *)calloc(srcSize.height * srcSize.width * channel, sizeof(Rpp32s));
        convolve_image_host(srcPtrMod, srcSizeMod, dstPtrIntermediateY, srcSize, kernelY, rppiKernelSize, chnFormat, channel);

        compute_magnitude_host(dstPtrIntermediateX, dstPtrIntermediateY, srcSize, dstPtr, chnFormat, channel);
    }

    return RPP_SUCCESS;
}
