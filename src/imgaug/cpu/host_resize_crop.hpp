#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus resize_crop_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           Rpp32u x1, Rpp32u y1, Rpp32u x2, Rpp32u y2,
                           RppiChnFormat chnFormat, unsigned int channel)
{
    if (dstSize.height < 0 || dstSize.width < 0)
    {
        return RPP_ERROR;
    }
    if ((RPPINRANGE(x1, 0, srcSize.width - 1) == 0) 
        || (RPPINRANGE(x2, 0, srcSize.width - 1) == 0) 
        || (RPPINRANGE(y1, 0, srcSize.height - 1) == 0) 
        || (RPPINRANGE(y2, 0, srcSize.height - 1) == 0))
    {
        return RPP_ERROR;
    }

// Abishek - Pointers Version - More efficient with if-statement for last row/column

    RppiSize srcNewSize;
    int xDiff = (int) x2 - (int) x1;
    int yDiff = (int) y2 - (int) y1;
    srcNewSize.width = (Rpp32u) RPPABS(xDiff);
    srcNewSize.height = (Rpp32u) RPPABS(yDiff);
    
    T *srcNewPtr = (T *)calloc(channel * srcNewSize.height * srcNewSize.width, sizeof(T));
    T *srcPtrTemp, *srcNewPtrTemp;
    srcPtrTemp = srcPtr;
    srcNewPtrTemp = srcNewPtr;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            srcPtrTemp += (c * srcSize.height * srcSize.width);
            srcPtrTemp += ((RPPMIN2(y1, y2) * srcSize.width) + RPPMIN2(x1, x2));
            for (int i = RPPMIN2(y1, y2); i < RPPMAX2(y1, y2); i++)
            {
                for (int j = RPPMIN2(x1, x2); j < RPPMAX2(x1, x2); j++)
                {
                    *srcNewPtrTemp = *srcPtrTemp;
                    srcNewPtrTemp++;
                    srcPtrTemp++;
                }
                srcPtrTemp += (srcSize.width - srcNewSize.width);
            }
            srcPtrTemp = srcPtr;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        Rpp32s elementsInRow = srcSize.width * channel;
        srcPtrTemp += (RPPMIN2(y1, y2) * elementsInRow) + (RPPMIN2(x1, x2) * channel);
        for (int i = RPPMIN2(y1, y2); i < RPPMAX2(y1, y2); i++)
        {
            for (int j = RPPMIN2(x1, x2); j < RPPMAX2(x1, x2); j++)
            {
                for (int c = 0; c < channel; c++)
                {
                    *srcNewPtrTemp = *srcPtrTemp;
                    srcNewPtrTemp++;
                    srcPtrTemp++;
                }
            }
            srcPtrTemp += ((srcSize.width - srcNewSize.width) * channel);
        }
    }
    //for (int i = 0; i < (channel * srcNewSize.width * srcNewSize.height); i++)
    //{
    //    dstPtr[i] = srcNewPtr[i];
    //}

    Rpp32f hRatio = (((Rpp32f) (dstSize.height - 1)) / ((Rpp32f) (srcNewSize.height - 1)));
    Rpp32f wRatio = (((Rpp32f) (dstSize.width - 1)) / ((Rpp32f) (srcNewSize.width - 1)));
    Rpp32f srcLocationRow, srcLocationColumn, pixel;
    Rpp32s srcLocationRowFloor, srcLocationColumnFloor;
    T *dstPtrTemp, *srcNewPtrTopRow, *srcNewPtrBottomRow;
    srcNewPtrTemp = srcNewPtr;
    dstPtrTemp = dstPtr;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < dstSize.height; i++)
            {   
                srcLocationRow = ((Rpp32f) i) / hRatio;
                srcLocationRowFloor = (Rpp32s) RPPFLOOR(srcLocationRow);
                Rpp32f weightedHeight = srcLocationRow - srcLocationRowFloor;
                if (srcLocationRowFloor > (srcNewSize.height - 2))
                {
                    srcLocationRowFloor = srcNewSize.height - 2;
                }

                srcNewPtrTopRow = srcNewPtrTemp + srcLocationRowFloor * srcNewSize.width;
                srcNewPtrBottomRow  = srcNewPtrTopRow + srcNewSize.width;
                
                for (int j = 0; j < dstSize.width; j++)
                {
                    srcLocationColumn = ((Rpp32f) j) / wRatio;
                    srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);
                    Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;

                    if (srcLocationColumnFloor > (srcNewSize.width - 2))
                    {
                        srcLocationColumnFloor = srcNewSize.width - 2;
                    }
                    pixel = ((*(srcNewPtrTopRow + srcLocationColumnFloor)) * (1 - weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcNewPtrTopRow + srcLocationColumnFloor + 1)) * (1 - weightedHeight) * (weightedWidth)) 
                            + ((*(srcNewPtrBottomRow + srcLocationColumnFloor)) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcNewPtrBottomRow + srcLocationColumnFloor + 1)) * (weightedHeight) * (weightedWidth));
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
                    dstPtrTemp ++;
                }
            }
            srcNewPtrTemp += srcNewSize.height * srcNewSize.width;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        Rpp32s elementsInRow = srcNewSize.width * channel;
        for (int i = 0; i < dstSize.height; i++)
        {
            srcLocationRow = ((Rpp32f) i) / hRatio;
            srcLocationRowFloor = (Rpp32s) RPPFLOOR(srcLocationRow);
            Rpp32f weightedHeight = srcLocationRow - srcLocationRowFloor;

            if (srcLocationRowFloor > (srcNewSize.height - 2))
            {
                srcLocationRowFloor = srcNewSize.height - 2;
            }

            srcNewPtrTopRow = srcNewPtrTemp + srcLocationRowFloor * elementsInRow;
            srcNewPtrBottomRow  = srcNewPtrTopRow + elementsInRow;

            for (int j = 0; j < dstSize.width; j++)
            {   
                srcLocationColumn = ((Rpp32f) j) / wRatio;
                srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);
                Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;

                if (srcLocationColumnFloor > (srcNewSize.width - 2))
                {
                    srcLocationColumnFloor = srcNewSize.width - 2;
                }

                Rpp32s srcLocColFloorChanneled = channel * srcLocationColumnFloor;
                
                for (int c = 0; c < channel; c++)
                {
                    pixel = ((*(srcNewPtrTopRow + c + srcLocColFloorChanneled)) * (1 - weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcNewPtrTopRow + c + srcLocColFloorChanneled + channel)) * (1 - weightedHeight) * (weightedWidth)) 
                            + ((*(srcNewPtrBottomRow + c + srcLocColFloorChanneled)) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcNewPtrBottomRow + c + srcLocColFloorChanneled + channel)) * (weightedHeight) * (weightedWidth));
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
                    dstPtrTemp ++;
                }
            }
        }
    }

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
// Abishek - Pointers Version - More efficient with if-statement for last row/column
/*
    Rpp32f hRatio = (((Rpp32f) dstSize.height) / ((Rpp32f) srcNewSize.height));
    Rpp32f wRatio = (((Rpp32f) dstSize.width) / ((Rpp32f) srcNewSize.width));
    Rpp32f srcLocationRow, srcLocationColumn, pixel;
    Rpp32s srcLocationRowFloor, srcLocationColumnFloor;
    T *srcNewPtrTemp, *dstPtrTemp, *srcNewPtrTopRow, *srcNewPtrBottomRow;
    srcNewPtrTemp = srcNewPtr;
    dstPtrTemp = dstPtr;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < dstSize.height; i++)
            {   
                srcLocationRow = ((Rpp32f) i) / hRatio;
                srcLocationRowFloor = (Rpp32s) RPPFLOOR(srcLocationRow);
                Rpp32f weightedHeight = srcLocationRow - srcLocationRowFloor;
                if (srcLocationRowFloor > (srcNewSize.height - 2))
                {
                    srcLocationRowFloor = srcNewSize.height - 2;
                }

                srcNewPtrTopRow = srcNewPtrTemp + srcLocationRowFloor * srcNewSize.width;
                srcNewPtrBottomRow  = srcNewPtrTopRow + srcNewSize.width;
                
                for (int j = 0; j < dstSize.width; j++)
                {
                    srcLocationColumn = ((Rpp32f) j) / wRatio;
                    srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);
                    Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;

                    if (srcLocationColumnFloor > (srcNewSize.width - 2))
                    {
                        srcLocationColumnFloor = srcNewSize.width - 2;
                    }
                    pixel = ((*(srcNewPtrTopRow + srcLocationColumnFloor)) * (1 - weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcNewPtrTopRow + srcLocationColumnFloor + 1)) * (1 - weightedHeight) * (weightedWidth)) 
                            + ((*(srcNewPtrBottomRow + srcLocationColumnFloor)) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcNewPtrBottomRow + srcLocationColumnFloor + 1)) * (weightedHeight) * (weightedWidth));
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
                    dstPtrTemp ++;
                }
            }
            srcNewPtrTemp += srcNewSize.height * srcNewSize.width;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        Rpp32s elementsInRow = srcNewSize.width * channel;
        for (int i = 0; i < dstSize.height; i++)
        {
            srcLocationRow = ((Rpp32f) i) / hRatio;
            srcLocationRowFloor = (Rpp32s) RPPFLOOR(srcLocationRow);
            Rpp32f weightedHeight = srcLocationRow - srcLocationRowFloor;

            if (srcLocationRowFloor > (srcNewSize.height - 2))
            {
                srcLocationRowFloor = srcNewSize.height - 2;
            }

            srcNewPtrTopRow = srcNewPtrTemp + srcLocationRowFloor * elementsInRow;
            srcNewPtrBottomRow  = srcNewPtrTopRow + elementsInRow;

            for (int j = 0; j < dstSize.width; j++)
            {   
                srcLocationColumn = ((Rpp32f) j) / wRatio;
                srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);
                Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;

                if (srcLocationColumnFloor > (srcNewSize.width - 2))
                {
                    srcLocationColumnFloor = srcNewSize.width - 2;
                }

                Rpp32s srcLocColFloorChanneled = channel * srcLocationColumnFloor;
                
                for (int c = 0; c < channel; c++)
                {
                    pixel = ((*(srcNewPtrTopRow + c + srcLocColFloorChanneled)) * (1 - weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcNewPtrTopRow + c + srcLocColFloorChanneled + channel)) * (1 - weightedHeight) * (weightedWidth)) 
                            + ((*(srcNewPtrBottomRow + c + srcLocColFloorChanneled)) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcNewPtrBottomRow + c + srcLocColFloorChanneled + channel)) * (weightedHeight) * (weightedWidth));
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
                    dstPtrTemp ++;
                }
            }
        }
    }
 */

    
    
    
    
    
    
    
    
    
    
    
    
    









    
