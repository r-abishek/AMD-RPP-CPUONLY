#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus scale_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           Rpp32f percentage,
                           RppiChnFormat chnFormat, Rpp32u channel)
{
    if (dstSize.height < 0 || dstSize.width < 0)
    {
        return RPP_ERROR;
    }
    if (percentage < 0)
    {
        return RPP_ERROR;
    }

    percentage /= 100;

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

                srcPtrTopRow = srcPtrTemp + srcLocationRowFloor * srcSize.width;
                srcPtrBottomRow  = srcPtrTopRow + srcSize.width;
                
                for (int j = 0; j < dstSize.width; j++)
                {
                    srcLocationColumn = ((Rpp32f) j) / percentage;
                    srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);
                    Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;

                    if (srcLocationRow < 0 || srcLocationColumn < 0 || srcLocationRow > (srcSize.height - 2) || srcLocationColumn > (srcSize.width - 2))
                    {
                        *dstPtrTemp = (T) 0;
                        dstPtrTemp++;
                    }
                    else
                    {
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
            srcLocationRow = ((Rpp32f) i) / percentage;
            srcLocationRowFloor = (Rpp32s) RPPFLOOR(srcLocationRow);
            Rpp32f weightedHeight = srcLocationRow - srcLocationRowFloor;

            srcPtrTopRow = srcPtrTemp + srcLocationRowFloor * elementsInRow;
            srcPtrBottomRow  = srcPtrTopRow + elementsInRow;

            for (int j = 0; j < dstSize.width; j++)
            {   
                srcLocationColumn = ((Rpp32f) j) / percentage;
                srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);
                Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;

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














//RppStatus scale_output_size_host(RppiSize srcSize, RppiSize *dstSizePtr,
//                                  Rpp32f percentage)
//{
//    if (percentage < 0)
//    {
//        return RPP_ERROR;
//    }
//    percentage /= 100;
//    dstSizePtr->height = (Rpp32s) (percentage * (Rpp32f) srcSize.height);
//    dstSizePtr->width = (Rpp32s) (percentage * (Rpp32f) srcSize.width);
//
//    return RPP_SUCCESS;
//}

//template <typename T>
//RppStatus scale_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
//                           Rpp32f percentage,
//                           RppiChnFormat chnFormat, Rpp32u channel)
//{
//    resize_kernel_host(srcPtr, srcSize, dstPtr, dstSize, chnFormat, channel);
//
//    return RPP_SUCCESS;
//}
