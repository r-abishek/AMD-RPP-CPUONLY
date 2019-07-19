#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus local_binary_pattern_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    unsigned int kernelSize, RppiLbpFormat lbpFormat, 
                    RppiChnFormat chnFormat, unsigned int channel)
{
    if (lbpFormat == RPPI_LBP)
    {
        if (kernelSize != 3 && kernelSize != 5)
        {
            return RPP_ERROR;
        }
        
    }
    else if (lbpFormat == RPPI_MLBP)
    {

    }
    else if (lbpFormat == RPPI_ULBP)
    {

    }

    int bound = ((kernelSize - 1) / 2);

    RppiSize srcSizeMod;
    srcSizeMod.width = srcSize.width + (2 * bound);
    srcSizeMod.height = srcSize.height + (2 * bound);
    Rpp8u *srcPtrMod = (Rpp8u *)calloc(srcSizeMod.width * srcSizeMod.height * channel, sizeof(Rpp8u));

    generate_evenly_padded_image_host(srcPtr, srcSize, srcPtrMod, srcSizeMod, chnFormat, channel);

    Rpp32u remainingElementsInRowPlanar = srcSizeMod.width - kernelSize;
    Rpp32u remainingElementsInRowPacked = (srcSizeMod.width - kernelSize) * channel;
    
    T *srcPtrWindow, *dstPtrTemp;
    srcPtrWindow = srcPtrMod;
    dstPtrTemp = dstPtr;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    local_binary_pattern_kernel_host(srcPtrWindow, dstPtrTemp, srcSize, 
                                    kernelSize, remainingElementsInRowPlanar, lbpFormat, 
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
        for (int i = 0; i < srcSize.height; i++)
        {
            for (int j = 0; j < srcSize.width; j++)
            {
                for (int c = 0; c < channel; c++)
                {   
                    local_binary_pattern_kernel_host(srcPtrWindow, dstPtrTemp, srcSize, 
                                    kernelSize, remainingElementsInRowPacked, lbpFormat, 
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
