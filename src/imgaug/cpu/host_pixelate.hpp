#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus pixelate_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    unsigned int kernelSize, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, 
                    RppiChnFormat chnFormat, unsigned int channel)
{
    if (kernelSize % 2 == 0)
    {
        return RPP_ERROR;
    }

    unsigned int bound = ((kernelSize - 1) / 2);

    if ((RPPINRANGE(x1, bound, srcSize.width - 1 - bound) == 0) 
        || (RPPINRANGE(x2, bound, srcSize.width - 1 - bound) == 0) 
        || (RPPINRANGE(y1, bound, srcSize.height - 1 - bound) == 0) 
        || (RPPINRANGE(y2, bound, srcSize.height - 1 - bound) == 0))
    {
        return RPP_ERROR;
    }



    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        *dstPtrTemp = *srcPtrTemp;
        srcPtrTemp++;
        dstPtrTemp++;
    }



    Rpp32f *kernel = (Rpp32f *)calloc(kernelSize * kernelSize, sizeof(Rpp32f));

    generate_box_kernel_host(kernel, kernelSize);


/*
    RppiSize srcSizeMod, srcSizeSubImage;
    T *srcPtrMod, *srcPtrSubImage, *dstPtrSubImage;

    int yDiff = (int) y2 - (int) y1;
    int xDiff = (int) x2 - (int) x1;

    srcSizeSubImage.height = (Rpp32u) RPPABS(yDiff) + 1;
    srcSizeSubImage.width = (Rpp32u) RPPABS(xDiff) + 1;
    srcSizeMod.height = srcSizeSubImage.height + (2 * bound);
    srcSizeMod.width = srcSizeSubImage.width + (2* bound);

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        srcPtrSubImage = srcPtr + (RPPMIN2(y1, y2) * srcSize.width) + RPPMIN2(x1, x2);
        srcPtrMod = srcPtrSubImage - (bound * srcSize.width) - bound;
        //srcPtrMod = srcPtr + ((RPPMIN2(y1, y2) - bound) * srcSize.width) + (RPPMIN2(x1, x2) - bound);
        dstPtrSubImage = dstPtr + (RPPMIN2(y1, y2) * srcSize.width) + RPPMIN2(x1, x2);
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        srcPtrSubImage = srcPtr + (RPPMIN2(y1, y2) * srcSize.width * channel) + (RPPMIN2(x1, x2) * channel);
        srcPtrMod = srcPtrSubImage - (bound * srcSize.width * channel) - (bound * channel);
        //srcPtrMod = srcPtr + ((RPPMIN2(y1, y2) - bound) * srcSize.width * channel) + ((RPPMIN2(x1, x2) - bound) * channel);
        dstPtrSubImage = dstPtr + (RPPMIN2(y1, y2) * srcSize.width * channel) + (RPPMIN2(x1, x2) * channel);
    }
*/
///*
    RppiSize srcSizeMod, srcSizeSubImage;
    T *srcPtrMod, *srcPtrSubImage, *dstPtrSubImage;

    compute_subimage_location_host(srcPtr, &srcPtrSubImage, dstPtr, &dstPtrSubImage, srcSize, &srcSizeSubImage, x1, y1, x2, y2, chnFormat, channel);

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
//*/
    convolve_subimage_host(srcPtrMod, srcSizeMod, dstPtrSubImage, srcSizeSubImage, srcSize, kernel, kernelSize, chnFormat, channel);

    return RPP_SUCCESS;
}
