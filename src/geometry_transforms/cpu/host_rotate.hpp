#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus rotate_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           Rpp32f angleDeg,
                           RppiChnFormat chnFormat, Rpp32u channel)
{
    Rpp32f angleRad = -RAD(angleDeg);
    Rpp32f rotate[4] = {0};
    rotate[0] = cos(angleRad);
    rotate[1] = sin(angleRad);
    rotate[2] = -sin(angleRad);
    rotate[3] = cos(angleRad);

    T *srcPtrTemp, *dstPtrTemp, *srcPtrTopRow, *srcPtrBottomRow;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;
    
    Rpp32f divisor = (rotate[1] * rotate[2]) - (rotate[0] * rotate[3]);
    Rpp32f srcLocationRow, srcLocationColumn, srcLocationRowTerm1, srcLocationColumnTerm1, pixel;
    Rpp32s srcLocationRowFloor, srcLocationColumnFloor;

    Rpp32f halfSrcHeight = srcSize.height / 2;
    Rpp32f halfSrcWidth = srcSize.width / 2;
    Rpp32f halfDstHeight = dstSize.height / 2;
    Rpp32f halfDstWidth = dstSize.width / 2;
    Rpp32f halfHeightDiff = halfSrcHeight - halfDstHeight;
    Rpp32f halfWidthDiff = halfSrcWidth - halfDstWidth;

    Rpp32f srcLocationRowParameter = (rotate[0] * halfSrcHeight) + (rotate[1] * halfSrcWidth) - halfSrcHeight + halfHeightDiff;
    Rpp32f srcLocationColumnParameter = (rotate[2] * halfSrcHeight) + (rotate[3] * halfSrcWidth) - halfSrcWidth + halfWidthDiff;
    Rpp32f srcLocationRowParameter2 = (-rotate[3] * (Rpp32s)srcLocationRowParameter) + (rotate[1] * (Rpp32s)srcLocationColumnParameter);
    Rpp32f srcLocationColumnParameter2 = (rotate[2] * (Rpp32s)srcLocationRowParameter) + (-rotate[0] * (Rpp32s)srcLocationColumnParameter);

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < dstSize.height; i++)
            {
                srcLocationRowTerm1 = -rotate[3] * i;
                srcLocationColumnTerm1 = rotate[2] * i;
                for (int j = 0; j < dstSize.width; j++)
                {
                    srcLocationRow = (srcLocationRowTerm1 + (rotate[1] * j) + srcLocationRowParameter2) / divisor;
                    srcLocationColumn = (srcLocationColumnTerm1 + (-rotate[0] * j) + srcLocationColumnParameter2) / divisor;
                    
                    if (srcLocationRow < 0 || srcLocationColumn < 0 || srcLocationRow > (srcSize.height - 2) || srcLocationColumn > (srcSize.width - 2))
                    {
                        *dstPtrTemp = 0;
                        dstPtrTemp++;
                    }
                    else
                    {
                        srcLocationRowFloor = (Rpp32s) RPPFLOOR(srcLocationRow);
                        srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);
                        Rpp32f weightedHeight = srcLocationRow - srcLocationRowFloor;
                        Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;
                        
                        srcPtrTopRow = srcPtrTemp + srcLocationRowFloor * srcSize.width;
                        srcPtrBottomRow  = srcPtrTopRow + srcSize.width;

                        Rpp32s srcLocColFloorChanneled = channel * srcLocationColumnFloor;

                        pixel = ((*(srcPtrTopRow + srcLocationColumnFloor)) * (1 - weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrTopRow + srcLocationColumnFloor + 1)) * (1 - weightedHeight) * (weightedWidth)) 
                            + ((*(srcPtrBottomRow + srcLocationColumnFloor)) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrBottomRow + srcLocationColumnFloor + 1)) * (weightedHeight) * (weightedWidth));
                    
                        *dstPtrTemp = (T) round(pixel);
                        dstPtrTemp ++;
                    }
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
            srcLocationRowTerm1 = -rotate[3] * i;
            srcLocationColumnTerm1 = rotate[2] * i;
            for (int j = 0; j < dstSize.width; j++)
            {
                srcLocationRow = (srcLocationRowTerm1 + (rotate[1] * j) + srcLocationRowParameter2) / divisor;
                srcLocationColumn = (srcLocationColumnTerm1 + (-rotate[0] * j) + srcLocationColumnParameter2) / divisor;
                
                if (srcLocationRow < 0 || srcLocationColumn < 0 || srcLocationRow > (srcSize.height - 2) || srcLocationColumn > (srcSize.width - 2))
                {
                    for (int c = 0; c < channel; c++)
                    {
                        *dstPtrTemp = 0;
                        dstPtrTemp++;
                    }
                }
                else
                {
                    srcLocationRowFloor = (Rpp32s) RPPFLOOR(srcLocationRow);
                    srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);
                    Rpp32f weightedHeight = srcLocationRow - srcLocationRowFloor;
                    Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;
                    
                    srcPtrTopRow = srcPtrTemp + srcLocationRowFloor * elementsInRow;
                    srcPtrBottomRow  = srcPtrTopRow + elementsInRow;

                    Rpp32s srcLocColFloorChanneled = channel * srcLocationColumnFloor;

                    for (int c = 0; c < channel; c++)
                    {
                        pixel = ((*(srcPtrTopRow + c + srcLocColFloorChanneled)) * (1 - weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrTopRow + c + srcLocColFloorChanneled + channel)) * (1 - weightedHeight) * (weightedWidth)) 
                            + ((*(srcPtrBottomRow + c + srcLocColFloorChanneled)) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrBottomRow + c + srcLocColFloorChanneled + channel)) * (weightedHeight) * (weightedWidth));
                    
                        *dstPtrTemp = (T) round(pixel);
                        dstPtrTemp ++;
                    }
                }
            }
        }
    }

    return RPP_SUCCESS;
}


































