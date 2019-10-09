#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus warp_affine_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           Rpp32f* affine,
                           RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTemp, *dstPtrTemp, *srcPtrTopRow, *srcPtrBottomRow;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;
    Rpp32f srcLocationRow, srcLocationColumn, pixel;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < dstSize.height; i++)
            {
                Rpp32s iNew = i - (srcSize.height / 2);
                for (int j = 0; j < dstSize.width; j++)
                {
                    Rpp32s jNew = j - (srcSize.width / 2);
                    
                    srcLocationColumn = (jNew * affine[0]) + (iNew * affine[1]) + affine[2];
                    srcLocationRow = (jNew * affine[3]) + (iNew * affine[4]) + affine[5];

                    srcLocationColumn += (srcSize.width / 2);
                    srcLocationRow += (srcSize.height / 2);

                    //printf("\nsrcLocationColumn = %f, srcLocationRow = %f", srcLocationColumn, srcLocationRow);

                    if (srcLocationRow < 0 || srcLocationColumn < 0 || srcLocationRow > (srcSize.height - 2) || srcLocationColumn > (srcSize.width - 2))
                    {
                        *dstPtrTemp = 0;
                        dstPtrTemp++;
                    }
                    else
                    {
                        //printf("\nsrcLocationColumn = %f, srcLocationRow = %f", srcLocationColumn, srcLocationRow);
                        
                        Rpp32s srcLocationRowFloor = (Rpp32s) RPPFLOOR(srcLocationRow);
                        Rpp32s srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);
                        //printf("\nsrcLocationColumnFloor = %d, srcLocationRowFloor = %d", srcLocationColumnFloor, srcLocationRowFloor);
                        
                        Rpp32f weightedHeight = srcLocationRow - srcLocationRowFloor;
                        Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;
                        //printf("\nweightedHeight = %f, weightedWidth = %f", weightedHeight, weightedWidth);
                        
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
            Rpp32s iNew = i - (srcSize.height / 2);
            for (int j = 0; j < dstSize.width; j++)
            {
                Rpp32s jNew = j - (srcSize.width / 2);

                srcLocationColumn = (jNew * affine[0]) + (iNew * affine[1]) + affine[2];
                srcLocationRow = (jNew * affine[3]) + (iNew * affine[4]) + affine[5];

                srcLocationColumn += (srcSize.width / 2);
                srcLocationRow += (srcSize.height / 2);

                //printf("\nsrcLocationColumn = %f, srcLocationRow = %f", srcLocationColumn, srcLocationRow);

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
                    //printf("\nsrcLocationColumn = %f, srcLocationRow = %f", srcLocationColumn, srcLocationRow);
                    
                    Rpp32s srcLocationRowFloor = (Rpp32s) RPPFLOOR(srcLocationRow);
                    Rpp32s srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);
                    //printf("\nsrcLocationColumnFloor = %d, srcLocationRowFloor = %d", srcLocationColumnFloor, srcLocationRowFloor);
                    
                    Rpp32f weightedHeight = srcLocationRow - srcLocationRowFloor;
                    Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;
                    //printf("\nweightedHeight = %f, weightedWidth = %f", weightedHeight, weightedWidth);
                    
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









// Previous Working Version
/*
{
    T *srcPtrTemp, *dstPtrTemp, *srcPtrTopRow, *srcPtrBottomRow;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    Rpp32f divisor = (affine[1] * affine[3]) - (affine[0] * affine[4]);
    Rpp32f srcLocationRow, srcLocationColumn, srcLocationRowTerm1, srcLocationColumnTerm1, pixel;
    Rpp32s srcLocationRowFloor, srcLocationColumnFloor;

    Rpp32f halfSrcHeight = srcSize.height / 2;
    Rpp32f halfSrcWidth = srcSize.width / 2;
    Rpp32f halfDstHeight = dstSize.height / 2;
    Rpp32f halfDstWidth = dstSize.width / 2;
    Rpp32f halfHeightDiff = halfSrcHeight - halfDstHeight;
    Rpp32f halfWidthDiff = halfSrcWidth - halfDstWidth;

    Rpp32f srcLocationRowParameter = (affine[0] * halfSrcHeight) + (affine[1] * halfSrcWidth) + (affine[2] * 1) - halfSrcHeight + halfHeightDiff;
    Rpp32f srcLocationColumnParameter = (affine[3] * halfSrcHeight) + (affine[4] * halfSrcWidth) + (affine[5] * 1) - halfSrcWidth + halfWidthDiff;
    Rpp32f srcLocationRowParameter2 = (-affine[4] * (-affine[2] + (Rpp32s)srcLocationRowParameter)) + (affine[1] * (-affine[5] + (Rpp32s)srcLocationColumnParameter));
    Rpp32f srcLocationColumnParameter2 = (affine[3] * (-affine[2] + (Rpp32s)srcLocationRowParameter)) + (-affine[0] * (-affine[5] + (Rpp32s)srcLocationColumnParameter));

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < dstSize.height; i++)
            {
                srcLocationRowTerm1 = -affine[4] * i;
                srcLocationColumnTerm1 = affine[3] * i;
                for (int j = 0; j < dstSize.width; j++)
                {
                    srcLocationRow = (srcLocationRowTerm1 + (affine[1] * j) + srcLocationRowParameter2) / divisor;
                    srcLocationColumn = (srcLocationColumnTerm1 + (-affine[0] * j) + srcLocationColumnParameter2) / divisor;
                    
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
            srcLocationRowTerm1 = -affine[4] * i;
            srcLocationColumnTerm1 = affine[3] * i;
            for (int j = 0; j < dstSize.width; j++)
            {
                srcLocationRow = (srcLocationRowTerm1 + (affine[1] * j) + srcLocationRowParameter2) / divisor;
                srcLocationColumn = (srcLocationColumnTerm1 + (-affine[0] * j) + srcLocationColumnParameter2) / divisor;
                
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










































// Old Version
/*
template <typename T>
RppStatus warp_affine_output_size_host(RppiSize srcSize, RppiSize *dstSizePtr,
                                       Rpp32f* affine)
{
    Rpp32f minX = 0, minY = 0, maxX = 0, maxY = 0;
    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            Rpp32f newi = 0, newj = 0;
            newi = (affine[0] * i) + (affine[1] * j) + (affine[2] * 1);
            newj = (affine[3] * i) + (affine[4] * j) + (affine[5] * 1);
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

template <typename T, typename U>
RppStatus warp_affine_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           Rpp32f* affine,
                           RppiChnFormat chnFormat, Rpp32u channel)
{
    Rpp32f minX = 0, minY = 0;
    T *srcPtrTemp, *dstPtrTemp, *srcPtrTopRow, *srcPtrBottomRow;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;
    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            Rpp32f newi = 0, newj = 0;
            newi = (affine[0] * i) + (affine[1] * j) + (affine[2] * 1);
            newj = (affine[3] * i) + (affine[4] * j) + (affine[5] * 1);
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

    Rpp32f divisor = (affine[1] * affine[3]) - (affine[0] * affine[4]);
    Rpp32f srcLocationRow, srcLocationColumn, srcLocationRowTerm1, srcLocationColumnTerm1, pixel;
    Rpp32s srcLocationRowFloor, srcLocationColumnFloor;

    Rpp32f srcLocationRowParameter = (-affine[4] * (-affine[2] + (Rpp32s)minX)) + (affine[1] * (-affine[5] + (Rpp32s)minY));
    Rpp32f srcLocationColumnParameter = (affine[3] * (-affine[2] + (Rpp32s)minX)) + (-affine[0] * (-affine[5] + (Rpp32s)minY));

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < dstSize.height; i++)
            {
                srcLocationRowTerm1 = -affine[4] * i;
                srcLocationColumnTerm1 = affine[3] * i;
                for (int j = 0; j < dstSize.width; j++)
                {
                    srcLocationRow = (srcLocationRowTerm1 + (affine[1] * j) + srcLocationRowParameter) / divisor;
                    srcLocationColumn = (srcLocationColumnTerm1 + (-affine[0] * j) + srcLocationColumnParameter) / divisor;
                    
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
            srcLocationRowTerm1 = -affine[4] * i;
            srcLocationColumnTerm1 = affine[3] * i;
            for (int j = 0; j < dstSize.width; j++)
            {
                srcLocationRow = (srcLocationRowTerm1 + (affine[1] * j) + srcLocationRowParameter) / divisor;
                srcLocationColumn = (srcLocationColumnTerm1 + (-affine[0] * j) + srcLocationColumnParameter) / divisor;
                
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