#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus warp_perspective_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           Rpp32f* perspective,
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
                    
                    srcLocationColumn = ((jNew * perspective[0]) + (iNew * perspective[1]) + perspective[2]) / ((jNew * perspective[6]) + (iNew * perspective[7]) + perspective[8]);
                    srcLocationRow = ((jNew * perspective[3]) + (iNew * perspective[4]) + perspective[5]) / ((jNew * perspective[6]) + (iNew * perspective[7]) + perspective[8]);

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

                srcLocationColumn = ((jNew * perspective[0]) + (iNew * perspective[1]) + perspective[2]) / ((jNew * perspective[6]) + (iNew * perspective[7]) + perspective[8]);
                srcLocationRow = ((jNew * perspective[3]) + (iNew * perspective[4]) + perspective[5]) / ((jNew * perspective[6]) + (iNew * perspective[7]) + perspective[8]);

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
























// Old Version
/*
template <typename T>
RppStatus warp_perspective_output_size_host(RppiSize srcSize, RppiSize *dstSizePtr,
                                       Rpp32f* perspective)
{
    Rpp32f minX = 0, minY = 0, maxX = 0, maxY = 0;
    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            Rpp32f newi = 0, newj = 0;
            newi = ((perspective[0] * i) + (perspective[1] * j) + ((perspective[2] - (Rpp32s)minX) * 1)) / ((perspective[6] * i) + (perspective[7] * j) + (perspective[8] * 1));
            newj = ((perspective[3] * i) + (perspective[4] * j) + ((perspective[5] - (Rpp32s)minY) * 1)) / ((perspective[6] * i) + (perspective[7] * j) + (perspective[8] * 1));
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
RppStatus warp_perspective_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           Rpp32f* perspective,
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
            newi = ((perspective[0] * i) + (perspective[1] * j) + ((perspective[2] - (Rpp32s)minX) * 1)) / ((perspective[6] * i) + (perspective[7] * j) + (perspective[8] * 1));
            newj = ((perspective[3] * i) + (perspective[4] * j) + ((perspective[5] - (Rpp32s)minY) * 1)) / ((perspective[6] * i) + (perspective[7] * j) + (perspective[8] * 1));
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

    Rpp32f parameter1, parameter2, parameter3, parameter4, parameter5, parameter6;
    Rpp32f rowTerm1NR, rowTerm2NR, rowTerm3NR, rowTerm4NR, rowTerm5NR;
    Rpp32f rowTerm1DR, rowTerm2DR, rowTerm3DR, rowTerm4DR, rowTerm5DR;
    Rpp32f colTerm1, colTerm2, colTerm3;
    Rpp32f srcLocationRow, srcLocationColumn, pixel;
    Rpp32s srcLocationRowFloor, srcLocationColumnFloor;

    parameter1 = (((perspective[2] - (Rpp32s)minX) * perspective[4]) - (perspective[1] * (perspective[5] - (Rpp32s)minY)));
    parameter2 = ((perspective[7] * (perspective[5] - (Rpp32s)minY)) - (perspective[8] * perspective[4]));
    parameter3 = ((perspective[8] * perspective[1]) - (perspective[7] * (perspective[2] - (Rpp32s)minX)));
    parameter4 = ((perspective[1] * perspective[3]) - (perspective[0] * perspective[4]));
    parameter5 = ((perspective[6] * perspective[4]) - (perspective[7] * perspective[3]));
    parameter6 = ((perspective[7] * perspective[0]) - (perspective[6] * perspective[1]));

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < dstSize.height; i++)
            {
                rowTerm1NR = i * parameter1;
                rowTerm3NR = i * i * parameter2;
                rowTerm5NR = i * (parameter2 + parameter3);
                rowTerm1DR = i * parameter4;
                rowTerm3DR = i * i * parameter5;
                rowTerm5DR = i * (parameter5 + parameter6);

                colTerm1 = (perspective[2] - (Rpp32s)minX) + (perspective[5] - (Rpp32s)minY) - (i * perspective[8]);
                colTerm2 = perspective[0] + perspective[3] - (i * perspective[6]);
                colTerm3 = (i * perspective[7]) - perspective[1] - perspective[4];

                for (int j = 0; j < dstSize.width; j++)
                {
                    rowTerm2NR = j * parameter1;
                    rowTerm4NR = j * j * parameter3;
                    rowTerm2DR = j * parameter4;
                    rowTerm4DR = j * j * parameter6;
                    
                    srcLocationRow = (rowTerm1NR + rowTerm2NR + rowTerm3NR + rowTerm4NR + (rowTerm5NR * j)) / (rowTerm1DR + rowTerm2DR + rowTerm3DR + rowTerm4DR + (rowTerm5DR * j));
                    srcLocationColumn = ((colTerm1 - (j * perspective[8])) + (srcLocationRow * (colTerm2 - (j * perspective[6])))) / (colTerm3 + (j * perspective[7]));

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
            rowTerm1NR = i * parameter1;
            rowTerm3NR = i * i * parameter2;
            rowTerm5NR = i * (parameter2 + parameter3);
            rowTerm1DR = i * parameter4;
            rowTerm3DR = i * i * parameter5;
            rowTerm5DR = i * (parameter5 + parameter6);

            colTerm1 = (perspective[2] - (Rpp32s)minX) + (perspective[5] - (Rpp32s)minY) - (i * perspective[8]);
            colTerm2 = perspective[0] + perspective[3] - (i * perspective[6]);
            colTerm3 = (i * perspective[7]) - perspective[1] - perspective[4];

            for (int j = 0; j < dstSize.width; j++)
            {
                rowTerm2NR = j * parameter1;
                rowTerm4NR = j * j * parameter3;
                rowTerm2DR = j * parameter4;
                rowTerm4DR = j * j * parameter6;
                
                srcLocationRow = (rowTerm1NR + rowTerm2NR + rowTerm3NR + rowTerm4NR + (rowTerm5NR * j)) / (rowTerm1DR + rowTerm2DR + rowTerm3DR + rowTerm4DR + (rowTerm5DR * j));
                srcLocationColumn = ((colTerm1 - (j * perspective[8])) + (srcLocationRow * (colTerm2 - (j * perspective[6])))) / (colTerm3 + (j * perspective[7]));
                
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