#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus resizeCrop_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
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

    RppiSize srcNewSize;
    int xDiff = (int) x2 - (int) x1;
    int yDiff = (int) y2 - (int) y1;
    srcNewSize.width = (Rpp32u) RPPABS(xDiff);
    srcNewSize.height = (Rpp32u) RPPABS(yDiff);
    
    Rpp8u *srcNewPtr = (Rpp8u *)calloc(channel * srcNewSize.height * srcNewSize.width, sizeof(Rpp8u));

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = RPPMIN2(y1, y2), m = 0; i < RPPMAX2(y1, y2); i++, m++)
            {
                for (int j = RPPMIN2(x1, x2), n = 0; j < RPPMAX2(x1, x2); j++, n++)
                {
                    *(srcNewPtr + (c * srcNewSize.height * srcNewSize.width) + (m * srcNewSize.width) + n) 
                        = *(srcPtr + (c * srcSize.height * srcSize.width) + (i * srcSize.width) + j);
                }
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int i = RPPMIN2(y1, y2), m = 0; i < RPPMAX2(y1, y2); i++, m++)
        {
            for (int j = RPPMIN2(x1, x2), n = 0; j < RPPMAX2(x1, x2); j++, n++)
            {
                for (int c = 0; c < channel; c++)
                {
                    *(srcNewPtr + c + (channel * m * srcNewSize.width) + (channel * n)) 
                        = *(srcPtr + c + (channel * i * srcSize.width) + (channel * j));
                }
            }
        }
    }

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

    return RPP_SUCCESS;
    
}