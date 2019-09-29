#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus hog_host(T* srcPtr, RppiSize srcSize, U* binsTensor, Rpp32u binsTensorLength, 
                   RppiSize kernelSize, RppiSize windowSize,  Rpp32u windowStride, Rpp32u numOfBins, 
                   RppiChnFormat chnFormat, Rpp32u channel)
{
    Rpp32u imageDim = srcSize.height * srcSize.width;
    Rpp32u newChannel = 1;

    Rpp32f gradientKernel[3] = {-1, 0, 1};
    RppiSize rppiGradientKernelSizeX, rppiGradientKernelSizeY;
    rppiGradientKernelSizeX.height = 1;
    rppiGradientKernelSizeX.width = 3;
    rppiGradientKernelSizeY.height = 3;
    rppiGradientKernelSizeY.width = 1;

    Rpp32s *gradientX = (Rpp32s *)calloc(imageDim * newChannel, sizeof(Rpp32s));
    Rpp32s *gradientY = (Rpp32s *)calloc(imageDim * newChannel, sizeof(Rpp32s));
    Rpp32s *gradientMagnitude = (Rpp32s *)calloc(imageDim * newChannel, sizeof(Rpp32s));
    Rpp32f *gradientDirection = (Rpp32f *)calloc(imageDim * newChannel, sizeof(Rpp32f));
    
    if (channel == 1)
    {
        hog_single_channel_gradient_computations_kernel_host(srcPtr, srcSize, gradientX, gradientY, gradientMagnitude, gradientDirection, 
                                                             gradientKernel, rppiGradientKernelSizeX, rppiGradientKernelSizeY);
    }
    else if (channel == 3)
    {
        Rpp32s *gradientX0 = (Rpp32s *)calloc(imageDim * newChannel, sizeof(Rpp32s));
        Rpp32s *gradientX1 = (Rpp32s *)calloc(imageDim * newChannel, sizeof(Rpp32s));
        Rpp32s *gradientX2 = (Rpp32s *)calloc(imageDim * newChannel, sizeof(Rpp32s));
        Rpp32s *gradientY0 = (Rpp32s *)calloc(imageDim * newChannel, sizeof(Rpp32s));
        Rpp32s *gradientY1 = (Rpp32s *)calloc(imageDim * newChannel, sizeof(Rpp32s));
        Rpp32s *gradientY2 = (Rpp32s *)calloc(imageDim * newChannel, sizeof(Rpp32s));

        T *srcPtrSingleChannel = (T *)calloc(imageDim * newChannel, sizeof(T));
        
        hog_three_channel_gradient_computations_kernel_host(srcPtr, srcPtrSingleChannel, srcSize, 
                                                             gradientX0, gradientY0, gradientX1, gradientY1, gradientX2, gradientY2, 
                                                             gradientX, gradientY, 
                                                             gradientMagnitude, gradientDirection, 
                                                             gradientKernel, rppiGradientKernelSizeX, rppiGradientKernelSizeY, chnFormat, channel);
    }

    Rpp32s *gradientMagnitudeTemp, *gradientMagnitudeTemp2, *gradientMagnitudeTemp3;
    Rpp32f *gradientDirectionTemp, *gradientDirectionTemp2, *gradientDirectionTemp3;

    Rpp32u binsTensorTrueLength = 0;
    Rpp32u windowKernelHeightRatio = windowSize.height / kernelSize.height;
    Rpp32u windowKernelWidthRatio = windowSize.width / kernelSize.width;
    binsTensorTrueLength = ((windowKernelWidthRatio * windowKernelHeightRatio) + ((windowKernelWidthRatio - 1) * (windowKernelHeightRatio - 1)));
    Rpp32u numOfPositionsAlongImageWidth = (srcSize.width / windowStride - (windowSize.width / windowStride - 1));
    Rpp32u numOfPositionsAlongImageHeight = (srcSize.height / windowStride - (windowSize.height / windowStride - 1));
    binsTensorTrueLength = binsTensorTrueLength * (numOfPositionsAlongImageWidth * numOfPositionsAlongImageHeight);
    binsTensorTrueLength = binsTensorTrueLength * numOfBins;
    //printf("\nbinsTensorTrueLength = %d", binsTensorTrueLength);
    //printf("\nnumOfPositionsAlongImageWidth = %d", numOfPositionsAlongImageWidth);
    //printf("\nnumOfPositionsAlongImageHeight = %d", numOfPositionsAlongImageHeight);

    Rpp32u numOfPositionsAlongWindowWidth = (windowSize.width / kernelSize.width);
    Rpp32u numOfPositionsAlongWindowHeight = (windowSize.height / kernelSize.height);
    //printf("\nnumOfPositionsAlongWindowWidth = %d", numOfPositionsAlongWindowWidth);
    //printf("\nnumOfPositionsAlongWindowHeight = %d", numOfPositionsAlongWindowHeight);

    U *binsTensorTemp;
    binsTensorTemp = binsTensor;
    Rpp32f rangeInBin = PI / numOfBins;
    //printf("\nrangeInBin = %f", rangeInBin);
    Rpp32u elementCount = 0, bin;
    Rpp32f adder = PI / 2;

    // For sliding window on image

    for(int i = 0; i < numOfPositionsAlongImageHeight; i++)
    {
        gradientMagnitudeTemp = gradientMagnitude + (i * windowStride * srcSize.width);
        gradientDirectionTemp = gradientDirection + (i * windowStride * srcSize.width);
        for(int j = 0; j < numOfPositionsAlongImageWidth; j++)
        {
            // For each window

            // Layer 1
            for (int m = 0; m < numOfPositionsAlongWindowHeight; m++)
            {
                gradientMagnitudeTemp2 = gradientMagnitudeTemp + (m * kernelSize.height * srcSize.width);
                gradientDirectionTemp2 = gradientDirectionTemp + (m * kernelSize.height * srcSize.width);
                for (int n = 0; n < numOfPositionsAlongWindowWidth; n++)
                {
                    U *kernelHistogram = (U*) calloc(numOfBins, sizeof(U));
                    
                    // For each kernel
                    for (int p = 0; p < kernelSize.height; p++)
                    {
                        gradientMagnitudeTemp3 = gradientMagnitudeTemp2 + (p * srcSize.width);
                        gradientDirectionTemp3 = gradientDirectionTemp2 + (p * srcSize.width);
                        for (int q = 0; q < kernelSize.width; q++)
                        {
                            bin = (Rpp32u) ((*gradientDirectionTemp3 + adder) / rangeInBin);
                            if (bin > (numOfBins - 1))
                            {
                                bin = numOfBins - 1;
                            }
                            *(kernelHistogram + bin) += *gradientMagnitudeTemp3;
                            gradientMagnitudeTemp3++;
                            gradientDirectionTemp3++;
                        }
                    }
                    U *kernelHistogramTemp;
                    kernelHistogramTemp = kernelHistogram;
                    for (int r = 0; r < numOfBins; r++)
                    {
                        if (elementCount < (binsTensorLength - 1))
                        {
                            *binsTensorTemp = *kernelHistogramTemp;
                            binsTensorTemp++;
                            kernelHistogramTemp++;
                            elementCount++;
                        }
                        else
                        {
                            return RPP_SUCCESS;
                        }
                        
                    }
                    gradientMagnitudeTemp2 += kernelSize.width;
                    gradientDirectionTemp2 += kernelSize.width;
                }
            }

            // Layer 2
            for (int m = 0; m < numOfPositionsAlongWindowHeight - 1; m++)
            {
                gradientMagnitudeTemp2 = gradientMagnitudeTemp + (kernelSize.height / 2 * srcSize.width) + (m * kernelSize.height * srcSize.width) + (kernelSize.width / 2);
                gradientDirectionTemp2 = gradientDirectionTemp + (kernelSize.height / 2 * srcSize.width)+ (m * kernelSize.height * srcSize.width) + (kernelSize.width / 2);
                for (int n = 0; n < numOfPositionsAlongWindowWidth - 1; n++)
                {
                    U *kernelHistogram = (U*) calloc(numOfBins, sizeof(U));
                    
                    // For each kernel
                    
                    for (int p = 0; p < kernelSize.height; p++)
                    {
                        gradientMagnitudeTemp3 = gradientMagnitudeTemp2 + (p * srcSize.width);
                        gradientDirectionTemp3 = gradientDirectionTemp2 + (p * srcSize.width);
                        for (int q = 0; q < kernelSize.width; q++)
                        {
                            bin = (Rpp32u) ((*gradientDirectionTemp3 + adder) / rangeInBin);
                            if (bin > (numOfBins - 1))
                            {
                                bin = numOfBins - 1;
                            }
                            *(kernelHistogram + bin) += *gradientMagnitudeTemp3;
                            gradientMagnitudeTemp3++;
                            gradientDirectionTemp3++;
                        }
                    }
                    U *kernelHistogramTemp;
                    kernelHistogramTemp = kernelHistogram;
                    for (int r = 0; r < numOfBins; r++)
                    {
                        if (elementCount < (binsTensorLength - 1))
                        {
                            *binsTensorTemp = *kernelHistogramTemp;
                            binsTensorTemp++;
                            kernelHistogramTemp++;
                            elementCount++;
                        }
                        else
                        {
                            return RPP_SUCCESS;
                        }
                        
                    }

                    gradientMagnitudeTemp2 += kernelSize.width;
                    gradientDirectionTemp2 += kernelSize.width;
                }
            }

            gradientMagnitudeTemp += windowStride;
            gradientDirectionTemp += windowStride;
        }
    }

    return RPP_SUCCESS;
}