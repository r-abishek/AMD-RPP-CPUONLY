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
    int channeledJitterRangeX = jitterRangeX * channel;
    int channeledJitterRangeY = jitterRangeY * channel;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        //srcPtrBeginJitter = srcPtr + (maxJitterY * srcSize.width) + maxJitterX;
        //dstPtrBeginJitter = dstPtr + (maxJitterY * srcSize.width) + maxJitterX;
        for (int c = 0; c < channel; c++)
        {
            srcPtrTemp = srcPtr + (c * srcSize.height * srcSize.width);
            dstPtrTemp = dstPtr + (c * srcSize.height * srcSize.width);
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    if (i < maxJitterY)
                    {
                        jitteredPixelLocDiffY = rand() % (maxJitterY + 1);
                    }
                    else if (i >= (srcSize.height - maxJitterY))
                    {
                        jitteredPixelLocDiffY = -1 * (rand() % (maxJitterY + 1));
                    }
                    else
                    {
                        jitteredPixelLocDiffY = (rand() % (jitterRangeY + 1)) - maxJitterY;
                    }
                    
                    if (j < maxJitterX)
                    {
                        jitteredPixelLocDiffX = rand() % (maxJitterX + 1);
                    }
                    else if (j >= (srcSize.width - maxJitterX))
                    {
                        jitteredPixelLocDiffX = -1 * (rand() % (maxJitterX + 1));
                    }
                    else
                    {
                        jitteredPixelLocDiffX = (rand() % (jitterRangeX + 1)) - maxJitterX;
                    }

                    //printf("\njitteredPixelLocDiffX = %d", jitteredPixelLocDiffX);
                    //printf("\njitteredPixelLocDiffY = %d", jitteredPixelLocDiffY);
                    //return RPP_SUCCESS;

                    *dstPtrTemp = *(srcPtrTemp + (jitteredPixelLocDiffY * (int) srcSize.width) + jitteredPixelLocDiffX);
                    srcPtrTemp++;
                    dstPtrTemp++;
                }
                srcPtrTemp += jitterRangeX;
                dstPtrTemp += jitterRangeX;
            }
        }
        
        
        
        /*
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
                    jitteredPixelLocDiffX = (rand() % (jitterRangeX + 1)) - 1;
                    jitteredPixelLocDiffY = (rand() % (jitterRangeY + 1)) - 1;
                    jitteredPixelLocDiffX -= maxJitterX;
                    jitteredPixelLocDiffY -= maxJitterY;
                    //printf("\njitteredPixelLocDiffX = %d", jitteredPixelLocDiffX);
                    //printf("\njitteredPixelLocDiffY = %d", jitteredPixelLocDiffY);
                    //return RPP_SUCCESS;
                    *dstPtrTemp = *(srcPtrTemp + (jitteredPixelLocDiffY * (int) srcSize.width) + jitteredPixelLocDiffX);
                    srcPtrTemp++;
                    dstPtrTemp++;
                }
                srcPtrTemp += jitterRangeX;
                dstPtrTemp += jitterRangeX;
            }
        }
        */
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        int elementsInRow = (int)(srcSize.width * channel);
        srcPtrBeginJitter = srcPtr + (maxJitterY * elementsInRow) + (maxJitterX * channel);
        dstPtrBeginJitter = dstPtr + (maxJitterY * elementsInRow) + (maxJitterX * channel);
        for (int i = 0; i < srcSize.height - jitterRangeY; i++)
        {
            for (int j = 0; j < srcSize.width - jitterRangeX; j++)
            {
                for (int c = 0; c < channel; c++)
                {
                    jitteredPixelLocDiffX = rand() % (jitterRangeX);
                    jitteredPixelLocDiffY = rand() % (jitterRangeY);
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
    
/*

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



    RppiSize srcSizeMod, srcSizeSubImage;
    T *srcPtrMod, *dstPtrSubImage;

    int yDiff = (int) y2 - (int) y1;
    int xDiff = (int) x2 - (int) x1;

    srcSizeSubImage.height = (Rpp32u) RPPABS(yDiff) + 1;
    srcSizeSubImage.width = (Rpp32u) RPPABS(xDiff) + 1;
    srcSizeMod.height = srcSizeSubImage.height + (2 * bound);
    srcSizeMod.width = srcSizeSubImage.width + (2* bound);

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        srcPtrMod = srcPtr + ((RPPMIN2(y1, y2) - bound) * srcSize.width) + (RPPMIN2(x1, x2) - bound);
        dstPtrSubImage = dstPtr + (RPPMIN2(y1, y2) * srcSize.width) + RPPMIN2(x1, x2);
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        srcPtrMod = srcPtr + ((RPPMIN2(y1, y2) - bound) * srcSize.width * channel) + ((RPPMIN2(x1, x2) - bound) * channel);
        dstPtrSubImage = dstPtr + (RPPMIN2(y1, y2) * srcSize.width * channel) + (RPPMIN2(x1, x2) * channel);
    }

    convolve_subimage_host(srcPtrMod, srcSizeMod, dstPtrSubImage, srcSizeSubImage, srcSize, kernel, kernelSize, chnFormat, channel);
*/
    return RPP_SUCCESS;
}