// Abishek - Pointers Version - Inefficient
/*
    Rpp32f hRatio = (((Rpp32f) dstSize.height) / ((Rpp32f) srcNewSize.height));
    Rpp32f wRatio = (((Rpp32f) dstSize.width) / ((Rpp32f) srcNewSize.width));
    Rpp32f srcLocationRow, srcLocationColumn, pixel;
    T *dstPtrTemp, *srcNewPtrTopRow, *srcNewPtrBottomRow;
    dstPtrTemp = dstPtr;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < dstSize.height; i++)
            {   
                srcLocationRow = ((Rpp32f) i) / hRatio;
                Rpp32f weightedHeight = srcLocationRow - RPPFLOOR(srcLocationRow);
                
                for (int j = 0; j < dstSize.width; j++)
                {
                    srcLocationColumn = ((Rpp32f) j) / wRatio;
                    Rpp32f weightedWidth = srcLocationColumn - RPPFLOOR(srcLocationColumn);

                    if ((Rpp32s) RPPFLOOR(srcLocationRow) > (srcNewSize.height - 2))
                    {
                        srcLocationRow = srcNewSize.height - 2;
                    }
                    if ((Rpp32s) RPPFLOOR(srcLocationColumn) > (srcNewSize.width - 2))
                    {
                        srcLocationColumn = srcNewSize.width - 2;
                    }

                    srcNewPtrTopRow = srcNewPtr + (c * srcNewSize.height * srcNewSize.width) 
                                            + ((Rpp32s) RPPFLOOR(srcLocationRow) * srcNewSize.width) 
                                            + (Rpp32s) RPPFLOOR(srcLocationColumn);
                    srcNewPtrBottomRow = srcNewPtr + (c * srcNewSize.height * srcNewSize.width) 
                                                + ((Rpp32s) RPPFLOOR(srcLocationRow + 1) * srcNewSize.width) 
                                                + (Rpp32s) RPPFLOOR(srcLocationColumn);

                    pixel = ((*srcNewPtrTopRow) * (1 - weightedWidth) * (1 - weightedHeight)) 
                            + ((*(srcNewPtrTopRow + 1)) * (weightedWidth) * (1 - weightedHeight)) 
                            + ((*srcNewPtrBottomRow) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcNewPtrBottomRow + 1)) * (weightedWidth) * (weightedHeight));
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
                    dstPtrTemp ++;
                }
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int i = 0; i < dstSize.height; i++)
        {
            srcLocationRow = ((Rpp32f) i) / hRatio;
            Rpp32f weightedHeight = srcLocationRow - RPPFLOOR(srcLocationRow);

            for (int j = 0; j < dstSize.width; j++)
            {   
                for (int c = 0; c < channel; c++)
                {
                    srcLocationColumn = ((Rpp32f) j) / wRatio;
                    Rpp32f weightedWidth = srcLocationColumn - RPPFLOOR(srcLocationColumn);

                    if ((Rpp32s) RPPFLOOR(srcLocationRow) > (srcNewSize.height - 2))
                    {
                        srcLocationRow = srcNewSize.height - 2;
                    }
                    if ((Rpp32s) RPPFLOOR(srcLocationColumn) > (srcNewSize.width - 2))
                    {
                        srcLocationColumn = srcNewSize.width - 2;
                    }
                    
                    srcNewPtrTopRow = srcNewPtr + c + channel * (((Rpp32s) RPPFLOOR(srcLocationRow) * srcNewSize.width) 
                                                                    + (Rpp32s) RPPFLOOR(srcLocationColumn));
                    srcNewPtrBottomRow = srcNewPtr + c + channel * (((Rpp32s) RPPFLOOR(srcLocationRow + 1) * srcNewSize.width) 
                                                                        + (Rpp32s) RPPFLOOR(srcLocationColumn));
                    
                    pixel = ((*srcNewPtrTopRow) * (1 - weightedWidth) * (1 - weightedHeight)) 
                            + ((*(srcNewPtrTopRow + channel)) * (weightedWidth) * (1 - weightedHeight)) 
                            + ((*srcNewPtrBottomRow) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcNewPtrBottomRow + channel)) * (weightedWidth) * (weightedHeight));
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
                    dstPtrTemp ++;
                }
            }
        }
    }
*/

    return RPP_SUCCESS;
    
}