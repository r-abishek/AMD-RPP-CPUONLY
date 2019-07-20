#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus lens_correction_host(T* srcPtr, RppiSize srcSize, T* dstPtr, 
                               Rpp32f strength, Rpp32f zoom, 
                               RppiChnFormat chnFormat, Rpp32u channel)
{
    if (strength < 0)
    {
        return RPP_ERROR;
    }

    if (zoom < 1)
    {
        return RPP_ERROR;
    }
    
    Rpp32f halfHeight, halfWidth, newI, newJ, correctionRadius, euclideanDistance, correctedDistance, theta;
    Rpp32f srcLocationRow, srcLocationColumn, pixel;
    Rpp32s srcLocationRowFloor, srcLocationColumnFloor;
    T *srcPtrTemp, *dstPtrTemp, *srcPtrTopRow, *srcPtrBottomRow;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    halfHeight = ((Rpp32f) srcSize.height) / 2.0;
    halfWidth = ((Rpp32f) srcSize.width) / 2.0;

    if (strength == 0) strength = 0.000001;

    correctionRadius = sqrt(srcSize.height * srcSize.height + srcSize.width * srcSize.width) / strength;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for(int c = 0; c < channel; c++)
        {
            srcPtrTemp = srcPtr + (c * srcSize.height * srcSize.width);
            for (int i = 0; i < srcSize.height; i++)
            {
                newI = i - halfHeight;
                for (int j = 0; j < srcSize.width; j++)
                {
                    newJ = j - halfWidth;

                    euclideanDistance = sqrt(newI * newI + newJ * newJ);
                    
                    correctedDistance = euclideanDistance / correctionRadius;

                    if(correctedDistance == 0)
                    {
                        theta = 1;
                    }
                    else
                    {
                        theta = atan(correctedDistance) / correctedDistance;
                    }

                    srcLocationRow = halfHeight + theta * newI * zoom;
                    srcLocationColumn = halfWidth + theta * newJ * zoom;
                    
                    if ((srcLocationRow >= 0) && (srcLocationColumn >= 0) && 
                        (srcLocationRow < srcSize.height) && (srcLocationColumn < srcSize.width))
                    {
                        srcLocationRowFloor = (Rpp32s) RPPFLOOR(srcLocationRow);
                        srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);

                        Rpp32f weightedHeight = srcLocationRow - srcLocationRowFloor;
                        if (srcLocationRowFloor > (srcSize.height - 2))
                        {
                            srcLocationRowFloor = srcSize.height - 2;
                        }

                        srcPtrTopRow = srcPtrTemp + srcLocationRowFloor * srcSize.width;
                        srcPtrBottomRow  = srcPtrTopRow + srcSize.width;

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
                    }
                    dstPtrTemp++;
                }
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        Rpp32s elementsInRow = srcSize.width * channel;
        for (int i = 0; i < srcSize.height; i++)
        {
            for (int j = 0; j < srcSize.width; j++)
            {
                for(int c = 0; c < channel; c++)
                {
                    newI = i - halfHeight;
                    newJ = j - halfWidth;

                    euclideanDistance = sqrt(newI * newI + newJ * newJ);
                    
                    correctedDistance = euclideanDistance / correctionRadius;

                    if(correctedDistance == 0)
                    {
                        theta = 1;
                    }
                    else
                    {
                        theta = atan(correctedDistance) / correctedDistance;
                    }

                    srcLocationRow = halfHeight + theta * newI * zoom;
                    srcLocationColumn = halfWidth + theta * newJ * zoom;

                    if ((srcLocationRow >= 0) && (srcLocationColumn >= 0) && 
                        (srcLocationRow < srcSize.height) && (srcLocationColumn < srcSize.width))
                    {
                        srcLocationRowFloor = (Rpp32s) RPPFLOOR(srcLocationRow);
                        srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);

                        Rpp32f weightedHeight = srcLocationRow - srcLocationRowFloor;
                        if (srcLocationRowFloor > (srcSize.height - 2))
                        {
                            srcLocationRowFloor = srcSize.height - 2;
                        }

                        srcPtrTopRow = srcPtrTemp + srcLocationRowFloor * elementsInRow;
                        srcPtrBottomRow  = srcPtrTopRow + elementsInRow;

                        Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;
                        if (srcLocationColumnFloor > (srcSize.width - 2))
                        {
                            srcLocationColumnFloor = srcSize.width - 2;
                        }

                        Rpp32s srcLocColFloorChanneled = channel * srcLocationColumnFloor;

                        pixel = ((*(srcPtrTopRow + c + srcLocColFloorChanneled)) * (1 - weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrTopRow + c + srcLocColFloorChanneled + channel)) * (1 - weightedHeight) * (weightedWidth)) 
                            + ((*(srcPtrBottomRow + c + srcLocColFloorChanneled)) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrBottomRow + c + srcLocColFloorChanneled + channel)) * (weightedHeight) * (weightedWidth));
                        
                        *dstPtrTemp = (Rpp8u) round(pixel);
                    }
                    dstPtrTemp++;
                }
            }
        }
    }

    // Without bilinear interpolation
    /*
    int iSrc, jSrc, srcPosition;
    Rpp32u elementsPerChannel = srcSize.height * srcSize.width;
    Rpp32u elements = channel * elementsPerChannel;
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for(int c = 0; c < channel; c++)
        {
            srcPtrTemp = srcPtr + (c * srcSize.height * srcSize.width);
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    
                    newI = i - halfHeight;
                    newJ = j - halfWidth;

                    euclideanDistance = sqrt(newI * newI + newJ * newJ);
                    
                    correctedDistance = euclideanDistance / correctionRadius;

                    if(correctedDistance == 0)
                    {
                        theta = 1;
                    }
                    else
                    {
                        theta = atan(correctedDistance) / correctedDistance;
                    }

                    iSrc = (int) (halfHeight + theta * newI * zoom);
                    jSrc = (int) (halfWidth + theta * newJ * zoom);
                    
                    srcPosition = (int)((iSrc * srcSize.width) + jSrc);

                    if ((srcPosition >= 0) && (srcPosition < elementsPerChannel))
                    {
                        *dstPtrTemp = *(srcPtrTemp + srcPosition);
                    }
                    
                    dstPtrTemp++;
                }
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int i = 0; i < srcSize.height; i++)
        {
            for (int j = 0; j < srcSize.width; j++)
            {
                for(int c = 0; c < channel; c++)
                {
                    srcPtrTemp = srcPtr + c;

                    newI = i - halfHeight;
                    newJ = j - halfWidth;

                    euclideanDistance = sqrt(newI * newI + newJ * newJ);
                    
                    correctedDistance = euclideanDistance / correctionRadius;

                    if(correctedDistance == 0)
                    {
                        theta = 1;
                    }
                    else
                    {
                        theta = atan(correctedDistance) / correctedDistance;
                    }

                    iSrc = (int) (halfHeight + theta * newI * zoom);
                    jSrc = (int) (halfWidth + theta * newJ * zoom);
                    
                    srcPosition = (int)(channel * ((iSrc * srcSize.width) + jSrc));

                    if ((srcPosition >= 0) && (srcPosition < elements))
                    {
                        *dstPtrTemp = *(srcPtrTemp + srcPosition);
                    }
                    
                    dstPtrTemp++;
                }
            }
        }
    }
     */
    return RPP_SUCCESS;
}