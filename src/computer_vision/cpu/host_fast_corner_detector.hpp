#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus fast_corner_detector_host(T* srcPtr, RppiSize srcSize, T* dstPtr, 
                                      Rpp32u numOfPixels, T threshold, 
                                      Rpp32u nonmaxKernelSize, 
                                      RppiChnFormat chnFormat, Rpp32u channel)
{
    // RGB to Greyscale Conversion

    Rpp32u imageDim = srcSize.height * srcSize.width;
    Rpp32u twiceImageDim = 2 * imageDim;

    T *srcPtrGreyscale = (T *)calloc(imageDim, sizeof(T));
    T *srcPtrGreyscaleTemp;
    srcPtrGreyscaleTemp = srcPtrGreyscale;

    if (channel == 3)
    {
        if (chnFormat == RPPI_CHN_PLANAR)
        {
            T *srcPtrTempR, *srcPtrTempG, *srcPtrTempB;
            srcPtrTempR = srcPtr;
            srcPtrTempG = srcPtr + imageDim;
            srcPtrTempB = srcPtrTempG + imageDim;

            for (int i = 0; i < imageDim; i++)
            {
                *srcPtrGreyscaleTemp = (T) (((Rpp32u)(*srcPtrTempR) + (Rpp32u)(*srcPtrTempG) + (Rpp32u)(*srcPtrTempB)) / 3);
                srcPtrGreyscaleTemp++;
                srcPtrTempR++;
                srcPtrTempG++;
                srcPtrTempB++;
            }
        }
        else if (chnFormat == RPPI_CHN_PACKED)
        {
            T *srcPtrTempR, *srcPtrTempG, *srcPtrTempB;
            srcPtrTempR = srcPtr;
            srcPtrTempG = srcPtr + 1;
            srcPtrTempB = srcPtrTempG + 1;
            
            for (int i = 0; i < imageDim; i++)
            {
                *srcPtrGreyscaleTemp = (T) (((Rpp32u)(*srcPtrTempR) + (Rpp32u)(*srcPtrTempG) + (Rpp32u)(*srcPtrTempB)) / 3);
                srcPtrGreyscaleTemp++;
                srcPtrTempR += channel;
                srcPtrTempG += channel;
                srcPtrTempB += channel;
            }
        }
    }
    else if (channel == 1)
    {
        memcpy(srcPtrGreyscale, srcPtr, imageDim * sizeof(T));
    }

    Rpp32u newChannel = 1;

    // Pad image
    
    int bound = 3;
    RppiSize srcSizeMod;
    srcSizeMod.height = srcSize.height + (2 * bound);
    srcSizeMod.width = srcSize.width + (2 * bound);
    
    T *srcPtrMod = (T *)calloc(srcSizeMod.height * srcSizeMod.width * newChannel, sizeof(T));
    generate_evenly_padded_image_host(srcPtrGreyscale, srcSize, srcPtrMod, srcSizeMod, RPPI_CHN_PLANAR, newChannel);
    
    // Compute the fast corner strengh matrix
    
    Rpp32u kernelSize = 7;
    T *dstPtrGreyscale = (T *)calloc(srcSize.height * srcSize.width * newChannel, sizeof(T));
    T *dstPtrGreyscaleTemp;
    dstPtrGreyscaleTemp = dstPtrGreyscale;

    T *srcPtrWindow;
    srcPtrWindow = srcPtrMod;

    Rpp32u *bresenhamCirclePositions = (Rpp32u*) calloc(16, sizeof(Rpp32u));
    Rpp32u *bresenhamCirclePositionsTemp;
    bresenhamCirclePositionsTemp = bresenhamCirclePositions;

    *bresenhamCirclePositionsTemp = 3;
    bresenhamCirclePositionsTemp++;
    *bresenhamCirclePositionsTemp = 4;
    bresenhamCirclePositionsTemp++;
    *bresenhamCirclePositionsTemp = (1 * srcSizeMod.width) + 5;
    bresenhamCirclePositionsTemp++;
    *bresenhamCirclePositionsTemp = (2 * srcSizeMod.width) + 6;
    bresenhamCirclePositionsTemp++;
    *bresenhamCirclePositionsTemp = (3 * srcSizeMod.width) + 6;
    bresenhamCirclePositionsTemp++;
    *bresenhamCirclePositionsTemp = (4 * srcSizeMod.width) + 6;
    bresenhamCirclePositionsTemp++;
    *bresenhamCirclePositionsTemp = (5 * srcSizeMod.width) + 5;
    bresenhamCirclePositionsTemp++;
    *bresenhamCirclePositionsTemp = (6 * srcSizeMod.width) + 4;
    bresenhamCirclePositionsTemp++;
    *bresenhamCirclePositionsTemp = (6 * srcSizeMod.width) + 3;
    bresenhamCirclePositionsTemp++;
    *bresenhamCirclePositionsTemp = (6 * srcSizeMod.width) + 2;
    bresenhamCirclePositionsTemp++;
    *bresenhamCirclePositionsTemp = (5 * srcSizeMod.width) + 1;
    bresenhamCirclePositionsTemp++;
    *bresenhamCirclePositionsTemp = (4 * srcSizeMod.width);
    bresenhamCirclePositionsTemp++;
    *bresenhamCirclePositionsTemp = (3 * srcSizeMod.width);
    bresenhamCirclePositionsTemp++;
    *bresenhamCirclePositionsTemp = (2 * srcSizeMod.width);
    bresenhamCirclePositionsTemp++;
    *bresenhamCirclePositionsTemp = (1 * srcSizeMod.width) + 1;
    bresenhamCirclePositionsTemp++;
    *bresenhamCirclePositionsTemp = 2;

    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            fast_corner_detector_kernel_host(srcPtrWindow, dstPtrGreyscaleTemp, srcSizeMod, 
                                             bresenhamCirclePositions, threshold, numOfPixels);
            srcPtrWindow++;
            dstPtrGreyscaleTemp++;
        }
        srcPtrWindow += (kernelSize - 1);
    }

    //Rpp32u *dstPtrGreyscale32u = (Rpp32u *)calloc(srcSize.height * srcSize.width * newChannel, sizeof(Rpp32u));
    //Rpp32u *dstPtrGreyscale32uTemp;
    //dstPtrGreyscale32uTemp = dstPtrGreyscale32u;
    //dstPtrGreyscaleTemp = dstPtrGreyscale;
    //
    //for (int i = 0; i < imageDim; i++)
    //{
    //    *dstPtrGreyscale32uTemp = (Rpp32u) *dstPtrGreyscale;
    //    dstPtrGreyscale32uTemp++;
    //    dstPtrGreyscale++;
    //}

    // Create score function

    generate_evenly_padded_image_host(dstPtrGreyscale, srcSize, srcPtrMod, srcSizeMod, RPPI_CHN_PLANAR, newChannel);
    
    Rpp32u *dstPtrGreyscale32u = (Rpp32u *)calloc(srcSize.height * srcSize.width * newChannel, sizeof(Rpp32u));
    Rpp32u *dstPtrGreyscale32uTemp;
    Rpp32u windowCenter;
    
    srcPtrWindow = srcPtrMod;
    dstPtrGreyscale32uTemp = dstPtrGreyscale32u;
    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            windowCenter = (Rpp32u) *(srcPtrWindow + (3 * srcSizeMod.width) + 3);
            //fast_corner_detector_score_function_kernel_host(srcPtrWindow, dstPtrGreyscale32uTemp, srcSize, bresenhamCirclePositions, windowCenter);
            if (windowCenter != 0)
            {
                fast_corner_detector_score_function_kernel_host(srcPtrWindow, dstPtrGreyscale32uTemp, srcSize, bresenhamCirclePositions, windowCenter);
            }
            else
            {
                *dstPtrGreyscale32uTemp = (Rpp32u) 0;
            }
            srcPtrWindow++;
            dstPtrGreyscale32uTemp++;
        }
        srcPtrWindow += (kernelSize - 1);
    }
    
    // Apply non max suppression
    
    int nonmaxBound = (nonmaxKernelSize - 1) / 2;
    RppiSize srcSizeNonmaxMod;
    srcSizeNonmaxMod.height = srcSize.height + (2 * nonmaxBound);
    srcSizeNonmaxMod.width = srcSize.width + (2 * nonmaxBound);
    
    Rpp32u *dstPtrGreyscale32uMod = (Rpp32u *)calloc(srcSizeNonmaxMod.height * srcSizeNonmaxMod.width * newChannel, sizeof(Rpp32u));
    generate_evenly_padded_image_host(dstPtrGreyscale32u, srcSize, dstPtrGreyscale32uMod, srcSizeNonmaxMod, RPPI_CHN_PLANAR, newChannel);
    
    Rpp32u *dstPtrGreyscale32uWindow;
    dstPtrGreyscale32uWindow = dstPtrGreyscale32uMod;
    dstPtrGreyscale32uTemp = dstPtrGreyscale32u;
    
    Rpp32u windowCenterPosIncrement = (nonmaxBound * srcSizeNonmaxMod.width) + nonmaxBound;
    Rpp32u remainingElementsInRow = srcSizeNonmaxMod.width - nonmaxKernelSize;
    
    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            windowCenter = (Rpp32u) *(dstPtrGreyscale32uWindow + windowCenterPosIncrement);
            non_max_suppression_kernel_host(dstPtrGreyscale32uWindow, dstPtrGreyscale32uTemp, srcSize, 
                                nonmaxKernelSize, remainingElementsInRow, windowCenter, 
                                RPPI_CHN_PLANAR, newChannel);
            dstPtrGreyscale32uWindow++;
            dstPtrGreyscale32uTemp++;
        }
        dstPtrGreyscale32uWindow += (nonmaxKernelSize - 1);
    }

    // Overlay Fast Corners on original image - Large Dot

    memcpy(dstPtr, srcPtr, channel * imageDim * sizeof(T));
    
    T *dstPtrWindow;
    kernelSize = 3;
    bound = 1;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        dstPtrGreyscale32uTemp = dstPtrGreyscale32u + (bound * srcSize.width) + bound;
        dstPtrWindow = dstPtr;
        Rpp32u remainingElementsInRow = srcSize.width - kernelSize;
        for (int i = (2 * bound); i < srcSize.height; i++)
        {
            for (int j = (2 * bound); j < srcSize.width; j++)
            {
                if (*dstPtrGreyscale32uTemp != 0)
                {
                    if (channel == 3)
                    {
                        harris_corner_set_minimum_kernel_host(dstPtrWindow, kernelSize, remainingElementsInRow, 
                                                              chnFormat, channel);
                        harris_corner_set_minimum_kernel_host(dstPtrWindow + imageDim, kernelSize, remainingElementsInRow, 
                                                              chnFormat, channel);
                        harris_corner_set_maximum_kernel_host(dstPtrWindow + twiceImageDim, kernelSize, remainingElementsInRow, 
                                                              chnFormat, channel);
                    }
                    else if (channel == 1)
                    {
                        harris_corner_set_maximum_kernel_host(dstPtrWindow, kernelSize, remainingElementsInRow, 
                                                              chnFormat, channel);
                    }
                }
                dstPtrGreyscale32uTemp++;
                dstPtrWindow++;
            }
            dstPtrGreyscale32uTemp += (kernelSize - 1);
            dstPtrWindow += (kernelSize - 1);
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        dstPtrGreyscale32uTemp = dstPtrGreyscale32u + (channel * ((bound * srcSize.width) + bound));
        dstPtrWindow = dstPtr;
        Rpp32u remainingElementsInRow = channel * (srcSize.width - kernelSize);
        Rpp32u increment = channel * (kernelSize - 1);
        for (int i = (2 * bound); i < srcSize.height; i++)
        {
            for (int j = (2 * bound); j < srcSize.width; j++)
            {
                if (*dstPtrGreyscale32uTemp != 0)
                {
                    if (channel == 3)
                    {
                        harris_corner_set_minimum_kernel_host(dstPtrWindow, kernelSize, remainingElementsInRow, 
                                                              chnFormat, channel);
                        harris_corner_set_minimum_kernel_host(dstPtrWindow + 1, kernelSize, remainingElementsInRow, 
                                                              chnFormat, channel);
                        harris_corner_set_maximum_kernel_host(dstPtrWindow + 2, kernelSize, remainingElementsInRow, 
                                                              chnFormat, channel);
                    }
                    else if (channel == 1)
                    {
                        harris_corner_set_maximum_kernel_host(dstPtrWindow, kernelSize, remainingElementsInRow, 
                                                              chnFormat, channel);
                    }
                }
                dstPtrGreyscale32uTemp++;
                dstPtrWindow += channel;
            }
            dstPtrGreyscale32uTemp += (kernelSize - 1);
            dstPtrWindow += increment;
        }
    }

    // Overlay Fast Corners on original image - Small Dot

    //memcpy(dstPtr, srcPtr, channel * imageDim * sizeof(T));
    //
    //T *dstPtrTemp;
    //dstPtrTemp = dstPtr;
    //dstPtrGreyscale32uTemp = dstPtrGreyscale32u;
    //
    //if (channel == 1)
    //{
    //    for (int i = 0; i < imageDim; i++)
    //    {
    //        if (*dstPtrGreyscale32uTemp != 0)
    //        {
    //            *dstPtrTemp = (T) 255;
    //        }
    //        dstPtrGreyscale32uTemp++;
    //        dstPtrTemp++;
    //    }
    //}
    //else if (channel == 3)
    //{
    //    if (chnFormat == RPPI_CHN_PLANAR)
    //    {
    //        for (int c = 0; c < 2; c++)
    //        {
    //            dstPtrGreyscale32uTemp = dstPtrGreyscale32u;
    //            for (int i = 0; i < srcSize.height; i++)
    //            {
    //                for (int j = 0; j < srcSize.width; j++)
    //                {
    //                    if (*dstPtrGreyscale32uTemp != 0)
    //                    {
    //                        *dstPtrTemp = (T) 0;
    //                    }
    //                    dstPtrGreyscale32uTemp++;
    //                    dstPtrTemp++;
    //                }
    //            }
    //        }
    //        dstPtrGreyscale32uTemp = dstPtrGreyscale32u;
    //        for (int i = 0; i < srcSize.height; i++)
    //        {
    //            for (int j = 0; j < srcSize.width; j++)
    //            {
    //                if (*dstPtrGreyscale32uTemp != 0)
    //                {
    //                    *dstPtrTemp = (T) 255;
    //                }
    //                dstPtrGreyscale32uTemp++;
    //                dstPtrTemp++;
    //            }
    //        }
    //    }
    //    else if (chnFormat == RPPI_CHN_PACKED)
    //    {
    //        for (int i = 0; i < srcSize.height; i++)
    //        {
    //            for (int j = 0; j < srcSize.width; j++)
    //            {
    //                if (*dstPtrGreyscale32uTemp != 0)
    //                {
    //                    *dstPtrTemp = (T) 0;
    //                    dstPtrTemp++;
    //                    *dstPtrTemp = (T) 0;
    //                    dstPtrTemp++;
    //                    *dstPtrTemp = (T) 255;
    //                    dstPtrTemp++;
    //                }
    //                dstPtrGreyscale32uTemp++;
    //            }
    //        }
    //    }
    //}

    

    return RPP_SUCCESS;
}