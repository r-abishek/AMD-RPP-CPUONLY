#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus harris_corner_detector_host(T* srcPtr, RppiSize srcSize, T* dstPtr, 
                                      Rpp32u gaussianKernelSize, Rpp32f stdDev, 
                                      Rpp32u kernelSize, Rpp32f kValue, Rpp32f threshold, 
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







    // Gaussian Filter
    
    Rpp32f *gaussianKernel = (Rpp32f *)calloc(gaussianKernelSize * gaussianKernelSize, sizeof(Rpp32f));
    int gaussianBound = ((gaussianKernelSize - 1) / 2);
    
    generate_gaussian_kernel_host(stdDev, gaussianKernel, gaussianKernelSize);
    
    RppiSize srcSizeMod;
    srcSizeMod.width = srcSize.width + (2 * gaussianBound);
    srcSizeMod.height = srcSize.height + (2 * gaussianBound);
    T *srcPtrGaussianPadded = (T *)calloc(srcSizeMod.height * srcSizeMod.width * newChannel, sizeof(T));
    
    generate_evenly_padded_image_host(srcPtrGreyscale, srcSize, srcPtrGaussianPadded, srcSizeMod, chnFormat, newChannel);
    
    RppiSize rppiGaussianKernelSize;
    rppiGaussianKernelSize.height = gaussianKernelSize;
    rppiGaussianKernelSize.width = gaussianKernelSize;
    convolve_image_host(srcPtrGaussianPadded, srcSizeMod, srcPtrGreyscale, srcSize, gaussianKernel, rppiGaussianKernelSize, chnFormat, newChannel);






    // Sobel Filter
    
    RppiSize rppiSobelKernelSize;
    Rpp32u sobelKernelSize = 3;
    int sobelBound = (sobelKernelSize - 1) / 2;

    srcSizeMod.width = srcSize.width + (2 * sobelBound);
    srcSizeMod.height = srcSize.height + (2 * sobelBound);

    T *srcPtrMod = (T *)calloc(srcSizeMod.height * srcSizeMod.width * newChannel, sizeof(T));
    generate_evenly_padded_image_host(srcPtrGreyscale, srcSize, srcPtrMod, srcSizeMod, chnFormat, newChannel);

    rppiSobelKernelSize.height = sobelKernelSize;
    rppiSobelKernelSize.width = sobelKernelSize;

    Rpp32f *kernelX = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));
    generate_sobel_kernel_host(kernelX, 1);
    T *srcPtrDerivativeX = (T *)calloc(srcSize.height * srcSize.width * newChannel, sizeof(T));
    convolve_image_host(srcPtrMod, srcSizeMod, srcPtrDerivativeX, srcSize, kernelX, rppiSobelKernelSize, chnFormat, newChannel);

    Rpp32f *kernelY = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));
    generate_sobel_kernel_host(kernelY, 2);
    T *srcPtrDerivativeY = (T *)calloc(srcSize.height * srcSize.width * newChannel, sizeof(T));
    convolve_image_host(srcPtrMod, srcSizeMod, srcPtrDerivativeY, srcSize, kernelY, rppiSobelKernelSize, chnFormat, newChannel);
    
    
    
    
    
    
    // Pad x and y gradient images
    
    int bound = (kernelSize - 1) / 2;
    RppiSize srcSizeDerivativeMod;
    srcSizeDerivativeMod.height = srcSize.height + (2 * bound);
    srcSizeDerivativeMod.width = srcSize.width + (2 * bound);

    T *srcPtrDerivativeXmod = (T *)calloc(srcSizeDerivativeMod.height * srcSizeDerivativeMod.width * newChannel, sizeof(T));
    generate_evenly_padded_image_host(srcPtrDerivativeX, srcSize, srcPtrDerivativeXmod, srcSizeDerivativeMod, chnFormat, newChannel);

    T *srcPtrDerivativeYmod = (T *)calloc(srcSizeDerivativeMod.height * srcSizeDerivativeMod.width * newChannel, sizeof(T));
    generate_evenly_padded_image_host(srcPtrDerivativeY, srcSize, srcPtrDerivativeYmod, srcSizeDerivativeMod, chnFormat, newChannel);







    // Compute the harris corner strengh matrix
    
    Rpp32f *dstPtrGreyscaleFloat = (Rpp32f *)calloc(srcSize.height * srcSize.width * newChannel, sizeof(Rpp32f));
    Rpp32f *dstPtrGreyscaleFloatTemp;
    dstPtrGreyscaleFloatTemp = dstPtrGreyscaleFloat;

    T *srcPtrWindowX, *srcPtrWindowY;
    srcPtrWindowX = srcPtrDerivativeXmod;
    srcPtrWindowY = srcPtrDerivativeYmod;
    
    Rpp32u remainingElementsInRow = srcSizeDerivativeMod.width - kernelSize;
    //Rpp32f min = 65535, max = -65535;

    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            //harris_corner_detector_kernel_host(srcPtrWindowX, srcPtrWindowY, dstPtrGreyscaleFloatTemp, srcSize, 
            //                                    kernelSize, remainingElementsInRow, kValue, &min, &max, threshold, 
            //                                    chnFormat, channel);
            harris_corner_detector_kernel_host(srcPtrWindowX, srcPtrWindowY, dstPtrGreyscaleFloatTemp, srcSize, 
                                                kernelSize, remainingElementsInRow, kValue, threshold, 
                                                chnFormat, channel);
            srcPtrWindowX++;
            srcPtrWindowY++;
            dstPtrGreyscaleFloatTemp++;
        }
        srcPtrWindowX += (kernelSize - 1);
        srcPtrWindowY += (kernelSize - 1);
    }
    //printf("\nMax = %0.4f, Min = %0.4f", max, min);

    int nonmaxBound = (nonmaxKernelSize - 1) / 2;
    RppiSize srcSizeNonmaxMod;
    srcSizeNonmaxMod.height = srcSize.height + (2 * nonmaxBound);
    srcSizeNonmaxMod.width = srcSize.width + (2 * nonmaxBound);
    
    Rpp32f *dstPtrGreyscaleFloatMod = (Rpp32f *)calloc(srcSizeNonmaxMod.height * srcSizeNonmaxMod.width * newChannel, sizeof(Rpp32f));
    generate_evenly_padded_image_host(dstPtrGreyscaleFloat, srcSize, dstPtrGreyscaleFloatMod, srcSizeNonmaxMod, chnFormat, newChannel);
    
    Rpp32f *dstPtrGreyscaleWindow;
    Rpp32f windowCenter;
    dstPtrGreyscaleWindow = dstPtrGreyscaleFloatMod;
    dstPtrGreyscaleFloatTemp = dstPtrGreyscaleFloat;
    
    Rpp32u windowCenterPosIncrement = (nonmaxBound * srcSizeNonmaxMod.width) + nonmaxBound;

    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            windowCenter = (Rpp32f) *(dstPtrGreyscaleWindow + windowCenterPosIncrement);
            non_max_suppression_kernel_host(dstPtrGreyscaleWindow, dstPtrGreyscaleFloatTemp, srcSize, 
                                nonmaxKernelSize, remainingElementsInRow, windowCenter, 
                                RPPI_CHN_PLANAR, newChannel);
            dstPtrGreyscaleWindow++;
            dstPtrGreyscaleFloatTemp++;
        }
        dstPtrGreyscaleWindow += (nonmaxKernelSize - 1);
    }









    // Generate binary image (0 or 255)

    // T *dstPtrGreyscale = (T *)calloc(srcSize.height * srcSize.width * newChannel, sizeof(T));
    // T *dstPtrGreyscaleTemp;
    // dstPtrGreyscaleTemp = dstPtrGreyscale;
    // dstPtrGreyscaleFloatTemp = dstPtrGreyscaleFloat;
    // 
    // for (int i = 0; i < srcSize.height; i++)
    // {
    //     for (int j = 0; j < srcSize.width; j++)
    //     {
    //         if (*dstPtrGreyscaleFloatTemp == 0)
    //         {
    //             *dstPtrGreyscaleTemp = T (0);
    //         }
    //         else
    //         {
    //             *dstPtrGreyscaleTemp = T (255);
    //         }
    //         dstPtrGreyscaleTemp++;
    //         dstPtrGreyscaleFloatTemp++;
    //     }
    // }








    // Greyscale TO RGB Conversion

    // dstPtrGreyscaleTemp = dstPtrGreyscale;
    // 
    // T *dstPtrTemp;
    // dstPtrTemp = dstPtr;
    // 
    // if (channel == 3)
    // {
    //     if (chnFormat == RPPI_CHN_PLANAR)
    //     {
    //         for (int c = 0; c < channel; c++)
    //         {
    //             memcpy(dstPtrTemp, dstPtrGreyscaleTemp, imageDim * sizeof(T));
    //             dstPtrTemp += imageDim;
    //         }
    //     }
    //     else if (chnFormat == RPPI_CHN_PACKED)
    //     {
    //         for (int i = 0; i < imageDim; i++)
    //         {
    //             memcpy(dstPtrTemp, dstPtrGreyscaleTemp, sizeof(T));
    //             dstPtrTemp++;
    //             memcpy(dstPtrTemp, dstPtrGreyscaleTemp, sizeof(T));
    //             dstPtrTemp++;
    //             memcpy(dstPtrTemp, dstPtrGreyscaleTemp, sizeof(T));
    //             dstPtrTemp++;
    //             dstPtrGreyscaleTemp++;
    //         }
    //     }
    // }
    // else if (channel == 1)
    // {
    //     memcpy(dstPtr, dstPtrGreyscale, imageDim * sizeof(T));
    // }



    // Overlay Harris Corners on original image

    memcpy(dstPtr, srcPtr, channel * imageDim * sizeof(T));
    
    T *dstPtrWindow;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        dstPtrGreyscaleFloatTemp = dstPtrGreyscaleFloat + (bound * srcSize.width) + bound;
        dstPtrWindow = dstPtr;
        Rpp32u remainingElementsInRow = srcSize.width - kernelSize;
        for (int i = (2 * bound); i < srcSize.height; i++)
        {
            for (int j = (2 * bound); j < srcSize.width; j++)
            {
                if (*dstPtrGreyscaleFloatTemp != 0)
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
                dstPtrGreyscaleFloatTemp++;
                dstPtrWindow++;
            }
            dstPtrGreyscaleFloatTemp += (kernelSize - 1);
            dstPtrWindow += (kernelSize - 1);
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        dstPtrGreyscaleFloatTemp = dstPtrGreyscaleFloat + (channel * ((bound * srcSize.width) + bound));
        dstPtrWindow = dstPtr;
        Rpp32u remainingElementsInRow = channel * (srcSize.width - kernelSize);
        Rpp32u increment = channel * (kernelSize - 1);
        for (int i = (2 * bound); i < srcSize.height; i++)
        {
            for (int j = (2 * bound); j < srcSize.width; j++)
            {
                if (*dstPtrGreyscaleFloatTemp != 0)
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
                dstPtrGreyscaleFloatTemp++;
                dstPtrWindow += channel;
            }
            dstPtrGreyscaleFloatTemp += (kernelSize - 1);
            dstPtrWindow += increment;
        }
    }

    return RPP_SUCCESS;
}