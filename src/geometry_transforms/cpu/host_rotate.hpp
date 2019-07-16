#include <cpu/rpp_cpu_common.hpp>

// Abishek - Rotate from destination to source

RppStatus rotate_output_size_host(RppiSize srcSize, RppiSize *dstSizePtr,
                                       Rpp32f angleDeg)
{
    Rpp32f angleRad = -RAD(angleDeg);
    Rpp32f rotate[4] = {0};
    rotate[0] = cos(angleRad);
    rotate[1] = sin(angleRad);
    rotate[2] = -sin(angleRad);
    rotate[3] = cos(angleRad);
    
    float minX = 0, minY = 0, maxX = 0, maxY = 0;
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
                           RppiChnFormat chnFormat, unsigned int channel)
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
                    
                        *dstPtrTemp = (Rpp8u) round(pixel);
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
                    
                        *dstPtrTemp = (Rpp8u) round(pixel);
                        dstPtrTemp ++;
                    }
                }
            }
        }
    }

    return RPP_SUCCESS;
}
























// Abishek - Rotate from source to destination
/*
RppStatus rotate_output_size_host(RppiSize srcSize, RppiSize *dstSizePtr,
                                  Rpp32f angleDeg)
{
    Rpp32f angleRad = RAD(angleDeg);
    Rpp32f rotate[4] = {0};
    rotate[0] = cos(angleRad);
    rotate[1] = sin(angleRad);
    rotate[2] = -sin(angleRad);
    rotate[3] = cos(angleRad);

    float minX = 0, minY = 0, maxX = 0, maxY = 0;
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
                           RppiChnFormat chnFormat, unsigned int channel)
{
    Rpp32f angleRad = RAD(angleDeg);
    Rpp32f rotate[4] = {0};
    rotate[0] = cos(angleRad);
    rotate[1] = sin(angleRad);
    rotate[2] = -sin(angleRad);
    rotate[3] = cos(angleRad);

    float minX = 0, minY = 0;
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

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    int k = (Rpp32s)((rotate[0] * i) + (rotate[1] * j));
                    int l = (Rpp32s)((rotate[2] * i) + (rotate[3] * j));
                    k -= (Rpp32s)minX;
                    l -= (Rpp32s)minY;
                    dstPtr[(c * dstSize.height * dstSize.width) + (k * dstSize.width) + l] = srcPtr[(c * srcSize.height * srcSize.width) + (i * srcSize.width) + j];
                }
            }
        }

        for (int c = 0; c < channel; c++)
        {
            for (int i = 1; i < dstSize.height - 1; i++)
            {
                for (int j = 1; j < dstSize.width - 1; j++)
                {
                    if (dstPtr[(c * dstSize.height * dstSize.width) + (i * dstSize.width) + j] == 0)
                    {
                        Rpp32f pixel;
                        pixel = 0.25 * (dstPtr[(c * dstSize.height * dstSize.width) + ((i - 1) * dstSize.width) + (j - 1)] +
                                        dstPtr[(c * dstSize.height * dstSize.width) + ((i - 1) * dstSize.width) + (j + 1)] +
                                        dstPtr[(c * dstSize.height * dstSize.width) + ((i + 1) * dstSize.width) + (j - 1)] +
                                        dstPtr[(c * dstSize.height * dstSize.width) + ((i + 1) * dstSize.width) + (j + 1)]);
                        dstPtr[(c * dstSize.height * dstSize.width) + (i * dstSize.width) + j] = (Rpp8u) pixel;
                    }
                }
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    int k = (Rpp32s)((rotate[0] * i) + (rotate[1] * j));
                    int l = (Rpp32s)((rotate[2] * i) + (rotate[3] * j));
                    k -= (Rpp32s)minX;
                    l -= (Rpp32s)minY;
                    dstPtr[c + (channel * k * dstSize.width) + (channel * l)] = srcPtr[c + (channel * i * srcSize.width) + (channel * j)];
                }
            }
        }

        for (int c = 0; c < channel; c++)
        {
            for (int i = 1; i < dstSize.height - 1; i++)
            {
                for (int j = 1; j < dstSize.width - 1; j++)
                {
                    if (dstPtr[c + (channel * i * dstSize.width) + (channel * j)] == 0)
                    {
                        Rpp32f pixel;
                        pixel = 0.25 * (dstPtr[c + (channel * (i - 1) * dstSize.width) + (channel * (j - 1))] +
                                        dstPtr[c + (channel * (i - 1) * dstSize.width) + (channel * (j + 1))] +
                                        dstPtr[c + (channel * (i + 1) * dstSize.width) + (channel * (j - 1))] +
                                        dstPtr[c + (channel * (i + 1) * dstSize.width) + (channel * (j + 1))]);
                        dstPtr[c + (channel * i * dstSize.width) + (channel * j)] = (Rpp8u) pixel;
                    }
                }
            }
        }
    }

    return RPP_SUCCESS;
}
*/