/*
RppStatus rotate_output_size_host(RppiSize srcSize, RppiSize *dstSizePtr,
                                       Rpp32f angleDeg)
{
    Rpp32f angleRad = -RAD(angleDeg);
    Rpp32f rotate[4] = {0};
    rotate[0] = cos(angleRad);
    rotate[1] = sin(angleRad);
    rotate[2] = -sin(angleRad);
    rotate[3] = cos(angleRad);
    
    Rpp32f minX = 0, minY = 0, maxX = 0, maxY = 0;
    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            Rpp32f newi = 0, newj = 0;
            newi = (rotate[0] * i) + (rotate[1] * j);
            newj = (rotate[2] * i) + (rotate[3] * j);
            if (newi < minX)
            {
                minX = newi;
            }
            if (newj < minY)
            {
                minY = newj;
            }
            if (newi > maxX)
            {
                maxX = newi;
            }
            if (newj > maxY)
            {
                maxY = newj;
            }
        }
    }
    dstSizePtr->height = ((Rpp32s)maxX - (Rpp32s)minX) + 1;
    dstSizePtr->width = ((Rpp32s)maxY - (Rpp32s)minY) + 1;

    return RPP_SUCCESS;
}

template <typename T>
RppStatus rotate_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           Rpp32f angleDeg,
                           RppiChnFormat chnFormat, Rpp32u channel)
{
    Rpp32f angleRad = -RAD(angleDeg);
    Rpp32f rotate[4] = {0};
    rotate[0] = cos(angleRad);
    rotate[1] = sin(angleRad);
    rotate[2] = -sin(angleRad);
    rotate[3] = cos(angleRad);
    
    Rpp32f minX = 0, minY = 0;
    T *srcPtrTemp, *dstPtrTemp, *srcPtrTopRow, *srcPtrBottomRow;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;
    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            Rpp32f newi = 0, newj = 0;
            newi = (rotate[0] * i) + (rotate[1] * j);
            newj = (rotate[2] * i) + (rotate[3] * j);
            if (newi < minX)
            {
                minX = newi;
            }
            if (newj < minY)
            {
                minY = newj;
            }
        }
    }

    Rpp32f divisor = (rotate[1] * rotate[2]) - (rotate[0] * rotate[3]);
    Rpp32f srcLocationRow, srcLocationColumn, srcLocationRowTerm1, srcLocationColumnTerm1, pixel;
    Rpp32s srcLocationRowFloor, srcLocationColumnFloor;

    Rpp32f srcLocationRowParameter = (-rotate[3] * (Rpp32s)minX) + (rotate[1] * (Rpp32s)minY);
    Rpp32f srcLocationColumnParameter = (rotate[2] * (Rpp32s)minX) + (-rotate[0] * (Rpp32s)minY);

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < dstSize.height; i++)
            {
                srcLocationRowTerm1 = -rotate[3] * i;
                srcLocationColumnTerm1 = rotate[2] * i;
                for (int j = 0; j < dstSize.width; j++)
                {
                    srcLocationRow = (srcLocationRowTerm1 + (rotate[1] * j) + srcLocationRowParameter) / divisor;
                    srcLocationColumn = (srcLocationColumnTerm1 + (-rotate[0] * j) + srcLocationColumnParameter) / divisor;
                    
                    if (srcLocationRow < 0 || srcLocationColumn < 0 || srcLocationRow > (srcSize.height - 2) || srcLocationColumn > (srcSize.width - 2))
                    {
                        *dstPtrTemp = 0;
                        dstPtrTemp++;
                    }
                    else
                    {
                        srcLocationRowFloor = (Rpp32s) RPPFLOOR(srcLocationRow);
                        srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);
                        Rpp32f weightedHeight = srcLocationRow - srcLocationRowFloor;
                        Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;
                        
                        srcPtrTopRow = srcPtrTemp + srcLocationRowFloor * srcSize.width;
                        srcPtrBottomRow  = srcPtrTopRow + srcSize.width;

                        Rpp32s srcLocColFloorChanneled = channel * srcLocationColumnFloor;

                        pixel = ((*(srcPtrTopRow + srcLocationColumnFloor)) * (1 - weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrTopRow + srcLocationColumnFloor + 1)) * (1 - weightedHeight) * (weightedWidth)) 
                            + ((*(srcPtrBottomRow + srcLocationColumnFloor)) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrBottomRow + srcLocationColumnFloor + 1)) * (weightedHeight) * (weightedWidth));
                    
                        *dstPtrTemp = (T) round(pixel);
                        dstPtrTemp ++;
                    }
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
            srcLocationRowTerm1 = -rotate[3] * i;
            srcLocationColumnTerm1 = rotate[2] * i;
            for (int j = 0; j < dstSize.width; j++)
            {
                srcLocationRow = (srcLocationRowTerm1 + (rotate[1] * j) + srcLocationRowParameter) / divisor;
                srcLocationColumn = (srcLocationColumnTerm1 + (-rotate[0] * j) + srcLocationColumnParameter) / divisor;
                
                if (srcLocationRow < 0 || srcLocationColumn < 0 || srcLocationRow > (srcSize.height - 2) || srcLocationColumn > (srcSize.width - 2))
                {
                    for (int c = 0; c < channel; c++)
                    {
                        *dstPtrTemp = 0;
                        dstPtrTemp++;
                    }
                }
                else
                {
                    srcLocationRowFloor = (Rpp32s) RPPFLOOR(srcLocationRow);
                    srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);
                    Rpp32f weightedHeight = srcLocationRow - srcLocationRowFloor;
                    Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;
                    
                    srcPtrTopRow = srcPtrTemp + srcLocationRowFloor * elementsInRow;
                    srcPtrBottomRow  = srcPtrTopRow + elementsInRow;

                    Rpp32s srcLocColFloorChanneled = channel * srcLocationColumnFloor;

                    for (int c = 0; c < channel; c++)
                    {
                        pixel = ((*(srcPtrTopRow + c + srcLocColFloorChanneled)) * (1 - weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrTopRow + c + srcLocColFloorChanneled + channel)) * (1 - weightedHeight) * (weightedWidth)) 
                            + ((*(srcPtrBottomRow + c + srcLocColFloorChanneled)) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrBottomRow + c + srcLocColFloorChanneled + channel)) * (weightedHeight) * (weightedWidth));
                    
                        *dstPtrTemp = (T) round(pixel);
                        dstPtrTemp ++;
                    }
                }
            }
        }
    }

    return RPP_SUCCESS;
}
*/