#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus flip_host(T* srcPtr, RppiSize srcSize, T* dstPtr, 
                    RppiAxis flipAxis,
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        if (flipAxis == RPPI_HORIZONTAL_AXIS)
        {
            for (int c = 0; c < channel; c++)
            {
                srcPtrTemp = srcPtr + ((c + 1) * srcSize.height * srcSize.width) - srcSize.width;
                for (int i = 0; i < srcSize.height; i++)
                {
                    for (int j = 0; j < srcSize.width; j++)
                    {
                        *dstPtrTemp = *srcPtrTemp;
                        dstPtrTemp++;
                        srcPtrTemp++;
                    }
                    srcPtrTemp = srcPtrTemp - (2 * srcSize.width);
                }
            }
        }
        else if (flipAxis == RPPI_VERTICAL_AXIS)
        {
            for (int c = 0; c < channel; c++)
            {
                srcPtrTemp = srcPtr + (c * srcSize.height * srcSize.width) + srcSize.width - 1;
                for (int i = 0; i < srcSize.height; i++)
                {
                    for (int j = 0; j < srcSize.width; j++)
                    {
                        *dstPtrTemp = *srcPtrTemp;
                        dstPtrTemp++;
                        srcPtrTemp--;
                    }
                    srcPtrTemp = srcPtrTemp + (2 * srcSize.width);
                }
            }
        }
        else if (flipAxis == RPPI_BOTH_AXIS)
        {
            for (int c = 0; c < channel; c++)
            {
                srcPtrTemp = srcPtr + ((c + 1) * srcSize.height * srcSize.width) - 1;
                for (int i = 0; i < srcSize.height; i++)
                {
                    for (int j = 0; j < srcSize.width; j++)
                    {
                        *dstPtrTemp = *srcPtrTemp;
                        dstPtrTemp++;
                        srcPtrTemp--;
                    }
                }
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        if (flipAxis == RPPI_HORIZONTAL_AXIS)
        {
            srcPtrTemp = srcPtr + (channel * ((srcSize.height * srcSize.width) - srcSize.width));
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    for (int c = 0; c < channel; c++)
                    {
                        *dstPtrTemp = *srcPtrTemp;
                        dstPtrTemp++;
                        srcPtrTemp++;
                    }
                }
                srcPtrTemp = srcPtrTemp - (channel * (2 * srcSize.width));
            }
        }
        else if (flipAxis == RPPI_VERTICAL_AXIS)
        {
            srcPtrTemp = srcPtr + (channel * (srcSize.width - 1));
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    for (int c = 0; c < channel; c++)
                    {
                        *dstPtrTemp = *srcPtrTemp;
                        dstPtrTemp++;
                        srcPtrTemp++;
                    }
                    srcPtrTemp = srcPtrTemp - (2 * channel);
                }
                srcPtrTemp = srcPtrTemp + (channel * (2 * srcSize.width));
            }
        }
        else if (flipAxis == RPPI_BOTH_AXIS)
        {
            srcPtrTemp = srcPtr + (channel * ((srcSize.height * srcSize.width) - 1));
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    for (int c = 0; c < channel; c++)
                    {
                        *dstPtrTemp = *srcPtrTemp;
                        dstPtrTemp++;
                        srcPtrTemp++;
                    }
                    srcPtrTemp = srcPtrTemp - (2 * channel);
                }
            }
        }
    }
    
    return RPP_SUCCESS;
}