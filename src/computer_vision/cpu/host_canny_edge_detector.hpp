#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus canny_edge_detector_host(T* srcPtr, RppiSize srcSize, T* dstPtr, 
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

    
    // Gaussian Filter
    
    // Rpp32u kernelSize = 5;
    // Rpp32f stdDev = 0.75;
    // Rpp32f *kernel = (Rpp32f *)calloc(kernelSize * kernelSize, sizeof(Rpp32f));
    // int bound = ((kernelSize - 1) / 2);
    // 
    // generate_gaussian_kernel_host(stdDev, kernel, kernelSize);
    // 
    // RppiSize srcSizeMod;
    // srcSizeMod.width = srcSize.width + (2 * bound);
    // srcSizeMod.height = srcSize.height + (2 * bound);
    // T *srcPtrGaussianPadded = (T *)calloc(srcSizeMod.height * srcSizeMod.width * newChannel, sizeof(T));
    // 
    // generate_evenly_padded_image_host(srcPtrGreyscale, srcSize, srcPtrGaussianPadded, srcSizeMod, chnFormat, newChannel);
    // 
    // RppiSize rppiKernelSize;
    // rppiKernelSize.height = kernelSize;
    // rppiKernelSize.width = kernelSize;
    // T *srcPtrBlurred = (T *)calloc(srcSize.height * srcSize.width * newChannel, sizeof(T));
    // convolve_image_host(srcPtrGaussianPadded, srcSizeMod, srcPtrBlurred, srcSize, kernel, rppiKernelSize, chnFormat, newChannel);
    
    
    
    RppiSize srcSizeMod, rppiKernelSize;
    Rpp32u kernelSize;
    int bound;

    // Sobel Filter
    
    kernelSize = 3;
    bound = (kernelSize - 1) / 2;

    srcSizeMod.width = srcSize.width + (2 * bound);
    srcSizeMod.height = srcSize.height + (2 * bound);

    T *dstPtrGreyscale = (T *)calloc(srcSize.height * srcSize.width * newChannel, sizeof(T));

    T *srcPtrMod = (T *)calloc(srcSizeMod.height * srcSizeMod.width * newChannel, sizeof(T));
    generate_evenly_padded_image_host(srcPtrGreyscale, srcSize, srcPtrMod, srcSizeMod, chnFormat, newChannel);

    rppiKernelSize.height = kernelSize;
    rppiKernelSize.width = kernelSize;

    Rpp32f *kernelX = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));
    generate_sobel_kernel_host(kernelX, 1);
    Rpp32s *dstPtrIntermediateX = (Rpp32s *)calloc(srcSize.height * srcSize.width * newChannel, sizeof(Rpp32s));
    convolve_image_host(srcPtrMod, srcSizeMod, dstPtrIntermediateX, srcSize, kernelX, rppiKernelSize, chnFormat, newChannel);

    Rpp32f *kernelY = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));
    generate_sobel_kernel_host(kernelY, 2);
    Rpp32s *dstPtrIntermediateY = (Rpp32s *)calloc(srcSize.height * srcSize.width * newChannel, sizeof(Rpp32s));
    convolve_image_host(srcPtrMod, srcSizeMod, dstPtrIntermediateY, srcSize, kernelY, rppiKernelSize, chnFormat, newChannel);

    compute_magnitude_host(dstPtrIntermediateX, dstPtrIntermediateY, srcSize, dstPtrGreyscale, chnFormat, newChannel);





    // Find Image Maximum

    T *srcPtrTemp;
    srcPtrTemp = srcPtrGreyscale;
    Rpp8u max = *srcPtrTemp;
    for (int i = 0; i < (newChannel * srcSize.height * srcSize.width); i++)
    {
        if (*srcPtrTemp > max)
        {
            max = *srcPtrTemp;
        }
        srcPtrTemp++;
    }





    // Determine Gradients, Perform NMS, Double Thresholding and Edge Tracing by hysterisis

    Rpp32f gradient;
    Rpp32s *dstPtrIntermediateXTemp, *dstPtrIntermediateYTemp;
    dstPtrIntermediateXTemp = dstPtrIntermediateX;
    dstPtrIntermediateYTemp = dstPtrIntermediateY;

    T *srcPtrWindow, *dstPtrGreyscaleTemp, *srcPtrWindowCenter;
    srcPtrWindow = srcPtrMod;
    dstPtrGreyscaleTemp = dstPtrGreyscale;

    generate_evenly_padded_image_host(dstPtrGreyscale, srcSize, srcPtrMod, srcSizeMod, chnFormat, newChannel);
    
    Rpp32f minThresholdRatio=0.05, maxThresholdRatio=0.09;
    T maxThreshold = (T) ((Rpp32f) max * maxThresholdRatio);
    T minThreshold = (T) ((Rpp32f) maxThreshold * minThresholdRatio);
    
    srcPtrWindowCenter = srcPtrWindow + (bound * srcSizeMod.width) + bound;
    Rpp32u toNeighborhood1 = 1;
    Rpp32u toNeighborhood2 = 2;
    Rpp32u toNeighborhood3 = srcSizeMod.width + 2;
    Rpp32u toNeighborhood4 = 2 * (srcSizeMod.width + 1);
    Rpp32u toNeighborhood5 = (2 * srcSizeMod.width) + 1;
    Rpp32u toNeighborhood6 = 2 * srcSizeMod.width;
    Rpp32u toNeighborhood7 = srcSizeMod.width;
    T *position1Ptr, *position2Ptr;
    dstPtrIntermediateXTemp = dstPtrIntermediateX;
    dstPtrIntermediateYTemp = dstPtrIntermediateY;
    
    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            gradient = atan(RPPABS((Rpp32f) *dstPtrIntermediateYTemp) / RPPABS((Rpp32f) *dstPtrIntermediateXTemp));
            //printf("\ngradient = %0.4f", gradient);
            if (RPPABS(gradient) > 1.178097)
            {
                //position1Ptr = srcPtrWindow + toNeighborhood3;
                //position2Ptr = srcPtrWindow + toNeighborhood7;
                position1Ptr = srcPtrWindow + toNeighborhood1;
                position2Ptr = srcPtrWindow + toNeighborhood5;
            }
            else if (gradient > 0.392699)
            {
                //position1Ptr = srcPtrWindow;
                //position2Ptr = srcPtrWindow + toNeighborhood4;
                position1Ptr = srcPtrWindow + toNeighborhood2;
                position2Ptr = srcPtrWindow + toNeighborhood6;
            }
            else if (gradient < -0.392699)
            {
                //position1Ptr = srcPtrWindow + toNeighborhood2;
                //position2Ptr = srcPtrWindow + toNeighborhood6;
                position1Ptr = srcPtrWindow;
                position2Ptr = srcPtrWindow + toNeighborhood4;
            }
            else
            {
                //position1Ptr = srcPtrWindow + toNeighborhood1;
                //position2Ptr = srcPtrWindow + toNeighborhood5;
                position1Ptr = srcPtrWindow + toNeighborhood3;
                position2Ptr = srcPtrWindow + toNeighborhood7;
            }

            canny_non_max_suppression_kernel_host(dstPtrGreyscaleTemp, *srcPtrWindowCenter, position1Ptr, position2Ptr);
            
            if (*dstPtrGreyscaleTemp > maxThreshold)
            {
                *dstPtrGreyscaleTemp = (T) 255;
            }
            else if (*dstPtrGreyscaleTemp < minThreshold)
            {
                *dstPtrGreyscaleTemp = (T) 0;
            }
            else
            {
                *dstPtrGreyscaleTemp = (T) 100;
            }

            srcPtrWindow++;
            srcPtrWindowCenter++;
            dstPtrGreyscaleTemp++;
            dstPtrIntermediateXTemp++;
            dstPtrIntermediateYTemp++;
        }
        srcPtrWindow += (kernelSize - 1);
        srcPtrWindowCenter += (kernelSize - 1);
    }

    srcPtrWindow = srcPtrMod;
    dstPtrGreyscaleTemp = dstPtrGreyscale;
    generate_evenly_padded_image_host(dstPtrGreyscale, srcSize, srcPtrMod, srcSizeMod, chnFormat, newChannel);

    srcPtrWindowCenter = srcPtrWindow + (bound * srcSizeMod.width) + bound;
    Rpp32u remainingElementsInRow = srcSizeMod.width - kernelSize;
    
    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            if (*srcPtrWindowCenter == (T) 100)
            {
                canny_hysterisis_edge_tracing_kernel_host(srcPtrWindow, dstPtrGreyscaleTemp, srcSize, 
                                kernelSize, remainingElementsInRow, *srcPtrWindowCenter, bound, 
                                chnFormat, newChannel);
            }
            srcPtrWindow++;
            srcPtrWindowCenter++;
            dstPtrGreyscaleTemp++;
        }
        srcPtrWindow += (kernelSize - 1);
        srcPtrWindowCenter += (kernelSize - 1);
    }

