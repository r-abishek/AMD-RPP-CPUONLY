#include <cpu/rpp_cpu_common.hpp>

RppStatus scale_output_size_host(RppiSize srcSize, RppiSize *dstSizePtr,
                                  Rpp32f percentage)
{
    if (percentage < 0)
    {
        return RPP_ERROR;
    }
    percentage /= 100;
    dstSizePtr->height = (Rpp32s) (percentage * (Rpp32f) srcSize.height);
    dstSizePtr->width = (Rpp32s) (percentage * (Rpp32f) srcSize.width);

    return RPP_SUCCESS;
}

template <typename T>
RppStatus scale_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           Rpp32f percentage,
                           RppiChnFormat chnFormat, unsigned int channel)
{
    if (percentage < 0)
    {
        return RPP_ERROR;
    }
    
    percentage = round(percentage * 100.0) / 100.0;
    percentage /= 100;

// Abishek - Pointers Version - More efficient with if-statement for last row/column

    Rpp32f srcLocationRow, srcLocationColumn, pixel;
    Rpp32s srcLocationRowFloor, srcLocationColumnFloor;
    T *srcPtrTemp, *dstPtrTemp, *srcPtrTopRow, *srcPtrBottomRow;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < dstSize.height; i++)
            {   
                srcLocationRow = ((Rpp32f) i) / percentage;
                srcLocationRowFloor = (Rpp32s) RPPFLOOR(srcLocationRow);
                Rpp32f weightedHeight = srcLocationRow - srcLocationRowFloor;
                if (srcLocationRowFloor > (srcSize.height - 2))
                {
                    srcLocationRowFloor = srcSize.height - 2;
                }

                srcPtrTopRow = srcPtrTemp + srcLocationRowFloor * srcSize.width;
                srcPtrBottomRow  = srcPtrTopRow + srcSize.width;
                
                for (int j = 0; j < dstSize.width; j++)
                {
                    srcLocationColumn = ((Rpp32f) j) / percentage;
                    srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);
                    Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;

                    if (srcLocationColumnFloor > (srcSize.width - 2))
                    {
                        srcLocationColumnFloor = srcSize.width - 2;
                    }
                    pixel = ((*(srcPtrTopRow + srcLocationColumnFloor)) * (1 - weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrTopRow + srcLocationColumnFloor + 1)) * (1 - weightedHeight) * (weightedWidth)) 
                            + ((*(srcPtrBottomRow + srcLocationColumnFloor)) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrBottomRow + srcLocationColumnFloor + 1)) * (weightedHeight) * (weightedWidth));
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
                    dstPtrTemp ++;
                }
            }
            srcPtrTemp += srcSize.height * srcSize.width;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        Rpp32s elementsInRow = srcSize.width * channel;
        for (int i = 0; i < dstSize.height; i++)
        {
            srcLocationRow = ((Rpp32f) i) / percentage;
            srcLocationRowFloor = (Rpp32s) RPPFLOOR(srcLocationRow);
            Rpp32f weightedHeight = srcLocationRow - srcLocationRowFloor;

            if (srcLocationRowFloor > (srcSize.height - 2))
            {
                srcLocationRowFloor = srcSize.height - 2;
            }

            srcPtrTopRow = srcPtrTemp + srcLocationRowFloor * elementsInRow;
            srcPtrBottomRow  = srcPtrTopRow + elementsInRow;

            for (int j = 0; j < dstSize.width; j++)
            {   
                srcLocationColumn = ((Rpp32f) j) / percentage;
                srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);
                Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;

                if (srcLocationColumnFloor > (srcSize.width - 2))
                {
                    srcLocationColumnFloor = srcSize.width - 2;
                }

                Rpp32s srcLocColFloorChanneled = channel * srcLocationColumnFloor;
                
                for (int c = 0; c < channel; c++)
                {
                    pixel = ((*(srcPtrTopRow + c + srcLocColFloorChanneled)) * (1 - weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrTopRow + c + srcLocColFloorChanneled + channel)) * (1 - weightedHeight) * (weightedWidth)) 
                            + ((*(srcPtrBottomRow + c + srcLocColFloorChanneled)) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrBottomRow + c + srcLocColFloorChanneled + channel)) * (weightedHeight) * (weightedWidth));
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
                    dstPtrTemp ++;
                }
            }
        }
    }


















