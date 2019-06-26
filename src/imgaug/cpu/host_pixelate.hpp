#include <cpu/rpp_cpu_common.hpp>

RppStatus generate_gaussian_kernel_host(Rpp32f stdDev, Rpp32f* kernel, unsigned int kernelSize)
{
    Rpp32f s, sum = 0.0, multiplier;
    int bound = ((kernelSize - 1) / 2);
    unsigned int c = 0;
    s = 1 / (2 * stdDev * stdDev);
    multiplier = (1 / M_PI) * (s);
    for (int i = -bound; i <= bound; i++)
    {
        for (int j = -bound; j <= bound; j++)
        {
            kernel[c] = multiplier * exp((-1) * (s) * (i*i + j*j));
            sum += kernel[c];
            c += 1;
        }
    }
    for (int i = 0; i < (kernelSize * kernelSize); i++)
    {
        kernel[i] /= sum;
    }

    return RPP_SUCCESS;
}

template <typename T>
RppStatus generate_evenly_padded_image_host(T* srcPtr, RppiSize srcSize, T* srcPtrMod, RppiSize srcSizeMod, 
                                     RppiChnFormat chnFormat, unsigned int channel)
{
    if (RPPISEVEN(srcSize.height) != RPPISEVEN(srcSizeMod.height) 
        || RPPISEVEN(srcSize.width) != RPPISEVEN(srcSizeMod.width)
        || srcSizeMod.height < srcSize.height
        || srcSizeMod.width < srcSize.width)
    {
        printf("\nCannot generate evenly padded image!\n");

        return RPP_ERROR;
    }
    T *srcPtrTemp, *srcPtrModTemp;
    srcPtrTemp = srcPtr;
    srcPtrModTemp = srcPtrMod;
    int bound = (srcSizeMod.height - srcSize.height) / 2;
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int b = 0; b < bound; b++)
            {
                for (int i = 0; i < srcSizeMod.width; i++)
                {
                    *srcPtrModTemp = 0;
                    srcPtrModTemp++;
                }
            }
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int b = 0; b < bound; b++)
                {
                    *srcPtrModTemp = 0;
                    srcPtrModTemp++;
                }
                for (int j = 0; j < srcSize.width; j++)
                {
                    *srcPtrModTemp = *srcPtrTemp;
                    srcPtrModTemp++;
                    srcPtrTemp++;
                }
                for (int b = 0; b < bound; b++)
                {
                    *srcPtrModTemp = 0;
                    srcPtrModTemp++;
                }
            }
            for (int b = 0; b < bound; b++)
            {
                for (int i = 0; i < srcSizeMod.width; i++)
                {
                    *srcPtrModTemp = 0;
                    srcPtrModTemp++;
                }
            }
        }
    }
    else if(chnFormat == RPPI_CHN_PACKED)
    {
        for (int b = 0; b < bound; b++)
        {
            for (int i = 0; i < srcSizeMod.width; i++)
            {
                for (int c = 0; c < channel; c++)
                {
                    *srcPtrModTemp = 0;
                    srcPtrModTemp++;
                }

            }
        }

        for (int i = 0; i < srcSize.height; i++)
        {
            for (int b = 0; b < bound; b++)
            {
                for (int c = 0; c < channel; c++)
                {
                    *srcPtrModTemp = 0;
                    srcPtrModTemp++;
                }
            }
            for (int j = 0; j < srcSize.width; j++)
            {
                for (int c = 0; c < channel; c++)
                {
                    *srcPtrModTemp = *srcPtrTemp;
                    srcPtrModTemp++;
                    srcPtrTemp++;
                }
            }
            for (int b = 0; b < bound; b++)
            {
                for (int c = 0; c < channel; c++)
                {
                    *srcPtrModTemp = 0;
                    srcPtrModTemp++;
                }
            }
        }

        for (int b = 0; b < bound; b++)
        {
            for (int i = 0; i < srcSizeMod.width; i++)
            {
                for (int c = 0; c < channel; c++)
                {
                    *srcPtrModTemp = 0;
                    srcPtrModTemp++;
                }
            }
        }
    }

    return RPP_SUCCESS;
}