/*
    Rpp32f gradient, slope;
    Rpp32s *dstPtrIntermediateXTemp, *dstPtrIntermediateYTemp;
    dstPtrIntermediateXTemp = dstPtrIntermediateX;
    dstPtrIntermediateYTemp = dstPtrIntermediateY;

    T *srcPtrWindow, *dstPtrGreyscaleTemp, *srcPtrWindowCenter;
    srcPtrWindow = srcPtrMod;
    dstPtrGreyscaleTemp = dstPtrGreyscale;

    generate_evenly_padded_image_host(dstPtrGreyscale, srcSize, srcPtrMod, srcSizeMod, chnFormat, newChannel);
    
    Rpp32f minThresholdRatio=0.05, maxThresholdRatio=0.2;
    T maxThreshold = (T) ((Rpp32f) max * maxThresholdRatio);
    T minThreshold = (T) ((Rpp32f) maxThreshold * minThresholdRatio);
    
    srcPtrWindowCenter = srcPtrWindow + (bound * srcSizeMod.width) + bound;
    Rpp32u toNeighborhood1 = 1;
    Rpp32u toNeighborhood2 = 2;
    Rpp32u toNeighborhood3 = srcSizeMod.width + 2;
    Rpp32u toNeighborhood4 = 2 * (srcSizeMod.width + 1);
    Rpp32u toNeighborhood5 = (2 * srcSizeMod.width) + 1;
    Rpp32u toNeighborhood6 = 2 * srcSizeMod.width;
    Rpp32u toNeighborhood7 = srcSizeMod.width;
    T *position1Ptr, *position2Ptr;
    dstPtrIntermediateXTemp = dstPtrIntermediateX;
    dstPtrIntermediateYTemp = dstPtrIntermediateY;
    
    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            slope = (Rpp32f) *dstPtrIntermediateYTemp / (Rpp32f) *dstPtrIntermediateXTemp;
            gradient = atan(slope);
            if (RPPABS(gradient) > 1.178097)
            {
                position1Ptr = srcPtrWindow + toNeighborhood1;
                position2Ptr = srcPtrWindow + toNeighborhood5;
            }
            else if (gradient > 0.392699)
            {
                position1Ptr = srcPtrWindow + toNeighborhood2;
                position2Ptr = srcPtrWindow + toNeighborhood6;
            }
            else if (gradient < -0.392699)
            {
                position1Ptr = srcPtrWindow;
                position2Ptr = srcPtrWindow + toNeighborhood4;
            }
            else
            {
                position1Ptr = srcPtrWindow + toNeighborhood3;
                position2Ptr = srcPtrWindow + toNeighborhood7;
            }

            canny_non_max_suppression_kernel_host(dstPtrGreyscaleTemp, *srcPtrWindowCenter, position1Ptr, position2Ptr);
            
            if (*dstPtrGreyscaleTemp > maxThreshold)
            {
                *dstPtrGreyscaleTemp = (T) 255;
            }
            else if (*dstPtrGreyscaleTemp < minThreshold)
            {
                *dstPtrGreyscaleTemp = (T) 0;
            }
            else
            {
                *dstPtrGreyscaleTemp = (T) 100;
            }

            srcPtrWindow++;
            srcPtrWindowCenter++;
            dstPtrGreyscaleTemp++;
            dstPtrIntermediateXTemp++;
            dstPtrIntermediateYTemp++;
        }
        srcPtrWindow += (kernelSize - 1);
        srcPtrWindowCenter += (kernelSize - 1);
    }

    srcPtrWindow = srcPtrMod;
    dstPtrGreyscaleTemp = dstPtrGreyscale;
    generate_evenly_padded_image_host(dstPtrGreyscale, srcSize, srcPtrMod, srcSizeMod, chnFormat, newChannel);

    srcPtrWindowCenter = srcPtrWindow + (bound * srcSizeMod.width) + bound;
    Rpp32u remainingElementsInRow = srcSizeMod.width - kernelSize;
    
    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            if (*srcPtrWindowCenter == (T) 100)
            {
                canny_hysterisis_edge_tracing_kernel_host(srcPtrWindow, dstPtrGreyscaleTemp, srcSize, 
                                kernelSize, remainingElementsInRow, *srcPtrWindowCenter, bound, 
                                chnFormat, newChannel);
            }
            srcPtrWindow++;
            srcPtrWindowCenter++;
            dstPtrGreyscaleTemp++;
        }
        srcPtrWindow += (kernelSize - 1);
        srcPtrWindowCenter += (kernelSize - 1);
    }
*/







    

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