#include <cpu/rpp_cpu_common.hpp>
#include <stdlib.h>
#include <time.h>

template <typename T>
RppStatus jitterAdd_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    unsigned int maxJitterX, unsigned int maxJitterY, 
                    RppiChnFormat chnFormat, unsigned int channel)
{
    if ((RPPINRANGE(maxJitterX, 0, srcSize.width - 1) == 0) 
        || (RPPINRANGE(maxJitterY, 0, srcSize.height - 1) == 0))
    {
        return RPP_ERROR;
    }

    T *srcPtrTemp, *dstPtrTemp;
    T *srcPtrBeginJitter, *dstPtrBeginJitter;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        *dstPtrTemp = *srcPtrTemp;
        srcPtrTemp++;
        dstPtrTemp++;
    }

    srand (time(NULL));
    int jitteredPixelLocDiffX, jitteredPixelLocDiffY;
    int jitterRangeX = 2 * maxJitterX;
    int jitterRangeY = 2 * maxJitterY;

    if (chnFormat == RPPI_CHN_PLANAR)
    {      
        srcPtrBeginJitter = srcPtr + (maxJitterY * srcSize.width) + maxJitterX;
        dstPtrBeginJitter = dstPtr + (maxJitterY * srcSize.width) + maxJitterX;
        for (int c = 0; c < channel; c++)
        {
            srcPtrTemp = srcPtrBeginJitter + (c * srcSize.height * srcSize.width);
            dstPtrTemp = dstPtrBeginJitter + (c * srcSize.height * srcSize.width);
            for (int i = 0; i < srcSize.height - jitterRangeY; i++)
            {
                for (int j = 0; j < srcSize.width - jitterRangeX; j++)
                {
                    jitteredPixelLocDiffX = (rand() % (jitterRangeX + 1));
                    jitteredPixelLocDiffY = (rand() % (jitterRangeY + 1));
                    jitteredPixelLocDiffX -= maxJitterX;
                    jitteredPixelLocDiffY -= maxJitterY;
                    *dstPtrTemp = *(srcPtrTemp + (jitteredPixelLocDiffY * (int) srcSize.width) + jitteredPixelLocDiffX);
                    srcPtrTemp++;
                    dstPtrTemp++;
                }
                srcPtrTemp += jitterRangeX;
                dstPtrTemp += jitterRangeX;
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        int elementsInRow = (int)(srcSize.width * channel);
        int channeledJitterRangeX = jitterRangeX * channel;
        int channeledJitterRangeY = jitterRangeY * channel;
        srcPtrBeginJitter = srcPtr + (maxJitterY * elementsInRow) + (maxJitterX * channel);
        dstPtrBeginJitter = dstPtr + (maxJitterY * elementsInRow) + (maxJitterX * channel);
        srcPtrTemp = srcPtrBeginJitter;
        dstPtrTemp = dstPtrBeginJitter;
        for (int i = 0; i < srcSize.height - jitterRangeY; i++)
        {
            for (int j = 0; j < srcSize.width - jitterRangeX; j++)
            {
                for (int c = 0; c < channel; c++)
                {
                    jitteredPixelLocDiffX = rand() % (jitterRangeX + 1);
                    jitteredPixelLocDiffY = rand() % (jitterRangeY + 1);
                    jitteredPixelLocDiffX -= maxJitterX;
                    jitteredPixelLocDiffY -= maxJitterY;
                    *dstPtrTemp = *(srcPtrTemp + (jitteredPixelLocDiffY * elementsInRow) + (jitteredPixelLocDiffX * (int) channel));
                    srcPtrTemp++;
                    dstPtrTemp++;
                }
            }
            srcPtrTemp += channeledJitterRangeX;
            dstPtrTemp += channeledJitterRangeX;
        }
    }
    
    return RPP_SUCCESS;
}