// Abishek - Pointers Version - Inefficient
/*
    Rpp32f srcLocationRow, srcLocationColumn, pixel;
    T *dstPtrTemp, *srcPtrTopRow, *srcPtrBottomRow;
    dstPtrTemp = dstPtr;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < dstSize.height; i++)
            {   
                srcLocationRow = ((Rpp32f) i) / percentage;
                Rpp32f weightedHeight = srcLocationRow - RPPFLOOR(srcLocationRow);
                
                for (int j = 0; j < dstSize.width; j++)
                {
                    srcLocationColumn = ((Rpp32f) j) / percentage;
                    Rpp32f weightedWidth = srcLocationColumn - RPPFLOOR(srcLocationColumn);

                    if ((Rpp32s) RPPFLOOR(srcLocationRow) > (srcSize.height - 2))
                    {
                        srcLocationRow = srcSize.height - 2;
                    }
                    if ((Rpp32s) RPPFLOOR(srcLocationColumn) > (srcSize.width - 2))
                    {
                        srcLocationColumn = srcSize.width - 2;
                    }

                    srcPtrTopRow = srcPtr + (c * srcSize.height * srcSize.width) 
                                            + ((Rpp32s) RPPFLOOR(srcLocationRow) * srcSize.width) 
                                            + (Rpp32s) RPPFLOOR(srcLocationColumn);
                    srcPtrBottomRow = srcPtr + (c * srcSize.height * srcSize.width) 
                                                + ((Rpp32s) RPPFLOOR(srcLocationRow + 1) * srcSize.width) 
                                                + (Rpp32s) RPPFLOOR(srcLocationColumn);

                    pixel = ((*srcPtrTopRow) * (1 - weightedWidth) * (1 - weightedHeight)) 
                            + ((*(srcPtrTopRow + 1)) * (weightedWidth) * (1 - weightedHeight)) 
                            + ((*srcPtrBottomRow) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrBottomRow + 1)) * (weightedWidth) * (weightedHeight));
                    
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
            srcLocationRow = ((Rpp32f) i) / percentage;
            Rpp32f weightedHeight = srcLocationRow - RPPFLOOR(srcLocationRow);

            for (int j = 0; j < dstSize.width; j++)
            {   
                for (int c = 0; c < channel; c++)
                {
                    srcLocationColumn = ((Rpp32f) j) / percentage;
                    Rpp32f weightedWidth = srcLocationColumn - RPPFLOOR(srcLocationColumn);

                    if ((Rpp32s) RPPFLOOR(srcLocationRow) > (srcSize.height - 2))
                    {
                        srcLocationRow = srcSize.height - 2;
                    }
                    if ((Rpp32s) RPPFLOOR(srcLocationColumn) > (srcSize.width - 2))
                    {
                        srcLocationColumn = srcSize.width - 2;
                    }
                    
                    srcPtrTopRow = srcPtr + c + channel * (((Rpp32s) RPPFLOOR(srcLocationRow) * srcSize.width) 
                                                                    + (Rpp32s) RPPFLOOR(srcLocationColumn));
                    srcPtrBottomRow = srcPtr + c + channel * (((Rpp32s) RPPFLOOR(srcLocationRow + 1) * srcSize.width) 
                                                                        + (Rpp32s) RPPFLOOR(srcLocationColumn));
                    
                    pixel = ((*srcPtrTopRow) * (1 - weightedWidth) * (1 - weightedHeight)) 
                            + ((*(srcPtrTopRow + channel)) * (weightedWidth) * (1 - weightedHeight)) 
                            + ((*srcPtrBottomRow) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrBottomRow + channel)) * (weightedWidth) * (weightedHeight));
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
                    dstPtrTemp ++;
                }
            }
        }
    }
 */
    return RPP_SUCCESS;
}