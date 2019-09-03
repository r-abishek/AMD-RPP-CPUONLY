#include <cpu/rpp_cpu_common.hpp>
#include <algorithm>

template <typename T>
RppStatus nonlinear_filter_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    Rpp32u kernelSize,
                    RppiChnFormat chnFormat, Rpp32u channel)
//O(1)
/*
{
    if (kernelSize % 2 == 0)
    {
        return RPP_ERROR;
    }
    int bound = ((kernelSize - 1) / 2);
    Rpp32f limit = (kernelSize * kernelSize) / 2;

    RppiSize srcSizeMod;
    srcSizeMod.width = srcSize.width + (2 * bound);
    srcSizeMod.height = srcSize.height + (2 * bound);
    Rpp8u *srcPtrMod = (Rpp8u *)calloc(srcSizeMod.height * srcSizeMod.width * channel, sizeof(Rpp8u));

    generate_evenly_padded_image_host(srcPtr, srcSize, srcPtrMod, srcSizeMod, chnFormat, channel);
    
    T *srcPtrWindow, *dstPtrTemp;
    srcPtrWindow = srcPtrMod;
    dstPtrTemp = dstPtr;

    T *srcPtrWindowTemp;
    int count = 0;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        Rpp32u remainingElementsInRow = srcSizeMod.width - kernelSize;
        Rpp32u increment = kernelSize - 1;
        Rpp32u incrementToNewRow = increment * srcSizeMod.width;
        Rpp32u columnHistogramSize = 256 * srcSizeMod.width;
        Rpp32u *columnHistogram = (Rpp32u *)calloc(columnHistogramSize, sizeof(Rpp32u));
        Rpp32u *histogram = (Rpp32u *)calloc(256, sizeof(Rpp32u));
        Rpp32u *histogramTemp, *columnHistogramBegin, *columnHistogramTemp, *columnHistogramTemp2;
        Rpp32u sum;

        //displayHistogram(histogram, 256);

        for (int c = 0; c < channel; c++)
        {
            memset(histogram, 0, 256 * sizeof(Rpp32u));
            memset(columnHistogram, 0, columnHistogramSize * sizeof(Rpp32u));

            srcPtrWindowTemp = srcPtrWindow;

            for (int i = 0; i < kernelSize; i++)
            {
                columnHistogramBegin = columnHistogram;
                for (int j = 0; j < srcSizeMod.width; j++)
                {
                    *(columnHistogramBegin + (Rpp32u) *(srcPtrWindowTemp)) += 1;
                    srcPtrWindowTemp++;
                    columnHistogramBegin += 256;
                }
            }

            columnHistogramTemp = columnHistogram;
            for (int i = 0; i < kernelSize; i++)
            {
                histogramTemp = histogram;
                for (int j = 0; j < 256; j++)
                {
                    *histogramTemp += *columnHistogramTemp;
                    histogramTemp++;
                    columnHistogramTemp++;
                }    
            }

            //displayHistogram(histogram, 256);

            sum = 0;
            histogramTemp = histogram;
            while (sum < limit)
            {
                sum += *(histogramTemp);
                histogramTemp++;
            }
            *dstPtrTemp = (T) (histogramTemp - histogram);
            srcPtrWindow++;
            dstPtrTemp++;

            for (int i = 1; i < srcSize.width; i++)
            {
                median_filter_row_kernel_host(srcPtrWindow, dstPtrTemp, srcSize, 
                                              kernelSize, remainingElementsInRow, limit, histogram, 
                                              chnFormat, channel);
                //displayHistogram(histogram, 256);
                srcPtrWindow++;
                dstPtrTemp++;
            }
            srcPtrWindow += increment;

            





            for (int i = 1; i < srcSize.height; i++)
            {
                memset(histogram, 0, 256 * sizeof(Rpp32u));
                srcPtrWindowTemp = srcPtrWindow;
                columnHistogramBegin = columnHistogram;
                for (int j = 0; j < kernelSize; j++)
                {
                    *(columnHistogramBegin + *(srcPtrWindowTemp - srcSizeMod.width)) -= 1;
                    *(columnHistogramBegin + *(srcPtrWindowTemp + incrementToNewRow)) += 1;
                    columnHistogramBegin += 256;
                    srcPtrWindowTemp++;
                }

                columnHistogramTemp = columnHistogram;
                for (int i = 0; i < kernelSize; i++)
                {
                    histogramTemp = histogram;
                    for (int j = 0; j < 256; j++)
                    {
                        *histogramTemp += *columnHistogramTemp;
                        histogramTemp++;
                        columnHistogramTemp++;
                    }
                }
                //displayHistogram(histogram, 256);
                
                sum = 0;
                histogramTemp = histogram;
                while (sum < limit)
                {
                    sum += *(histogramTemp);
                    histogramTemp++;
                }
                *dstPtrTemp = (T) (histogramTemp - histogram);
                srcPtrWindow++;
                dstPtrTemp++;
                






                for (int j = 1; j < srcSize.width; j++)
                {
                    columnHistogramBegin = columnHistogram + ((j + increment) * 256);
                    *(columnHistogramBegin + *(srcPtrWindow + increment - srcSizeMod.width)) -= 1;
                    *(columnHistogramBegin + *(srcPtrWindow + (increment * (1 + srcSizeMod.width)))) += 1;

                    histogramTemp = histogram;
                    columnHistogramTemp = columnHistogramBegin;
                    columnHistogramTemp2 = columnHistogram + ((j - 1) * 256);
                    for (int k = 0; k < 256; k++)
                    {
                        *histogramTemp = *histogramTemp + *columnHistogramTemp - *columnHistogramTemp2;
                        histogramTemp++;
                        columnHistogramTemp++;
                        columnHistogramTemp2++;
                    }

                    sum = 0;
                    histogramTemp = histogram;
                    while (sum < limit)
                    {
                        sum += *(histogramTemp);
                        histogramTemp++;
                    }
                    *dstPtrTemp = (T) (histogramTemp - histogram);
                    srcPtrWindow++;
                    dstPtrTemp++;
                }
                srcPtrWindow += increment;
            }
            srcPtrWindow += (increment * srcSizeMod.width);
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        
    }
    
    return RPP_SUCCESS;
}
*/



