#include <cpu/rpp_cpu_common.hpp>
#include <stdlib.h>
#include <time.h>

template <typename T>
RppStatus crop_replace_subimage_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    Rpp32u occlusionHeight, Rpp32u occlusionWidth, 
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    if ((RPPINRANGE(occlusionHeight, 0, srcSize.height - 1) == 0) 
        || (RPPINRANGE(occlusionWidth, 0, srcSize.width - 1) == 0))
    {
        return RPP_ERROR;
    }

    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;
    memcpy(dstPtr, srcPtr, channel * srcSize.height * srcSize.width * sizeof(T));

    Rpp32u xDst = (rand() % (srcSize.width - occlusionWidth));
    Rpp32u yDst = (rand() % (srcSize.height - occlusionHeight));

    // Copies pixels from another location in the image
///*
    Rpp32u xSrc = (rand() % (srcSize.width - occlusionWidth));
    Rpp32u ySrc = (rand() % (srcSize.height - occlusionHeight));

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        Rpp32u remainingElementsInRow = srcSize.width - occlusionWidth;
        for (int c = 0; c < channel; c++)
        {
            srcPtrTemp = srcPtr + (c * srcSize.height * srcSize.width) + (ySrc * srcSize.width) + xSrc;
            dstPtrTemp = dstPtr + (c * srcSize.height * srcSize.width) + (yDst * srcSize.width) + xDst;

            for (int i = 0; i < occlusionHeight; i++)
            {
                for (int j = 0; j < occlusionWidth; j++)
                {
                    *dstPtrTemp = *srcPtrTemp;
                    dstPtrTemp++;
                    srcPtrTemp++;
                }
                srcPtrTemp += remainingElementsInRow;
                dstPtrTemp += remainingElementsInRow;
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        srcPtrTemp = srcPtr + (channel * ((ySrc * srcSize.width) + xSrc));
        dstPtrTemp = dstPtr + (channel * ((yDst * srcSize.width) + xDst));
        Rpp32u remainingElementsInRow = channel * (srcSize.width - occlusionWidth);
        for (int i = 0; i < occlusionHeight; i++)
        {
            for (int j = 0; j < occlusionWidth; j++)
            {
                for (int c = 0; c < channel; c++)
                {
                    *dstPtrTemp = *srcPtrTemp;
                    dstPtrTemp++;
                    srcPtrTemp++;
                }
            }
            srcPtrTemp += remainingElementsInRow;
            dstPtrTemp += remainingElementsInRow;
        }
    }
//*/

    // Makes pixels black
/*
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        Rpp32u remainingElementsInRow = srcSize.width - occlusionWidth;
        for (int c = 0; c < channel; c++)
        {
            dstPtrTemp = dstPtr + (c * srcSize.height * srcSize.width) + (yDst * srcSize.width) + xDst;

            for (int i = 0; i < occlusionHeight; i++)
            {
                for (int j = 0; j < occlusionWidth; j++)
                {
                    *dstPtrTemp = (T) 0;
                    dstPtrTemp++;
                }
                dstPtrTemp += remainingElementsInRow;
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        dstPtrTemp = dstPtr + (channel * ((yDst * srcSize.width) + xDst));
        Rpp32u remainingElementsInRow = channel * (srcSize.width - occlusionWidth);
        for (int i = 0; i < occlusionHeight; i++)
        {
            for (int j = 0; j < occlusionWidth; j++)
            {
                for (int c = 0; c < channel; c++)
                {
                    *dstPtrTemp = (T) 0;
                    dstPtrTemp++;
                }
            }
            dstPtrTemp += remainingElementsInRow;
        }
    }
*/
 
    return RPP_SUCCESS;
}
