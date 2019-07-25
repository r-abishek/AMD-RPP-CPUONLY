#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus sobel_filter_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    Rpp32f *kernelX = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));
    Rpp32f *kernelY = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));

    generate_sobel_kernel_host(kernelX, 1);
    generate_sobel_kernel_host(kernelY, 2);

    RppiSize srcSizeMod;
    srcSizeMod.width = srcSize.width + 2;
    srcSizeMod.height = srcSize.height + 2;
    T *srcPtrModX = (T *)calloc(srcSizeMod.height * srcSizeMod.width * channel, sizeof(T));
    T *srcPtrModY = (T *)calloc(srcSizeMod.height * srcSizeMod.width * channel, sizeof(T));

    T *dstPtrX = (T *)calloc(srcSize.height * srcSize.width * channel, sizeof(T));
    T *dstPtrY = (T *)calloc(srcSize.height * srcSize.width * channel, sizeof(T));

    generate_evenly_padded_image_host(srcPtr, srcSize, srcPtrModX, srcSizeMod, chnFormat, channel);
    generate_evenly_padded_image_host(srcPtr, srcSize, srcPtrModY, srcSizeMod, chnFormat, channel);
    
    convolve_image_host(srcPtrModX, srcSizeMod, dstPtrX, srcSize, kernelX, 3, chnFormat, channel);
    convolve_image_host(srcPtrModY, srcSizeMod, dstPtrY, srcSize, kernelY, 3, chnFormat, channel);

    compute_magnitude_host(dstPtrX, dstPtrY, srcSize, dstPtr, chnFormat, channel);
    
    return RPP_SUCCESS;
}