//O(r)
///*
{
    if (kernelSize % 2 == 0)
    {
        return RPP_ERROR;
    }
    int bound = ((kernelSize - 1) / 2);
    Rpp32f limit = (kernelSize * kernelSize) / 2;

    RppiSize srcSizeMod;
    srcSizeMod.width = srcSize.width + (2 * bound);
    srcSizeMod.height = srcSize.height + (2 * bound);
    Rpp8u *srcPtrMod = (Rpp8u *)calloc(srcSizeMod.height * srcSizeMod.width * channel, sizeof(Rpp8u));

    generate_evenly_padded_image_host(srcPtr, srcSize, srcPtrMod, srcSizeMod, chnFormat, channel);
    
    T *srcPtrWindow, *dstPtrTemp;
    srcPtrWindow = srcPtrMod;
    dstPtrTemp = dstPtr;

    T *srcPtrWindowTemp;
    int count = 0;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        Rpp32u remainingElementsInRow = srcSizeMod.width - kernelSize;
        Rpp32u increment = kernelSize - 1;
        Rpp32u *histogram = (Rpp32u *)calloc(256, sizeof(Rpp32u));

        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height; i++)
            {
                memset(histogram, 0, 256 * sizeof(Rpp32u));
                srcPtrWindowTemp = srcPtrWindow;
                for (int j = 0; j < kernelSize; j++)
                {
                    for (int k = 0; k < kernelSize; k++)
                    {
                        *(histogram + (Rpp32u) *(srcPtrWindowTemp)) += 1;
                        srcPtrWindowTemp++;
                    }
                    srcPtrWindowTemp += remainingElementsInRow;
                }
                Rpp32u sum = 0;
                Rpp32u *histogramTemp;
                histogramTemp = histogram;
                while (sum < limit)
                {
                    sum += *(histogramTemp);
                    histogramTemp++;
                }
                *dstPtrTemp = (T) (histogramTemp - histogram);
                srcPtrWindow++;
                dstPtrTemp++;

                for (int j = 1; j < srcSize.width; j++)
                {
                    median_filter_kernel_host(srcPtrWindow, dstPtrTemp, srcSize, 
                                      kernelSize, remainingElementsInRow, limit, histogram, 
                                      chnFormat, channel);
                    srcPtrWindow++;
                    dstPtrTemp++;
                }
                srcPtrWindow += increment;
            }
            srcPtrWindow += (increment * srcSizeMod.width);
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        Rpp32u remainingElementsInRow = (srcSizeMod.width - kernelSize) * channel;
        Rpp32u increment = (kernelSize - 1) * channel;
        Rpp32u *histogram = (Rpp32u *)calloc(256 * channel, sizeof(Rpp32u));

        for (int i = 0; i < srcSize.height; i++)
        {
            memset(histogram, 0, 256 * channel * sizeof(Rpp32u));
            srcPtrWindowTemp = srcPtrWindow;
            for (int j = 0; j < kernelSize; j++)
            {
                for (int k = 0; k < kernelSize; k++)
                {
                    for (int c = 0; c < channel; c++)
                    {
                        *(histogram + (c * 256) + (Rpp32u) *(srcPtrWindowTemp)) += 1;
                        srcPtrWindowTemp++;
                    }
                }
                srcPtrWindowTemp += remainingElementsInRow;
            }

            Rpp32u sum;
            Rpp32u *histogramTemp, *histogramBegin;

            for (int c = 0; c < channel; c++)
            {
                sum = 0;
                histogramBegin = histogram + (c * 256);
                histogramTemp = histogramBegin;
                while (sum < limit)
                {
                    sum += *(histogramTemp);
                    histogramTemp++;
                }
                *dstPtrTemp = (T) (histogramTemp - histogramBegin);
                srcPtrWindow++;
                dstPtrTemp++;
            }
            for (int j = 1; j < srcSize.width; j++)
            {
                for (int c = 0; c < channel; c++)
                {   
                    histogramBegin = histogram + (c * 256);
                    median_filter_kernel_host(srcPtrWindow, dstPtrTemp, srcSize, 
                                      kernelSize, remainingElementsInRow, limit, histogramBegin, 
                                      chnFormat, channel);
                    srcPtrWindow++;
                    dstPtrTemp++;
                }
            }
            srcPtrWindow += increment;
        }
    }
    
    return RPP_SUCCESS;
}
//*/