template<typename T>
RppStatus convolve_host(T* srcPtrMod, RppiSize srcSizeMod, T* dstPtr, RppiSize srcSize, 
                        Rpp32f* kernel, unsigned int kernelSize, 
                        RppiChnFormat chnFormat, unsigned int channel)
{
    T *dstPtrTemp;
    dstPtrTemp = dstPtr;
    int srcModLoc = 0;
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        int count = 0;
        float pixel = 0.0;
        int *convLocs = (int *)malloc(kernelSize * kernelSize * sizeof(int));
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    count = 0;
                    pixel = 0.0;
                    for (int m = 0; m < kernelSize; m++)
                    {
                        int convLocsRow = srcModLoc + (m * srcSizeMod.width);
                        for (int n = 0; n < kernelSize; n++, count++)
                        {
                            convLocs[count] = convLocsRow + n;
                        }
                    }
                    for (int k = 0; k < (kernelSize * kernelSize); k++)
                    {
                        pixel += (kernel[k] * (float)srcPtrMod[convLocs[k]]);
                    }
                    pixel = (pixel < (Rpp32f) 255) ? pixel : ((Rpp32f) 255);
                    pixel = (pixel > (Rpp32f) 0) ? pixel : ((Rpp32f) 0);
                    *dstPtrTemp = (Rpp8u) round(pixel);
                    dstPtrTemp++;
                    srcModLoc += 1;
                }
                srcModLoc += (kernelSize - 1);
            }
            srcModLoc += ((kernelSize - 1) * srcSizeMod.width);
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        int count = 0;
        float pixel = 0.0;
        int *convLocs = (int *)malloc(kernelSize * kernelSize * sizeof(int));
        int elementsInRow = srcSizeMod.width * channel;
        for (int i = 0; i < srcSize.height; i++)
        {
            for (int j = 0; j < srcSize.width; j++)
            {
                for (int c = 0; c < channel; c++)
                {   count = 0;
                    pixel = 0.0;
                    for (int m = 0; m < kernelSize; m++)
                    {
                        int convLocsRow = srcModLoc + (m * elementsInRow);
                        for (int n = 0; n < kernelSize; n++, count++)
                        {
                            convLocs[count] = convLocsRow + (n * channel);
                        }
                    }
                    for (int k = 0; k < (kernelSize * kernelSize); k++)
                    {
                        pixel += (kernel[k] * (float)srcPtrMod[convLocs[k]]);
                    }
                    pixel = (pixel < (Rpp32f) 255) ? pixel : ((Rpp32f) 255);
                    pixel = (pixel > (Rpp32f) 0) ? pixel : ((Rpp32f) 0);
                    *dstPtrTemp = (Rpp8u) round(pixel);
                    dstPtrTemp++;
                    srcModLoc++;
                }
            }
            srcModLoc += ((kernelSize - 1) * channel);
        }
    }

    return RPP_SUCCESS;
}

template <typename T>
RppStatus blur_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    Rpp32f stdDev, unsigned int kernelSize,
                    RppiChnFormat chnFormat, unsigned int channel)
{
    if (kernelSize % 2 == 0)
    {
        return RPP_ERROR;
    }
    Rpp32f *kernel = (Rpp32f *)malloc(kernelSize * kernelSize * sizeof(Rpp32f));
    int bound = ((kernelSize - 1) / 2);

    generate_gaussian_kernel_host(stdDev, kernel, kernelSize);

    RppiSize srcSizeMod;
    srcSizeMod.width = srcSize.width + (2 * bound);
    srcSizeMod.height = srcSize.height + (2 * bound);
    Rpp8u *srcPtrMod = (Rpp8u *)malloc(srcSizeMod.width * srcSizeMod.height * channel * sizeof(Rpp8u));

    generate_evenly_padded_image_host(srcPtr, srcSize, srcPtrMod, srcSizeMod, chnFormat, channel);
    
    convolve_host(srcPtrMod, srcSizeMod, dstPtr, srcSize, kernel, kernelSize, chnFormat, channel);
    
    return RPP_SUCCESS;
}
