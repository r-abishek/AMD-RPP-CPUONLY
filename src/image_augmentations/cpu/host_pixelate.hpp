#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus pixelate_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    Rpp32u kernelSize, Rpp32u x1, Rpp32u y1, Rpp32u x2, Rpp32u y2, 
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    if (kernelSize % 2 == 0)
    {
        return RPP_ERROR;
    }

    Rpp32u bound = ((kernelSize - 1) / 2);

    //if ((RPPINRANGE(x1, bound, srcSize.width - 1 - bound) == 0) 
    //    || (RPPINRANGE(x2, bound, srcSize.width - 1 - bound) == 0) 
    //    || (RPPINRANGE(y1, bound, srcSize.height - 1 - bound) == 0) 
    //    || (RPPINRANGE(y2, bound, srcSize.height - 1 - bound) == 0))
    //{
    //    return RPP_ERROR;
    //}

    memcpy(dstPtr, srcPtr, channel * srcSize.height * srcSize.width * sizeof(T));

    Rpp32f *kernel = (Rpp32f *)calloc(kernelSize * kernelSize, sizeof(Rpp32f));

    RppiSize rppiKernelSize;
    rppiKernelSize.height = kernelSize;
    rppiKernelSize.width = kernelSize;
    generate_box_kernel_host(kernel, rppiKernelSize);

    RppiSize srcSizeMod, srcSizeSubImage;
    T *srcPtrMod, *srcPtrSubImage, *dstPtrSubImage;

    compute_subimage_location_host(srcPtr, &srcPtrSubImage, srcSize, &srcSizeSubImage, x1, y1, x2, y2, chnFormat, channel);
    compute_subimage_location_host(dstPtr, &dstPtrSubImage, srcSize, &srcSizeSubImage, x1, y1, x2, y2, chnFormat, channel);

    srcSizeMod.height = srcSizeSubImage.height + (2 * bound);
    srcSizeMod.width = srcSizeSubImage.width + (2* bound);

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        srcPtrMod = srcPtrSubImage - (bound * srcSize.width) - bound;
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        srcPtrMod = srcPtrSubImage - (bound * srcSize.width * channel) - (bound * channel);
    }

    convolve_subimage_host(srcPtrMod, srcSizeMod, dstPtrSubImage, srcSizeSubImage, srcSize, kernel, rppiKernelSize, chnFormat, channel);

    return RPP_SUCCESS;
}
