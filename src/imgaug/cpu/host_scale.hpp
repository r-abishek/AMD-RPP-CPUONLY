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
 
    return RPP_SUCCESS;
}