#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus harris_corner_detector_host(T* srcPtr, RppiSize srcSize, T* dstPtr, 
                                      Rpp32u kernelSize, Rpp32f kValue, Rpp32u threshold, 
                                      RppiChnFormat chnFormat, Rpp32u channel)
{
    // RGB to Greyscale Conversion

    Rpp32u imageDim = srcSize.height * srcSize.width;

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






    // Sobel Filter
    
    RppiSize srcSizeMod, rppiSobelKernelSize;
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







    // Compute the corner strengh matrix

    T *dstPtrGreyscale = (T *)calloc(srcSize.height * srcSize.width * newChannel, sizeof(T));
    T *dstPtrGreyscaleTemp;
    dstPtrGreyscaleTemp = dstPtrGreyscale;

    T *srcPtrWindowX, *srcPtrWindowY;
    srcPtrWindowX = srcPtrDerivativeXmod;
    srcPtrWindowY = srcPtrDerivativeYmod;
    

    Rpp32u remainingElementsInRow = srcSizeDerivativeMod.width - kernelSize;
    Rpp32f min = 65535, max = -65535;

    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            harris_corner_detector_kernel_host(srcPtrWindowX, srcPtrWindowY, dstPtrGreyscaleTemp, srcSize, 
                                                kernelSize, remainingElementsInRow, kValue, &min, &max, 
                                                chnFormat, channel);
            srcPtrWindowX++;
            srcPtrWindowY++;
            dstPtrGreyscaleTemp++;
        }
        srcPtrWindowX += (kernelSize - 1);
        srcPtrWindowY += (kernelSize - 1);
    }
    printf("\nMax = %0.4f, Min = %0.4f", max, min);

    T *dstPtrGreyscaleMod = (T *)calloc(srcSizeDerivativeMod.height * srcSizeDerivativeMod.width * newChannel, sizeof(T));
    generate_evenly_padded_image_host(dstPtrGreyscale, srcSize, dstPtrGreyscaleMod, srcSizeDerivativeMod, chnFormat, newChannel);
    
    T *dstPtrGreyscaleWindow;
    T windowCenter;
    dstPtrGreyscaleWindow = dstPtrGreyscaleMod;
    dstPtrGreyscaleTemp = dstPtrGreyscale;
    
    Rpp32u windowCenterPosIncrement = (bound * srcSizeDerivativeMod.width) + bound;

    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            windowCenter = (T) *(dstPtrGreyscaleWindow + windowCenterPosIncrement);
            non_max_suppression_kernel_host(dstPtrGreyscaleWindow, dstPtrGreyscaleTemp, srcSize, 
                                kernelSize, remainingElementsInRow, windowCenter, 
                                chnFormat, newChannel);
            dstPtrGreyscaleWindow++;
            dstPtrGreyscaleTemp++;
        }
        dstPtrGreyscaleWindow += (kernelSize - 1);
    }












    // Greyscale TO RGB Conversion

    dstPtrGreyscaleTemp = dstPtrGreyscale;
    T *dstPtrTemp;
    dstPtrTemp = dstPtr;
    
    if (channel == 3)
    {
        if (chnFormat == RPPI_CHN_PLANAR)
        {
            for (int c = 0; c < channel; c++)
            {
                memcpy(dstPtrTemp, dstPtrGreyscaleTemp, imageDim * sizeof(T));
                dstPtrTemp += imageDim;
            }
        }
        else if (chnFormat == RPPI_CHN_PACKED)
        {
            for (int i = 0; i < imageDim; i++)
            {
                memcpy(dstPtrTemp, dstPtrGreyscaleTemp, sizeof(T));
                dstPtrTemp++;
                memcpy(dstPtrTemp, dstPtrGreyscaleTemp, sizeof(T));
                dstPtrTemp++;
                memcpy(dstPtrTemp, dstPtrGreyscaleTemp, sizeof(T));
                dstPtrTemp++;
                dstPtrGreyscaleTemp++;
            }
        }
    }
    else if (channel == 1)
    {
        memcpy(dstPtr, dstPtrGreyscale, imageDim * sizeof(T));
    }

    return RPP_SUCCESS;
}