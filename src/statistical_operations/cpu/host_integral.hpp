#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus integral_host(T* srcPtr, RppiSize srcSize, U* dstPtr, 
                        RppiChnFormat chnFormat, Rpp32u channel)
{
    U* srcPtrCopy = (U *)calloc(channel * srcSize.height * srcSize.width, sizeof(U));

    T *srcPtrTemp;
    srcPtrTemp = srcPtr;
    U *srcPtrCopyTemp;
    srcPtrCopyTemp = srcPtrCopy;

    for(int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        *srcPtrCopyTemp = (U) *srcPtrTemp;
        srcPtrTemp++;
        srcPtrCopyTemp++;
    }

    RppiSize srcSizeMod;
    srcSizeMod.height = srcSize.height + 1;
    srcSizeMod.width = srcSize.width + 1;

    U* srcPtrMod = (U *)calloc(channel * srcSizeMod.height * srcSizeMod.width, sizeof(U));

    generate_corner_padded_image_host(srcPtrCopy, srcSize, srcPtrMod, srcSizeMod, 1, chnFormat, channel);

    U *srcPtrModTemp, *srcPtrModTempAbove;
    U *dstPtrTemp;
    srcPtrModTemp = srcPtrMod;
    dstPtrTemp = dstPtr;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            srcPtrModTemp = srcPtrMod + (c * srcSizeMod.height * srcSizeMod.width) + srcSizeMod.width + 1;
            srcPtrModTempAbove = srcPtrModTemp - srcSizeMod.width;
            
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    *dstPtrTemp = *srcPtrModTemp + *(srcPtrModTemp - 1) + *srcPtrModTempAbove - *(srcPtrModTempAbove - 1);
                    *srcPtrModTemp = *dstPtrTemp;
                    dstPtrTemp++;
                    srcPtrModTemp++;
                    srcPtrModTempAbove++;
                }
                srcPtrModTemp++;
                srcPtrModTempAbove++;
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        srcPtrModTemp = srcPtrMod + (channel * (srcSizeMod.width + 1));
        srcPtrModTempAbove = srcPtrModTemp - (channel * srcSizeMod.width);
            
        for (int i = 0; i < srcSize.height; i++)
        {
            for (int j = 0; j < srcSize.width; j++)
            {
                for (int c = 0; c < channel; c++)
                {
                    *dstPtrTemp = *srcPtrModTemp + *(srcPtrModTemp - channel) + *srcPtrModTempAbove - *(srcPtrModTempAbove - channel);
                    *srcPtrModTemp = *dstPtrTemp;
                    dstPtrTemp++;
                    srcPtrModTemp++;
                    srcPtrModTempAbove++;
                }
            }
            srcPtrModTemp += channel;
            srcPtrModTempAbove += channel;
        }
    }
    
    return RPP_SUCCESS;

}