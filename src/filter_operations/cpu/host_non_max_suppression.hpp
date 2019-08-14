#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus non_max_suppression_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    Rpp32u kernelSize,
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    if (kernelSize % 2 == 0)
    {
        return RPP_ERROR;
    }
    int bound = ((kernelSize - 1) / 2);

    RppiSize srcSizeMod;
    srcSizeMod.width = srcSize.width + (2 * bound);
    srcSizeMod.height = srcSize.height + (2 * bound);
    Rpp8u *srcPtrMod = (Rpp8u *)calloc(srcSizeMod.height * srcSizeMod.width * channel, sizeof(Rpp8u));

    generate_evenly_padded_image_host(srcPtr, srcSize, srcPtrMod, srcSizeMod, chnFormat, channel);
    
    T *srcPtrWindow, *dstPtrTemp;
    T windowCenter;
    srcPtrWindow = srcPtrMod;
    dstPtrTemp = dstPtr;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        Rpp32u windowCenterPosIncrement = (bound * srcSizeMod.width) + bound;
        Rpp32u remainingElementsInRow = srcSizeMod.width - kernelSize;
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    windowCenter = (T) *(srcPtrWindow + windowCenterPosIncrement);
                    non_max_suppression_kernel_host(srcPtrWindow, dstPtrTemp, srcSize, 
                                      kernelSize, remainingElementsInRow, windowCenter, 
                                      chnFormat, channel);
                    srcPtrWindow++;
                    dstPtrTemp++;
                }
                srcPtrWindow += (kernelSize - 1);
            }
            srcPtrWindow += ((kernelSize - 1) * srcSizeMod.width);
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        Rpp32u windowCenterPosIncrement = channel * ((bound * srcSizeMod.width) + bound);
        Rpp32u remainingElementsInRow = (srcSizeMod.width - kernelSize) * channel;
        for (int i = 0; i < srcSize.height; i++)
        {
            for (int j = 0; j < srcSize.width; j++)
            {
                for (int c = 0; c < channel; c++)
                {   
                    windowCenter = (T) *(srcPtrWindow + windowCenterPosIncrement);
                    non_max_suppression_kernel_host(srcPtrWindow, dstPtrTemp, srcSize, 
                                      kernelSize, remainingElementsInRow, windowCenter, 
                                      chnFormat, channel);
                    srcPtrWindow++;
                    dstPtrTemp++;
                }
            }
            srcPtrWindow += ((kernelSize - 1) * channel);
        }
    }

    return RPP_SUCCESS;
}