//O(r^2)
/*
{
    if (kernelSize % 2 == 0)
    {
        return RPP_ERROR;
    }
    int bound = ((kernelSize - 1) / 2);

    RppiSize srcSizeMod;
    srcSizeMod.width = srcSize.width + (2 * bound);
    srcSizeMod.height = srcSize.height + (2 * bound);
    Rpp8u *srcPtrMod = (Rpp8u *)calloc(srcSizeMod.height * srcSizeMod.width * channel, sizeof(Rpp8u));

    generate_evenly_padded_image_host(srcPtr, srcSize, srcPtrMod, srcSizeMod, chnFormat, channel);
    
    Rpp32u remainingElementsInRowPlanar = srcSizeMod.width - kernelSize;
    Rpp32u remainingElementsInRowPacked = (srcSizeMod.width - kernelSize) * channel;
    
    T *srcPtrWindow, *dstPtrTemp;
    srcPtrWindow = srcPtrMod;
    dstPtrTemp = dstPtr;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    median_filter_kernel_host(srcPtrWindow, dstPtrTemp, srcSize, 
                                      kernelSize, remainingElementsInRowPlanar, 
                                      chnFormat, channel);
                    srcPtrWindow++;
                    dstPtrTemp++;
                }
                srcPtrWindow += (kernelSize - 1);
            }
            srcPtrWindow += ((kernelSize - 1) * srcSizeMod.width);
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int i = 0; i < srcSize.height; i++)
        {
            for (int j = 0; j < srcSize.width; j++)
            {
                for (int c = 0; c < channel; c++)
                {   
                    median_filter_kernel_host(srcPtrWindow, dstPtrTemp, srcSize, 
                                      kernelSize, remainingElementsInRowPacked, 
                                      chnFormat, channel);
                    srcPtrWindow++;
                    dstPtrTemp++;
                }
            }
            srcPtrWindow += ((kernelSize - 1) * channel);
        }
    }
    
    return RPP_SUCCESS;
}
*/
