#ifndef RPP_CPU_COMMON_H
#define RPP_CPU_COMMON_H

#include <math.h>
#include <algorithm>

#include <rppdefs.h>

#define PI 3.14159265
#define RAD(deg)                (deg * PI / 180)
#define RPPABS(a)               ((a < 0) ? (-a) : (a))
#define RPPMIN2(a,b)            ((a < b) ? a : b)
#define RPPMIN3(a,b,c)          ((a < b) && (a < c) ?  a : ((b < c) ? b : c))
#define RPPMAX2(a,b)            ((a > b) ? a : b)
#define RPPMAX3(a,b,c)          ((a > b) && (a > c) ?  a : ((b > c) ? b : c))
#define RPPINRANGE(a, x, y)     ((a >= x) && (a <= y) ? 1 : 0)
#define RPPFLOOR(a)             ((int) a)
#define RPPCEIL(a)              ((int) (a + 1.0))
#define RPPISEVEN(a)            ((a % 2 == 0) ? 1 : 0)
#define RPPPIXELCHECK(pixel)    (pixel < (Rpp32f) 0) ? ((Rpp32f) 0) : ((pixel < (Rpp32f) 255) ? pixel : ((Rpp32f) 255))
#define RPP16UPIXELCHECK(pixel)     (pixel < (Rpp32f) 0) ? ((Rpp32f) 0) : ((pixel < (Rpp32f) 65535) ? pixel : ((Rpp32f) 65535))
#define RPPISGREATER(pixel, value)  ((pixel > value) ? 1 : 0)
#define RPPISLESSER(pixel, value)  ((pixel < value) ? 1 : 0)




// Generate Functions

RppStatus generate_gaussian_kernel_host(Rpp32f stdDev, Rpp32f* kernel, Rpp32u kernelSize)
{
    Rpp32f s, sum = 0.0, multiplier;
    int bound = ((kernelSize - 1) / 2);
    Rpp32u c = 0;
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

RppStatus generate_gaussian_kernel_asymmetric_host(Rpp32f stdDev, Rpp32f* kernel, Rpp32u kernelSizeX, Rpp32u kernelSizeY)
{
    Rpp32f s, sum = 0.0, multiplier;
    if (kernelSizeX % 2 == 0)
    {
        return RPP_ERROR;
    }
    if (kernelSizeY % 2 == 0)
    {
        return RPP_ERROR;
    }
    int boundX = ((kernelSizeX - 1) / 2);
    int boundY = ((kernelSizeY - 1) / 2);
    Rpp32u c = 0;
    s = 1 / (2 * stdDev * stdDev);
    multiplier = (1 / M_PI) * (s);
    for (int i = -boundY; i <= boundY; i++)
    {
        for (int j = -boundX; j <= boundX; j++)
        {
            kernel[c] = multiplier * exp((-1) * (s) * (i*i + j*j));
            sum += kernel[c];
            c += 1;
        }
    }
    for (int i = 0; i < (kernelSizeX * kernelSizeY); i++)
    {
        kernel[i] /= sum;
    }

    return RPP_SUCCESS;
}

template <typename T>
RppStatus generate_bilateral_kernel_host(Rpp32f multiplierI, Rpp32f multiplierS, Rpp32f multiplier, Rpp32f* kernel, Rpp32u kernelSize, int bound, 
                                         T* srcPtrWindow, RppiSize srcSizeMod, Rpp32u remainingElementsInRow, Rpp32u incrementToWindowCenter, 
                                         RppiChnFormat chnFormat, Rpp32u channel)
{
    Rpp32f sum = 0.0;
    Rpp32f* kernelTemp;
    kernelTemp = kernel;
    
    T *srcPtrWindowTemp, *srcPtrWindowCenter;
    srcPtrWindowTemp = srcPtrWindow;
    srcPtrWindowCenter = srcPtrWindow + incrementToWindowCenter;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int i = -bound; i <= bound; i++)
        {
            for (int j = -bound; j <= bound; j++)
            {
                T pixel = *srcPtrWindowCenter - *srcPtrWindowTemp;
                pixel = RPPABS(pixel);
                pixel = pixel * pixel;
                *kernelTemp = multiplier * exp((multiplierS * (i*i + j*j)) + (multiplierI * pixel));
                sum = sum + *kernelTemp;
                kernelTemp++;
                srcPtrWindowTemp++;
            }
            srcPtrWindowTemp += remainingElementsInRow;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int i = -bound; i <= bound; i++)
        {
            for (int j = -bound; j <= bound; j++)
            {
                T pixel = *srcPtrWindowCenter - *srcPtrWindowTemp;
                pixel = RPPABS(pixel);
                pixel = pixel * pixel;
                *kernelTemp = multiplier * exp((multiplierS * (i*i + j*j)) + (multiplierI * pixel));
                sum = sum + *kernelTemp;
                kernelTemp++;
                srcPtrWindowTemp += channel;
            }
            srcPtrWindowTemp += remainingElementsInRow;
        }
    }

    kernelTemp = kernel;
    for (int i = 0; i < (kernelSize * kernelSize); i++)
    {
        *kernelTemp = *kernelTemp / sum;
        kernelTemp++;
    }
    
    return RPP_SUCCESS;
}

template <typename T>
RppStatus generate_evenly_padded_image_host(T* srcPtr, RppiSize srcSize, T* srcPtrMod, RppiSize srcSizeMod, 
                                     RppiChnFormat chnFormat, Rpp32u channel)
{
    if (RPPISEVEN(srcSize.height) != RPPISEVEN(srcSizeMod.height) 
        || RPPISEVEN(srcSize.width) != RPPISEVEN(srcSizeMod.width)
        || srcSizeMod.height < srcSize.height
        || srcSizeMod.width < srcSize.width)
    {
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
            memset (srcPtrModTemp,(T) 0,bound * srcSizeMod.width * sizeof(T));
            srcPtrModTemp += (bound * srcSizeMod.width);
            for (int i = 0; i < srcSize.height; i++)
            {
                memset (srcPtrModTemp,(T) 0,bound * sizeof(T));
                srcPtrModTemp += bound;
                
                memcpy(srcPtrModTemp, srcPtrTemp, srcSize.width * sizeof(T));
                srcPtrModTemp += srcSize.width;
                srcPtrTemp += srcSize.width;
                
                memset (srcPtrModTemp,(T) 0,bound * sizeof(T));
                srcPtrModTemp += bound;
            }
            memset (srcPtrModTemp,(T) 0,bound * srcSizeMod.width * sizeof(T));
            srcPtrModTemp += (bound * srcSizeMod.width);
        }
    }
    else if(chnFormat == RPPI_CHN_PACKED)
    {
        Rpp32u elementsInRow = channel * srcSize.width;
        Rpp32u numOfPixelsVtBorder = bound * channel;
        Rpp32u numOfPixelsHrBorder = numOfPixelsVtBorder * srcSizeMod.width;

        memset (srcPtrModTemp,(T) 0,numOfPixelsHrBorder * sizeof(T));
        srcPtrModTemp += (numOfPixelsHrBorder);

        for (int i = 0; i < srcSize.height; i++)
        {
            memset (srcPtrModTemp,(T) 0,numOfPixelsVtBorder * sizeof(T));
            srcPtrModTemp += (numOfPixelsVtBorder);

            memcpy(srcPtrModTemp, srcPtrTemp, elementsInRow * sizeof(T));
            srcPtrModTemp += elementsInRow;
            srcPtrTemp += elementsInRow;
            
            memset (srcPtrModTemp,(T) 0,numOfPixelsVtBorder * sizeof(T));
            srcPtrModTemp += (numOfPixelsVtBorder);
        }

        memset (srcPtrModTemp,(T) 0,numOfPixelsHrBorder * sizeof(T));
        srcPtrModTemp += (numOfPixelsHrBorder);
    }

    return RPP_SUCCESS;
}

template <typename T>
RppStatus generate_corner_padded_image_host(T* srcPtr, RppiSize srcSize, T* srcPtrMod, RppiSize srcSizeMod, Rpp32u padType, 
                                     RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTemp, *srcPtrModTemp;
    srcPtrTemp = srcPtr;
    srcPtrModTemp = srcPtrMod;
    Rpp32u boundY = srcSizeMod.height - srcSize.height;
    Rpp32u boundX = srcSizeMod.width - srcSize.width;
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            if (padType == 1 || padType == 2)
            {
                memset (srcPtrModTemp,(T) 0,boundY * srcSizeMod.width * sizeof(T));
                srcPtrModTemp += (boundY * srcSizeMod.width);
            }
            
            if (padType == 1 || padType == 3)
            {
                for (int i = 0; i < srcSize.height; i++)
                {
                    memset (srcPtrModTemp,(T) 0,boundX * sizeof(T));
                    srcPtrModTemp += boundX;

                    memcpy(srcPtrModTemp, srcPtrTemp, srcSize.width * sizeof(T));
                    srcPtrModTemp += srcSize.width;
                    srcPtrTemp += srcSize.width;
                }
            }
               
            if (padType == 2 || padType == 4)
            {
                for (int i = 0; i < srcSize.height; i++)
                {
                    memcpy(srcPtrModTemp, srcPtrTemp, srcSize.width * sizeof(T));
                    srcPtrModTemp += srcSize.width;
                    srcPtrTemp += srcSize.width;
                    
                    memset (srcPtrModTemp,(T) 0,boundX * sizeof(T));
                    srcPtrModTemp += boundX;
                }
            }
            
            if (padType == 3 || padType == 4)
            {
                memset (srcPtrModTemp,(T) 0,boundY * srcSizeMod.width * sizeof(T));
                srcPtrModTemp += (boundY * srcSizeMod.width);
            }
        }
    }
    else if(chnFormat == RPPI_CHN_PACKED)
    {
        Rpp32u elementsInRow = channel * srcSize.width;
        Rpp32u numOfPixelsVtBorder = boundX * channel;
        Rpp32u numOfPixelsHrBorder = boundY * channel * srcSizeMod.width;

        if (padType == 1 || padType == 2)
        {
            memset (srcPtrModTemp,(T) 0,numOfPixelsHrBorder * sizeof(T));
            srcPtrModTemp += (numOfPixelsHrBorder);
        }

        if (padType == 1 || padType == 3)
        {
            for (int i = 0; i < srcSize.height; i++)
            {
                memset (srcPtrModTemp,(T) 0,numOfPixelsVtBorder * sizeof(T));
                srcPtrModTemp += (numOfPixelsVtBorder);

                memcpy(srcPtrModTemp, srcPtrTemp, elementsInRow * sizeof(T));
                srcPtrModTemp += elementsInRow;
                srcPtrTemp += elementsInRow;
            }
        }
        
        if (padType == 2 || padType == 4)
        {
            for (int i = 0; i < srcSize.height; i++)
            {
                memcpy(srcPtrModTemp, srcPtrTemp, elementsInRow * sizeof(T));
                srcPtrModTemp += elementsInRow;
                srcPtrTemp += elementsInRow;

                memset (srcPtrModTemp,(T) 0,numOfPixelsVtBorder * sizeof(T));
                srcPtrModTemp += (numOfPixelsVtBorder);
            }
        }

        if (padType == 3 || padType == 4)
        {
            memset (srcPtrModTemp,(T) 0,numOfPixelsHrBorder * sizeof(T));
            srcPtrModTemp += (numOfPixelsHrBorder);
        }
    }

    return RPP_SUCCESS;
}

template <typename T>
RppStatus generate_opposite_edge_padded_image_host(T* srcPtr, RppiSize srcSize, T* srcPtrMod, RppiSize srcSizeMod, Rpp32u padType, 
                                                   RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTemp, *srcPtrModTemp;
    srcPtrTemp = srcPtr;
    srcPtrModTemp = srcPtrMod;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        if (padType == 1)
        {
            Rpp32u imageDim = srcSize.height * srcSize.width;
            Rpp32u boundY = (srcSizeMod.height - srcSize.height) / 2;

            for (int c = 0; c < channel; c++)
            {    
                memset (srcPtrModTemp,(T) 0,boundY * srcSizeMod.width * sizeof(T));
                srcPtrModTemp += (boundY * srcSizeMod.width);

                memcpy(srcPtrModTemp, srcPtrTemp, imageDim * sizeof(T));
                srcPtrModTemp += imageDim;
                srcPtrTemp += imageDim;

                memset (srcPtrModTemp,(T) 0,boundY * srcSizeMod.width * sizeof(T));
                srcPtrModTemp += (boundY * srcSizeMod.width);
            }
        }
        
        if (padType == 2)
        {
            Rpp32u boundX = (srcSizeMod.width - srcSize.width) / 2;

            for (int c = 0; c < channel; c++)
            {
                for (int i = 0; i < srcSize.height; i++)
                {
                    memset (srcPtrModTemp,(T) 0,boundX * sizeof(T));
                    srcPtrModTemp += boundX;

                    memcpy(srcPtrModTemp, srcPtrTemp, srcSize.width * sizeof(T));
                    srcPtrModTemp += srcSize.width;
                    srcPtrTemp += srcSize.width;

                    memset (srcPtrModTemp,(T) 0,boundX * sizeof(T));
                    srcPtrModTemp += boundX;
                }
            }
        }
    }
    else if(chnFormat == RPPI_CHN_PACKED)
    {
        if (padType == 1)
        {
            Rpp32u imageDim = srcSize.height * srcSize.width;
            Rpp32u boundY = (srcSizeMod.height - srcSize.height) / 2;
            Rpp32u numOfPixelsHrBorder = boundY * channel * srcSizeMod.width;

            memset (srcPtrModTemp,(T) 0,numOfPixelsHrBorder * sizeof(T));
            srcPtrModTemp += (numOfPixelsHrBorder);

            memcpy(srcPtrModTemp, srcPtrTemp, channel * imageDim * sizeof(T));
            srcPtrModTemp += (channel * imageDim);
            srcPtrTemp += (channel * imageDim);

            memset (srcPtrModTemp,(T) 0,numOfPixelsHrBorder * sizeof(T));
            srcPtrModTemp += (numOfPixelsHrBorder);
        }

        if (padType == 2)
        {
            Rpp32u boundX = (srcSizeMod.width - srcSize.width) / 2;
            Rpp32u numOfPixelsVtBorder = boundX * channel;
            Rpp32u elementsInRow = channel * srcSize.width;

            for (int i = 0; i < srcSize.height; i++)
            {
                memset (srcPtrModTemp,(T) 0,numOfPixelsVtBorder * sizeof(T));
                srcPtrModTemp += (numOfPixelsVtBorder);

                memcpy(srcPtrModTemp, srcPtrTemp, elementsInRow * sizeof(T));
                srcPtrModTemp += elementsInRow;
                srcPtrTemp += elementsInRow;

                memset (srcPtrModTemp,(T) 0,numOfPixelsVtBorder * sizeof(T));
                srcPtrModTemp += (numOfPixelsVtBorder);
            }
        }
    }

    return RPP_SUCCESS;
}

RppStatus generate_box_kernel_host(Rpp32f* kernel, RppiSize kernelSize)
{
    Rpp32f* kernelTemp;
    kernelTemp = kernel;
    Rpp32f kernelValue = 1.0 / (Rpp32f) (kernelSize.height * kernelSize.width);
    for (int i = 0; i < (kernelSize.height * kernelSize.width); i++)
    {
        *kernelTemp = kernelValue;
        kernelTemp++;
    }

    return RPP_SUCCESS;
}

template <typename T>
RppStatus generate_crop_host(T* srcPtr, RppiSize srcSize, T* srcPtrSubImage, RppiSize srcSizeSubImage, T* dstPtr, 
                             RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrSubImageTemp, *dstPtrTemp;
    srcPtrSubImageTemp = srcPtrSubImage;
    dstPtrTemp = dstPtr;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        Rpp32u remainingElementsInRow = (srcSize.width - srcSizeSubImage.width);
        for (int c = 0; c < channel; c++)
        {
            srcPtrSubImageTemp = srcPtrSubImage + (c * srcSize.height * srcSize.width);
            for (int i = 0; i < srcSizeSubImage.height; i++)
            {
                for (int j = 0; j < srcSizeSubImage.width; j++)
                {
                    *dstPtrTemp = *srcPtrSubImageTemp;
                    srcPtrSubImageTemp++;
                    dstPtrTemp++;
                }
                srcPtrSubImageTemp += remainingElementsInRow;
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        Rpp32u remainingElementsInRow = channel * (srcSize.width - srcSizeSubImage.width);
        for (int i = 0; i < srcSizeSubImage.height; i++)
        {
            for (int j = 0; j < srcSizeSubImage.width; j++)
            {
                for (int c = 0; c < channel; c++)
                {
                    *dstPtrTemp = *srcPtrSubImageTemp;
                    srcPtrSubImageTemp++;
                    dstPtrTemp++;
                }
            }
            srcPtrSubImageTemp += remainingElementsInRow;
        }
    }

    return RPP_SUCCESS;
}

// Unoptimized
///*
RppStatus generate_sobel_kernel_host(Rpp32f* kernel, Rpp32u type)
{
    Rpp32f* kernelTemp;
    kernelTemp = kernel;

    if (type == 1)
    {
        Rpp32f kernelX[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
        Rpp32f* kernelXTemp;
        kernelXTemp = kernelX;

        for (int i = 0; i < 9; i++)
        {
            *kernelTemp = *kernelXTemp;
            kernelTemp++;
            kernelXTemp++;
        }
    }
    else if (type == 2)
    {
        Rpp32f kernelY[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
        Rpp32f* kernelYTemp;
        kernelYTemp = kernelY;

        for (int i = 0; i < 9; i++)
        {
            *kernelTemp = *kernelYTemp;
            kernelTemp++;
            kernelYTemp++;
        }
    }
    else
    {
        return RPP_ERROR;
    }

    return RPP_SUCCESS;
}
//*/

// Optimized - Separable Kernel
/*
RppStatus generate_sobel_kernel_host(Rpp32f* kernel, Rpp32u type)
{
    Rpp32f* kernelTemp;
    kernelTemp = kernel;

    if (type == 1)
    {
        *kernel = -1;
        *(kernel + 1) = 0;
        *(kernel + 2) = 1;
    }
    else if (type == 2)
    {
        *kernel = 1;
        *(kernel + 1) = 2;
        *(kernel + 2) = 1;
    }
    else
    {
        return RPP_ERROR;
    }

    return RPP_SUCCESS;
}
*/

















// Kernels for functions

template<typename T, typename U>
RppStatus convolution_kernel_host(T* srcPtrWindow, U* dstPtrPixel, RppiSize srcSize, 
                                       Rpp32f* kernel, RppiSize kernelSize, Rpp32u remainingElementsInRow, U maxVal, U minVal, 
                                       RppiChnFormat chnFormat, Rpp32u channel)
{
    Rpp32f pixel = 0.0;

    T* srcPtrWindowTemp;
    srcPtrWindowTemp = srcPtrWindow;
    
    Rpp32f* kernelPtrTemp;
    kernelPtrTemp = kernel;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int m = 0; m < kernelSize.height; m++)
        {
            for (int n = 0; n < kernelSize.width; n++)
            {
                pixel += ((*kernelPtrTemp) * (Rpp32f)(*srcPtrWindowTemp));
                kernelPtrTemp++;
                srcPtrWindowTemp++;
            }
            srcPtrWindowTemp += remainingElementsInRow;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int m = 0; m < kernelSize.height; m++)
        {
            for (int n = 0; n < kernelSize.width; n++)
            {
                pixel += ((*kernelPtrTemp) * (Rpp32f)(*srcPtrWindowTemp));
                kernelPtrTemp++;
                srcPtrWindowTemp += channel;
            }
            srcPtrWindowTemp += remainingElementsInRow;
        }
    }
    (pixel < (Rpp32f) minVal) ? pixel = (Rpp32f) minVal : ((pixel < (Rpp32f) maxVal) ? pixel : pixel = (Rpp32f) maxVal);
    *dstPtrPixel = (U) round(pixel);

    return RPP_SUCCESS;
}

template<typename T>
RppStatus histogram_kernel_perChannel_host(T* srcPtr, RppiSize srcSize, Rpp32u* histogram, 
                                Rpp32u bins, Rpp32u increment, 
                                RppiChnFormat chnFormat, Rpp32u channel)
{
    Rpp32u packedIncrement = channel * increment;
    T *srcPtrTemp;
    srcPtrTemp = srcPtr;
    Rpp32u *histogramTemp;
    histogramTemp = histogram;
    int flag = 0;

    Rpp32u elementsInBin = ((Rpp32u)(std::numeric_limits<T>::max()) + 1) / bins;

    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            flag = 0;
            for (int binCheck = 0; binCheck < bins - 1; binCheck++)
            {
                if (*srcPtrTemp >= binCheck * elementsInBin && *srcPtrTemp <= ((binCheck + 1) * elementsInBin) - 1)
                {
                    *(histogramTemp + binCheck) += 1;
                    flag = 1;
                    break;
                }
            }
            if (flag == 0)
            {
                *(histogramTemp + bins - 1) += 1;
            }
            if (chnFormat == RPPI_CHN_PLANAR)
            {
                srcPtrTemp++;
            }
            else if (chnFormat == RPPI_CHN_PACKED)
            {
                srcPtrTemp += channel;
            }
        }
        if (chnFormat == RPPI_CHN_PLANAR)
        {
            srcPtrTemp += increment;
        }
        else if (chnFormat == RPPI_CHN_PACKED)
        {
            srcPtrTemp += packedIncrement;
        }
    }

    return RPP_SUCCESS;
}

template<typename T>
RppStatus histogram_kernel_host(T* srcPtr, RppiSize srcSize, Rpp32u* histogram, 
                                Rpp8u bins, 
                                Rpp32u channel)
{
    if (bins == 0)
    {
        *histogram = channel * srcSize.height * srcSize.width;
    }
    else
    {
        Rpp8u rangeInBin = 256 / (bins + 1);
        T *srcPtrTemp;
        srcPtrTemp = srcPtr;
        for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
        {
            *(histogram + (*srcPtrTemp / rangeInBin)) += 1;
            srcPtrTemp++;
        }
    }

    return RPP_SUCCESS;
}

template <typename T, typename U>
inline RppStatus accumulate_kernel_host(T* srcPtr1, U* srcPtr2, RppiSize srcSize,
                                        RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtr1Temp;
    U *srcPtr2Temp;
    srcPtr1Temp = srcPtr1;
    srcPtr2Temp = srcPtr2;

    Rpp32s pixel;
    
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        pixel = ((Rpp32s) (*srcPtr1Temp)) + ((Rpp32s) (*srcPtr2Temp));
        pixel = RPPPIXELCHECK(pixel);
        *srcPtr1Temp =(T) pixel;
        srcPtr1Temp++;
        srcPtr2Temp++;
    }

    return RPP_SUCCESS;

}

template <typename T>
RppStatus resize_kernel_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           RppiChnFormat chnFormat, Rpp32u channel)
{
    if (dstSize.height < 0 || dstSize.width < 0)
    {
        return RPP_ERROR;
    }

    Rpp32f hRatio = (((Rpp32f) (dstSize.height - 1)) / ((Rpp32f) (srcSize.height - 1)));
    Rpp32f wRatio = (((Rpp32f) (dstSize.width - 1)) / ((Rpp32f) (srcSize.width - 1)));
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
                srcLocationRow = ((Rpp32f) i) / hRatio;
                srcLocationRowFloor = (Rpp32s) RPPFLOOR(srcLocationRow);
                Rpp32f weightedHeight = srcLocationRow - srcLocationRowFloor;
                if (srcLocationRowFloor > (srcSize.height - 2))
                {
                    srcLocationRowFloor = srcSize.height - 2;
                }

                srcPtrTopRow = srcPtrTemp + srcLocationRowFloor * srcSize.width;
                srcPtrBottomRow  = srcPtrTopRow + srcSize.width;
                
                for (int j = 0; j < dstSize.width; j++)
                {
                    srcLocationColumn = ((Rpp32f) j) / wRatio;
                    srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);
                    Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;

                    if (srcLocationColumnFloor > (srcSize.width - 2))
                    {
                        srcLocationColumnFloor = srcSize.width - 2;
                    }
                    pixel = ((*(srcPtrTopRow + srcLocationColumnFloor)) * (1 - weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrTopRow + srcLocationColumnFloor + 1)) * (1 - weightedHeight) * (weightedWidth)) 
                            + ((*(srcPtrBottomRow + srcLocationColumnFloor)) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrBottomRow + srcLocationColumnFloor + 1)) * (weightedHeight) * (weightedWidth));
                    
                    *dstPtrTemp = (T) round(pixel);
                    dstPtrTemp ++;
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
            srcLocationRow = ((Rpp32f) i) / hRatio;
            srcLocationRowFloor = (Rpp32s) RPPFLOOR(srcLocationRow);
            Rpp32f weightedHeight = srcLocationRow - srcLocationRowFloor;

            if (srcLocationRowFloor > (srcSize.height - 2))
            {
                srcLocationRowFloor = srcSize.height - 2;
            }

            srcPtrTopRow = srcPtrTemp + srcLocationRowFloor * elementsInRow;
            srcPtrBottomRow  = srcPtrTopRow + elementsInRow;

            for (int j = 0; j < dstSize.width; j++)
            {   
                srcLocationColumn = ((Rpp32f) j) / wRatio;
                srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);
                Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;

                if (srcLocationColumnFloor > (srcSize.width - 2))
                {
                    srcLocationColumnFloor = srcSize.width - 2;
                }

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

    return RPP_SUCCESS;
}

template <typename T>
RppStatus resize_crop_kernel_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           Rpp32u x1, Rpp32u y1, Rpp32u x2, Rpp32u y2,
                           RppiChnFormat chnFormat, Rpp32u channel)
{
    RppiSize srcSizeSubImage;
    T *srcPtrSubImage;

    compute_subimage_location_host(srcPtr, &srcPtrSubImage, srcSize, &srcSizeSubImage, x1, y1, x2, y2, chnFormat, channel);

    T *srcPtrResize = (T*) calloc(channel * srcSizeSubImage.height * srcSizeSubImage.width, sizeof(T));

    generate_crop_host(srcPtr, srcSize, srcPtrSubImage, srcSizeSubImage, srcPtrResize, chnFormat, channel);

    resize_kernel_host(srcPtrResize, srcSizeSubImage, dstPtr, dstSize, chnFormat, channel);

    return RPP_SUCCESS;
    
}

template<typename T>
RppStatus erode_kernel_host(T* srcPtrWindow, T* dstPtrPixel, RppiSize srcSize, 
                                       Rpp32u kernelSize, Rpp32u remainingElementsInRow, 
                                       RppiChnFormat chnFormat, Rpp32u channel)
{
    T pixel;

    T* srcPtrWindowTemp;
    srcPtrWindowTemp = srcPtrWindow;
    pixel = *srcPtrWindowTemp;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int m = 0; m < kernelSize; m++)
        {
            for (int n = 0; n < kernelSize; n++)
            {
                if (*srcPtrWindowTemp < pixel)
                {
                    pixel = *srcPtrWindowTemp;
                }
                srcPtrWindowTemp++;
            }
            srcPtrWindowTemp += remainingElementsInRow;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int m = 0; m < kernelSize; m++)
        {
            for (int n = 0; n < kernelSize; n++)
            {
                if (*srcPtrWindowTemp < pixel)
                {
                    pixel = *srcPtrWindowTemp;
                }
                srcPtrWindowTemp += channel;
            }
            srcPtrWindowTemp += remainingElementsInRow;
        }
    }
    *dstPtrPixel = pixel;

    return RPP_SUCCESS;
}

template<typename T>
RppStatus dilate_kernel_host(T* srcPtrWindow, T* dstPtrPixel, RppiSize srcSize, 
                                       Rpp32u kernelSize, Rpp32u remainingElementsInRow, 
                                       RppiChnFormat chnFormat, Rpp32u channel)
{
    T pixel;

    T* srcPtrWindowTemp;
    srcPtrWindowTemp = srcPtrWindow;
    pixel = *srcPtrWindowTemp;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int m = 0; m < kernelSize; m++)
        {
            for (int n = 0; n < kernelSize; n++)
            {
                if (*srcPtrWindowTemp > pixel)
                {
                    pixel = *srcPtrWindowTemp;
                }
                srcPtrWindowTemp++;
            }
            srcPtrWindowTemp += remainingElementsInRow;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int m = 0; m < kernelSize; m++)
        {
            for (int n = 0; n < kernelSize; n++)
            {
                if (*srcPtrWindowTemp > pixel)
                {
                    pixel = *srcPtrWindowTemp;
                }
                srcPtrWindowTemp += channel;
            }
            srcPtrWindowTemp += remainingElementsInRow;
        }
    }
    *dstPtrPixel = pixel;

    return RPP_SUCCESS;
}

//O(1)
///*
template<typename T>
RppStatus median_filter_row_kernel_host(T* srcPtrWindow, T* dstPtrPixel, RppiSize srcSize, 
                                       Rpp32u kernelSize, Rpp32u remainingElementsInRow, Rpp32f limit, Rpp32u *histogram, 
                                       RppiChnFormat chnFormat, Rpp32u channel)
{
    T* srcPtrWindowTemp;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        srcPtrWindowTemp = srcPtrWindow - 1;
        for (int m = 0; m < kernelSize; m++)
        {
            *(histogram + (Rpp32u) *(srcPtrWindowTemp)) -= 1;
            srcPtrWindowTemp += kernelSize;
            *(histogram + (Rpp32u) *(srcPtrWindowTemp)) += 1;
            srcPtrWindowTemp += remainingElementsInRow;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        srcPtrWindowTemp = srcPtrWindow - channel;
        for (int m = 0; m < kernelSize; m++)
        {
            *(histogram + (Rpp32u) *(srcPtrWindowTemp)) -= 1;
            srcPtrWindowTemp += (kernelSize * channel);
            *(histogram + (Rpp32u) *(srcPtrWindowTemp)) += 1;
            srcPtrWindowTemp += remainingElementsInRow;
        }
    }

    Rpp32u sum = 0;
    Rpp32u *histogramTemp;
    histogramTemp = histogram;
    while (sum < limit)
    {
        sum += *(histogramTemp);
        histogramTemp++;
    }

    *dstPtrPixel = (T) (histogramTemp - histogram);

    return RPP_SUCCESS;
}
//*/

//O(r)
///*
template<typename T>
RppStatus median_filter_kernel_host(T* srcPtrWindow, T* dstPtrPixel, RppiSize srcSize, 
                                       Rpp32u kernelSize, Rpp32u remainingElementsInRow, Rpp32f limit, Rpp32u *histogram, 
                                       RppiChnFormat chnFormat, Rpp32u channel)
{
    T* srcPtrWindowTemp;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        srcPtrWindowTemp = srcPtrWindow - 1;
        for (int m = 0; m < kernelSize; m++)
        {
            *(histogram + (Rpp32u) *(srcPtrWindowTemp)) -= 1;
            srcPtrWindowTemp += kernelSize;
            *(histogram + (Rpp32u) *(srcPtrWindowTemp)) += 1;
            srcPtrWindowTemp += remainingElementsInRow;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        srcPtrWindowTemp = srcPtrWindow - channel;
        for (int m = 0; m < kernelSize; m++)
        {
            *(histogram + (Rpp32u) *(srcPtrWindowTemp)) -= 1;
            srcPtrWindowTemp += (kernelSize * channel);
            *(histogram + (Rpp32u) *(srcPtrWindowTemp)) += 1;
            srcPtrWindowTemp += remainingElementsInRow;
        }
    }

    Rpp32u sum = 0;
    Rpp32u *histogramTemp;
    histogramTemp = histogram;
    while (sum < limit)
    {
        sum += *(histogramTemp);
        histogramTemp++;
    }

    *dstPtrPixel = (T) (histogramTemp - histogram);

    return RPP_SUCCESS;
}
//*/

//O(r^2)
/*
template<typename T>
RppStatus median_filter_kernel_host(T* srcPtrWindow, T* dstPtrPixel, RppiSize srcSize, 
                                       Rpp32u kernelSize, Rpp32u remainingElementsInRow, 
                                       RppiChnFormat chnFormat, Rpp32u channel)
{
    T *kernel = (T*)calloc(kernelSize * kernelSize, sizeof(T));
    T *kernelTemp;
    kernelTemp = kernel;

    T* srcPtrWindowTemp;
    srcPtrWindowTemp = srcPtrWindow;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int m = 0; m < kernelSize; m++)
        {
            for (int n = 0; n < kernelSize; n++)
            {
                *kernelTemp = *srcPtrWindowTemp;
                srcPtrWindowTemp++;
                kernelTemp++;
            }
            srcPtrWindowTemp += remainingElementsInRow;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int m = 0; m < kernelSize; m++)
        {
            for (int n = 0; n < kernelSize; n++)
            {
                *kernelTemp = *srcPtrWindowTemp;
                srcPtrWindowTemp += channel;
                kernelTemp++;
            }
            srcPtrWindowTemp += remainingElementsInRow;
        }
    }

    std::sort(kernel, kernel + (kernelSize * kernelSize));

    *dstPtrPixel = *(kernel + (((kernelSize * kernelSize) - 1) / 2));

    return RPP_SUCCESS;
}
*/

template<typename T>
RppStatus local_binary_pattern_kernel_host(T* srcPtrWindow, T* dstPtrPixel, RppiSize srcSize, 
                                       Rpp32u remainingElementsInRow, T* centerPixelPtr, 
                                       RppiChnFormat chnFormat, Rpp32u channel)
{
    T pixel = (T) 0;
    T *srcPtrWindowTemp;
    srcPtrWindowTemp = srcPtrWindow;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        if (*srcPtrWindowTemp - *centerPixelPtr >= 0)
        {
            pixel += pow(2, 0);
        }
        srcPtrWindowTemp++;

        if (*srcPtrWindowTemp - *centerPixelPtr >= 0)
        {
            pixel += pow(2, 1);
        }
        srcPtrWindowTemp++;

        if (*srcPtrWindowTemp - *centerPixelPtr >= 0)
        {
            pixel += pow(2, 2);
        }
        srcPtrWindowTemp++;
        srcPtrWindowTemp += remainingElementsInRow;

        if (*srcPtrWindowTemp - *centerPixelPtr >= 0)
        {
            pixel += pow(2, 7);
        }
        srcPtrWindowTemp += 2;

        if (*srcPtrWindowTemp - *centerPixelPtr >= 0)
        {
            pixel += pow(2, 3);
        }
        srcPtrWindowTemp++;
        srcPtrWindowTemp += remainingElementsInRow;

        if (*srcPtrWindowTemp - *centerPixelPtr >= 0)
        {
            pixel += pow(2, 6);
        }
        srcPtrWindowTemp++;

        if (*srcPtrWindowTemp - *centerPixelPtr >= 0)
        {
            pixel += pow(2, 5);
        }
        srcPtrWindowTemp++;

        if (*srcPtrWindowTemp - *centerPixelPtr >= 0)
        {
            pixel += pow(2, 4);
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        if (*srcPtrWindowTemp - *centerPixelPtr >= 0)
        {
            pixel += pow(2, 0);
        }
        srcPtrWindowTemp += channel;

        if (*srcPtrWindowTemp - *centerPixelPtr >= 0)
        {
            pixel += pow(2, 1);
        }
        srcPtrWindowTemp += channel;

        if (*srcPtrWindowTemp - *centerPixelPtr >= 0)
        {
            pixel += pow(2, 2);
        }
        srcPtrWindowTemp += channel;
        srcPtrWindowTemp += remainingElementsInRow;

        if (*srcPtrWindowTemp - *centerPixelPtr >= 0)
        {
            pixel += pow(2, 7);
        }
        srcPtrWindowTemp += (2 * channel);

        if (*srcPtrWindowTemp - *centerPixelPtr >= 0)
        {
            pixel += pow(2, 3);
        }
        srcPtrWindowTemp += channel;
        srcPtrWindowTemp += remainingElementsInRow;

        if (*srcPtrWindowTemp - *centerPixelPtr >= 0)
        {
            pixel += pow(2, 6);
        }
        srcPtrWindowTemp += channel;

        if (*srcPtrWindowTemp - *centerPixelPtr >= 0)
        {
            pixel += pow(2, 5);
        }
        srcPtrWindowTemp += channel;

        if (*srcPtrWindowTemp - *centerPixelPtr >= 0)
        {
            pixel += pow(2, 4);
        }
    }

    *dstPtrPixel = pixel;

    return RPP_SUCCESS;
}

template<typename T>
RppStatus non_max_suppression_kernel_host(T* srcPtrWindow, T* dstPtrPixel, RppiSize srcSize, 
                                       Rpp32u kernelSize, Rpp32u remainingElementsInRow, T windowCenter, 
                                       RppiChnFormat chnFormat, Rpp32u channel)
{
    T pixel;

    T* srcPtrWindowTemp;
    srcPtrWindowTemp = srcPtrWindow;
    pixel = *srcPtrWindowTemp;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int m = 0; m < kernelSize; m++)
        {
            for (int n = 0; n < kernelSize; n++)
            {
                if (windowCenter < *srcPtrWindowTemp)
                {
                    *dstPtrPixel = (T) 0;
                    return RPP_SUCCESS;
                }
                srcPtrWindowTemp++;
            }
            srcPtrWindowTemp += remainingElementsInRow;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int m = 0; m < kernelSize; m++)
        {
            for (int n = 0; n < kernelSize; n++)
            {
                if (windowCenter < *srcPtrWindowTemp)
                {
                    *dstPtrPixel = (T) 0;
                    return RPP_SUCCESS;
                }
                srcPtrWindowTemp += channel;
            }
            srcPtrWindowTemp += remainingElementsInRow;
        }
    }
    *dstPtrPixel = windowCenter;

    return RPP_SUCCESS;
}

template<typename T>
RppStatus canny_non_max_suppression_kernel_host(T* dstPtrPixel, T windowCenter, T *position1Ptr, T *position2Ptr)
{
    if ((windowCenter >= *position1Ptr) && (windowCenter >= *position2Ptr))
    {
        *dstPtrPixel = windowCenter;
    }
    else
    {
        *dstPtrPixel = (T) 0;
    }

    return RPP_SUCCESS;
}

template<typename T>
RppStatus canny_hysterisis_edge_tracing_kernel_host(T* srcPtrWindow, T* dstPtrPixel, RppiSize srcSize, 
                                       Rpp32u kernelSize, Rpp32u remainingElementsInRow, T windowCenter, Rpp32u bound, 
                                       RppiChnFormat chnFormat, Rpp32u channel)
{
    T pixel;

    T* srcPtrWindowTemp;
    srcPtrWindowTemp = srcPtrWindow;
    pixel = *srcPtrWindowTemp;
    
    for (int m = 0; m < kernelSize; m++)
    {
        for (int n = 0; n < kernelSize; n++)
        {
            if (*srcPtrWindowTemp == (T) 255)
            {
                *dstPtrPixel = (T) 255;
                return RPP_SUCCESS;
            }
            srcPtrWindowTemp++;
        }
        srcPtrWindowTemp += remainingElementsInRow;
    }
    *dstPtrPixel = (T) 0;

    return RPP_SUCCESS;
}

template<typename T, typename U>
//RppStatus harris_corner_detector_kernel_host(T* srcPtrWindowX, T* srcPtrWindowY, U* dstPtrPixel, RppiSize srcSize, 
//                                             Rpp32u kernelSize, Rpp32u remainingElementsInRow, Rpp32f kValue, Rpp32f *min, Rpp32f *max, Rpp32f threshold, 
//                                             RppiChnFormat chnFormat, Rpp32u channel)
RppStatus harris_corner_detector_kernel_host(T* srcPtrWindowX, T* srcPtrWindowY, U* dstPtrPixel, RppiSize srcSize, 
                                             Rpp32u kernelSize, Rpp32u remainingElementsInRow, Rpp32f kValue, Rpp32f threshold, 
                                             RppiChnFormat chnFormat, Rpp32u channel)
{
    Rpp32f pixel;

    T *srcPtrWindowTempX, *srcPtrWindowTempY;
    srcPtrWindowTempX = srcPtrWindowX;
    srcPtrWindowTempY = srcPtrWindowY;

    Rpp32f sumXX = 0, sumYY = 0, sumXY = 0, valX = 0, valY = 0;
    
    for (int m = 0; m < kernelSize; m++)
    {
        for (int n = 0; n < kernelSize; n++)
        {
            valX = (Rpp32f) *srcPtrWindowTempX;
            valY = (Rpp32f) *srcPtrWindowTempY;
            sumXX += (valX * valX);
            sumYY += (valY * valY);
            sumXY += (valX * valY);

            srcPtrWindowTempX++;
            srcPtrWindowTempY++;
        }
        srcPtrWindowTempX += remainingElementsInRow;
        srcPtrWindowTempY += remainingElementsInRow;
    }
    Rpp32f det = (sumXX * sumYY) - (sumXY * sumXY);
    Rpp32f trace = sumXX + sumYY;
    pixel = (det) - (kValue * trace * trace);

    //if (pixel < *min)
    //{
    //    *min = pixel;
    //}
    //if (pixel > *max)
    //{
    //    *max = pixel;
    //}

    if (pixel > threshold)
    {
        *dstPtrPixel = (U) pixel;
    }
    else
    {
        *dstPtrPixel = (U) 0;
    }

    return RPP_SUCCESS;
}

template<typename T>
RppStatus harris_corner_set_maximum_kernel_host(T* dstPtrWindow, Rpp32u kernelSize, Rpp32u remainingElementsInRow, 
                                                  RppiChnFormat chnFormat, Rpp32u channel)
{
    T* dstPtrWindowTemp;
    dstPtrWindowTemp = dstPtrWindow;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int m = 0; m < kernelSize; m++)
        {
            for (int n = 0; n < kernelSize; n++)
            {
                *dstPtrWindowTemp = (T) 255;
                dstPtrWindowTemp++;
            }
            dstPtrWindowTemp += remainingElementsInRow;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int m = 0; m < kernelSize; m++)
        {
            for (int n = 0; n < kernelSize; n++)
            {
                *dstPtrWindowTemp = (T) 255;
                dstPtrWindowTemp += channel;
            }
            dstPtrWindowTemp += remainingElementsInRow;
        }
    }

    return RPP_SUCCESS;
}

template<typename T>
RppStatus harris_corner_set_minimum_kernel_host(T* dstPtrWindow, Rpp32u kernelSize, Rpp32u remainingElementsInRow, 
                                                  RppiChnFormat chnFormat, Rpp32u channel)
{
    T* dstPtrWindowTemp;
    dstPtrWindowTemp = dstPtrWindow;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int m = 0; m < kernelSize; m++)
        {
            for (int n = 0; n < kernelSize; n++)
            {
                *dstPtrWindowTemp = (T) 0;
                dstPtrWindowTemp++;
            }
            dstPtrWindowTemp += remainingElementsInRow;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int m = 0; m < kernelSize; m++)
        {
            for (int n = 0; n < kernelSize; n++)
            {
                *dstPtrWindowTemp = (T) 0;
                dstPtrWindowTemp += channel;
            }
            dstPtrWindowTemp += remainingElementsInRow;
        }
    }

    return RPP_SUCCESS;
}

template<typename T, typename U>
RppStatus match_template_kernel_host(T* srcPtrWindow, U* dstPtrWindow, 
                                     T* templateImage, RppiSize templateImageSize, Rpp32u remainingElementsInRow)
{
    T *srcPtrWindowTemp, *templateImageTemp;
    srcPtrWindowTemp = srcPtrWindow;
    templateImageTemp = templateImage;

    Rpp32f sumSqrDiff = 0, sumSqrIntTemplate = 0, sumSqrIntSrc = 0, srcValue, templateValue, diff;

    for (int m = 0; m < templateImageSize.height; m++)
    {
        for (int n = 0; n < templateImageSize.width; n++)
        {
            srcValue = (Rpp32f) *srcPtrWindowTemp;
            templateValue = (Rpp32f) *templateImageTemp;
            diff = srcValue - templateValue;
            
            sumSqrDiff += (diff * diff);
            sumSqrIntSrc += (srcValue * srcValue);
            sumSqrIntTemplate += (templateValue * templateValue);

            srcPtrWindowTemp++;
            templateImageTemp++;
        }
        srcPtrWindowTemp += remainingElementsInRow;
    }

    Rpp32f u16Normalizer = 65535 / 253;
    *dstPtrWindow = (U) (RPP16UPIXELCHECK((sumSqrDiff / sqrt(sumSqrIntSrc * sumSqrIntTemplate)) * u16Normalizer));

    return RPP_SUCCESS;
}

//RppStatus tensor_index_print_kernel_host(Rpp32u *loopCount, Rpp32u tensorDimension)
//{
//    Rpp32u *loopCountTemp;
//    loopCountTemp = loopCount;
//
//    for (int i =0; i < tensorDimension; i++)
//    {
//        printf("%d  ", *loopCountTemp);
//        loopCountTemp++;
//    }
//    printf("\n");
//    
//    return RPP_SUCCESS;
//}

RppStatus tensor_index_exchange_kernel_host(Rpp32u *loopCount, Rpp32u *loopCountTransposed, Rpp32u tensorDimension, Rpp32u dimension1, Rpp32u dimension2)
{
    memcpy(loopCountTransposed, loopCount, tensorDimension * sizeof(Rpp32u));

    loopCountTransposed[dimension2] = loopCount[dimension1];
    loopCountTransposed[dimension1] = loopCount[dimension2];
    
    return RPP_SUCCESS;
}

template<typename T>
RppStatus tensor_transpose_iterate_kernel_host(T* srcPtr, T* dstPtr, 
                                               Rpp32u tensorDimensionTemp, Rpp32u tensorDimension, 
                                               Rpp32u *tensorDimensionValues, Rpp32u *tensorDimensionValuesProduct, 
                                               Rpp32u *loopCount, Rpp32u *loopCountTransposed, 
                                               Rpp32u dimension1, Rpp32u dimension2)
{
    if (tensorDimensionTemp >= tensorDimension)
    {
        Rpp32u dstPtrLoc = 0;
        for (int i = tensorDimension - 1; i > 0 ; i--)
        {
            dstPtrLoc = dstPtrLoc + (loopCount[i] * tensorDimensionValuesProduct[i - 1]);
        }
        dstPtrLoc += loopCount[0];
        //printf("\ndstValue was %d\n", *(dstPtr + dstPtrLoc));
        
        //tensor_index_print_kernel_host(loopCount, tensorDimension);
        tensor_index_exchange_kernel_host(loopCount, loopCountTransposed, tensorDimension, dimension1, dimension2);
        //tensor_index_print_kernel_host(loopCountTransposed, tensorDimension);
        
        Rpp32u srcPtrLoc = 0;
        for (int i = tensorDimension - 1; i > 0 ; i--)
        {
            srcPtrLoc = srcPtrLoc + (loopCountTransposed[i] * tensorDimensionValuesProduct[i - 1]);
        }
        srcPtrLoc += loopCountTransposed[0];
        //printf("New srcValue = %d", *(srcPtr + srcPtrLoc));

        //printf("\n");
        
        *(dstPtr + dstPtrLoc) = *(srcPtr + srcPtrLoc);
        
        return RPP_SUCCESS;
    }
    for (int i = 0; i < *(tensorDimensionValues + tensorDimensionTemp); i++)
    { 
        *(loopCount + tensorDimensionTemp) = i;
        tensor_transpose_iterate_kernel_host(srcPtr, dstPtr, 
                                             tensorDimensionTemp + 1, tensorDimension, 
                                             tensorDimensionValues, tensorDimensionValuesProduct, 
                                             loopCount, loopCountTransposed, 
                                             dimension1, dimension2);
    }

    return RPP_SUCCESS;
}

template<typename T>
RppStatus fast_corner_detector_kernel_host(T* srcPtrWindow, T* dstPtrPixel, RppiSize srcSize, 
                                           Rpp32u* bresenhamCirclePositions, T threshold, Rpp32u numOfPixels)
{
    T centerPixel = *(srcPtrWindow + (3 * srcSize.width) + 3);
    T max = (T) (RPPPIXELCHECK((Rpp32s) centerPixel + (Rpp32s) threshold));
    T min = (T) (RPPPIXELCHECK((Rpp32s) centerPixel - (Rpp32s) threshold));

    // Find Bresenham Circle for the pixel

    Rpp32u *bresenhamCirclePositionsTemp;
    bresenhamCirclePositionsTemp = bresenhamCirclePositions;
    
    T *bresenhamCircle = (T*) calloc(16, sizeof(T));
    T *bresenhamCircleTemp;
    bresenhamCircleTemp = bresenhamCircle;

    //T* bresenhamCircleOutputMin = (T*) calloc(16, sizeof(T));
    //T *bresenhamCircleOutputMinTemp;
    //bresenhamCircleOutputMinTemp = bresenhamCircleOutputMin;

    //T* bresenhamCircleOutputMax = (T*) calloc(16, sizeof(T));
    //T *bresenhamCircleOutputMaxTemp;
    //bresenhamCircleOutputMaxTemp = bresenhamCircleOutputMax;
    
    T* bresenhamCircleOutput = (T*) calloc(16, sizeof(T));

    for (int i = 0; i < 16; i++)
    {
        *bresenhamCircleTemp = *(srcPtrWindow + *bresenhamCirclePositionsTemp);
        bresenhamCircleTemp++;
        bresenhamCirclePositionsTemp++;
    }

    //Rpp32u minConditions = 0;
    //Rpp32u maxConditions = 0;
    Rpp32u flag = 0;
    
    *bresenhamCircleOutput = (T) RPPISLESSER(*bresenhamCircle, min);
    *(bresenhamCircleOutput + 8) = (T) RPPISLESSER(*(bresenhamCircle + 8), min);

    if (*bresenhamCircleOutput == 1)
    {
        *(bresenhamCircleOutput + 4) = (T) RPPISLESSER(*(bresenhamCircle + 4), min);
        *(bresenhamCircleOutput + 12) = (T) RPPISLESSER(*(bresenhamCircle + 12), min);
        if (*(bresenhamCircleOutput + 8) == 1)
        {
            if (*(bresenhamCircleOutput + 4) == 1 || *(bresenhamCircleOutput + 12) == 1)
            {
                flag = 1;
            }
        }
        else if (*(bresenhamCircleOutput + 4) == 1 && *(bresenhamCircleOutput + 12) == 1)
        {
            flag = 1;
        }
    }
    else if (*(bresenhamCircleOutput + 8) == 1)
    {
        *(bresenhamCircleOutput + 4) = (T) RPPISLESSER(*(bresenhamCircle + 4), min);
        *(bresenhamCircleOutput + 12) = (T) RPPISLESSER(*(bresenhamCircle + 12), min);
        if (*(bresenhamCircleOutput + 4) == 1 && *(bresenhamCircleOutput + 12) == 1)
        {
            flag = 1;
        }
    }
    if (flag == 0)
    {
        *bresenhamCircleOutput = (T) RPPISGREATER(*bresenhamCircle, max);
        *(bresenhamCircleOutput + 8) = (T) RPPISGREATER(*(bresenhamCircle + 8), max);

        if (*bresenhamCircleOutput == 1)
        {
            *(bresenhamCircleOutput + 4) = (T) RPPISGREATER(*(bresenhamCircle + 4), max);
            *(bresenhamCircleOutput + 12) = (T) RPPISGREATER(*(bresenhamCircle + 12), max);
            if (*(bresenhamCircleOutput + 8) == 1)
            {
                if (*(bresenhamCircleOutput + 4) == 1 || *(bresenhamCircleOutput + 12) == 1)
                {
                    flag = 2;
                }
            }
            else if (*(bresenhamCircleOutput + 4) == 1 && *(bresenhamCircleOutput + 12) == 1)
            {
                flag = 2;
            }
        }
        else if (*(bresenhamCircleOutput + 8) == 1)
        {
            *(bresenhamCircleOutput + 4) = (T) RPPISGREATER(*(bresenhamCircle + 4), max);
            *(bresenhamCircleOutput + 12) = (T) RPPISGREATER(*(bresenhamCircle + 12), max);
            if (*(bresenhamCircleOutput + 4) == 1 && *(bresenhamCircleOutput + 12) == 1)
            {
                flag = 2;
            }
        }
    }
    if (flag == 0)
    {
        *dstPtrPixel = (T) 0;

        return RPP_SUCCESS;
    }
    else if (flag == 1)
    {
        *(bresenhamCircleOutput + 1) = (T) RPPISLESSER(*(bresenhamCircle + 1), min);
        *(bresenhamCircleOutput + 2) = (T) RPPISLESSER(*(bresenhamCircle + 2), min);
        *(bresenhamCircleOutput + 3) = (T) RPPISLESSER(*(bresenhamCircle + 3), min);
        *(bresenhamCircleOutput + 5) = (T) RPPISLESSER(*(bresenhamCircle + 5), min);
        *(bresenhamCircleOutput + 6) = (T) RPPISLESSER(*(bresenhamCircle + 6), min);
        *(bresenhamCircleOutput + 7) = (T) RPPISLESSER(*(bresenhamCircle + 7), min);
        *(bresenhamCircleOutput + 9) = (T) RPPISLESSER(*(bresenhamCircle + 9), min);
        *(bresenhamCircleOutput + 10) = (T) RPPISLESSER(*(bresenhamCircle + 10), min);
        *(bresenhamCircleOutput + 11) = (T) RPPISLESSER(*(bresenhamCircle + 11), min);
        *(bresenhamCircleOutput + 13) = (T) RPPISLESSER(*(bresenhamCircle + 13), min);
        *(bresenhamCircleOutput + 14) = (T) RPPISLESSER(*(bresenhamCircle + 14), min);
        *(bresenhamCircleOutput + 15) = (T) RPPISLESSER(*(bresenhamCircle + 15), min);
    }
    else if (flag == 2)
    {
        *(bresenhamCircleOutput + 1) = (T) RPPISGREATER(*(bresenhamCircle + 1), max);
        *(bresenhamCircleOutput + 2) = (T) RPPISGREATER(*(bresenhamCircle + 2), max);
        *(bresenhamCircleOutput + 3) = (T) RPPISGREATER(*(bresenhamCircle + 3), max);
        *(bresenhamCircleOutput + 5) = (T) RPPISGREATER(*(bresenhamCircle + 5), max);
        *(bresenhamCircleOutput + 6) = (T) RPPISGREATER(*(bresenhamCircle + 6), max);
        *(bresenhamCircleOutput + 7) = (T) RPPISGREATER(*(bresenhamCircle + 7), max);
        *(bresenhamCircleOutput + 9) = (T) RPPISGREATER(*(bresenhamCircle + 9), max);
        *(bresenhamCircleOutput + 10) = (T) RPPISGREATER(*(bresenhamCircle + 10), max);
        *(bresenhamCircleOutput + 11) = (T) RPPISGREATER(*(bresenhamCircle + 11), max);
        *(bresenhamCircleOutput + 13) = (T) RPPISGREATER(*(bresenhamCircle + 13), max);
        *(bresenhamCircleOutput + 14) = (T) RPPISGREATER(*(bresenhamCircle + 14), max);
        *(bresenhamCircleOutput + 15) = (T) RPPISGREATER(*(bresenhamCircle + 15), max);
    }
/*    
    if (*bresenhamCircleOutput == 1)
    {
        conditions++;
    }
    
    if (*(bresenhamCircleOutput + 8) == 1)
    {
        conditions++;
    }

    if (conditions == 2)
    {
        *dstPtrPixel = (T) 0;

        return RPP_SUCCESS;
    }
    
    *(bresenhamCircleOutput + 4) = (T) (RPPINRANGE(*(bresenhamCircle + 4), min, max));
    *(bresenhamCircleOutput + 12) = (T) (RPPINRANGE(*(bresenhamCircle + 12), min, max));
    
    if (conditions == 1)
    {
        if (*(bresenhamCircleOutput + 4) == 1 || *(bresenhamCircleOutput + 12) == 1)
        {
            *dstPtrPixel = (T) 0;

            return RPP_SUCCESS;
        }
    }
    else if (conditions == 0)
    {
        if (*(bresenhamCircleOutput + 4) == 1 && *(bresenhamCircleOutput + 12) == 1)
        {
            *dstPtrPixel = (T) 0;

            return RPP_SUCCESS;
        }
    }

    *(bresenhamCircleOutput + 1) = (T) (RPPINRANGE(*(bresenhamCircle + 1), min, max));
    *(bresenhamCircleOutput + 2) = (T) (RPPINRANGE(*(bresenhamCircle + 2), min, max));
    *(bresenhamCircleOutput + 3) = (T) (RPPINRANGE(*(bresenhamCircle + 3), min, max));
    *(bresenhamCircleOutput + 5) = (T) (RPPINRANGE(*(bresenhamCircle + 5), min, max));
    *(bresenhamCircleOutput + 6) = (T) (RPPINRANGE(*(bresenhamCircle + 6), min, max));
    *(bresenhamCircleOutput + 7) = (T) (RPPINRANGE(*(bresenhamCircle + 7), min, max));
    *(bresenhamCircleOutput + 9) = (T) (RPPINRANGE(*(bresenhamCircle + 9), min, max));
    *(bresenhamCircleOutput + 10) = (T) (RPPINRANGE(*(bresenhamCircle + 10), min, max));
    *(bresenhamCircleOutput + 11) = (T) (RPPINRANGE(*(bresenhamCircle + 11), min, max));
    *(bresenhamCircleOutput + 13) = (T) (RPPINRANGE(*(bresenhamCircle + 13), min, max));
    *(bresenhamCircleOutput + 14) = (T) (RPPINRANGE(*(bresenhamCircle + 14), min, max));
    *(bresenhamCircleOutput + 15) = (T) (RPPINRANGE(*(bresenhamCircle + 15), min, max));
*/
    // Find maximum contiguous pixels in bresenhamCircleOutput with value 1
    
    Rpp32u count = 0;
    Rpp32u maxLength = 0;
    
    for (int i = 0; i < 32; i++)
    {
        if (*(bresenhamCircleOutput + (i % 16)) == 0)
        {
            count = 0;
            if (i >= 16)
            {
                break;
            } 
        }
        else
        {
            count++;
            maxLength = RPPMAX2(maxLength, count);
        }
    }
    
    // Corner Classification

    if (maxLength >= numOfPixels)
    {
        *dstPtrPixel = (T) 255;
    }
    else
    {
        *dstPtrPixel = (T) 0;
    }
    
    
    return RPP_SUCCESS;
}

template<typename T, typename U>
RppStatus fast_corner_detector_score_function_kernel_host(T* srcPtrWindow, U* dstPtrPixel, RppiSize srcSize, 
                                                          Rpp32u* bresenhamCirclePositions, U centerPixel)
{
    //U centerPixel = (U) *(srcPtrWindow + (3 * srcSize.width) + 3);
    U* bresenhamCircle = (U*) calloc(16, sizeof(U));
    U *bresenhamCircleTemp;
    bresenhamCircleTemp = bresenhamCircle;
    Rpp32u *bresenhamCirclePositionsTemp;
    bresenhamCirclePositionsTemp = bresenhamCirclePositions;
    
    for (int i = 0; i < 16; i++)
    {
        *bresenhamCircleTemp = (U) *(srcPtrWindow + *bresenhamCirclePositionsTemp);
        bresenhamCircleTemp++;
        bresenhamCirclePositionsTemp++;
    }

    U score = 0;
    bresenhamCircleTemp = bresenhamCircle;
    for (int i = 0; i < 16; i++)
    {
        score += RPPABS(centerPixel - *bresenhamCircleTemp);
        bresenhamCircleTemp++;
    }

    *dstPtrPixel = score;
    
    return RPP_SUCCESS;
}




















// Convolution Functions

template<typename T, typename U>
RppStatus convolve_image_host(T* srcPtrMod, RppiSize srcSizeMod, U* dstPtr, RppiSize srcSize, 
                        Rpp32f* kernel, RppiSize kernelSize, 
                        RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrWindow;
    U *dstPtrTemp;
    srcPtrWindow = srcPtrMod;
    dstPtrTemp = dstPtr;

    U maxVal = (U)(std::numeric_limits<U>::max());
    U minVal = (U)(std::numeric_limits<U>::min());

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        Rpp32u remainingElementsInRow = srcSizeMod.width - kernelSize.width;
        Rpp32u windowRowIncrement = kernelSize.width - 1;

        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    convolution_kernel_host(srcPtrWindow, dstPtrTemp, srcSize, 
                                                 kernel, kernelSize, remainingElementsInRow, maxVal, minVal, 
                                                 chnFormat, channel);
                    srcPtrWindow++;
                    dstPtrTemp++;
                }
                srcPtrWindow += windowRowIncrement;
            }
            srcPtrWindow += ((kernelSize.height - 1) * srcSizeMod.width);
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        Rpp32u remainingElementsInRow = (srcSizeMod.width - kernelSize.width) * channel;
        Rpp32u windowRowIncrement = (kernelSize.width - 1) * channel;
        
        for (int i = 0; i < srcSize.height; i++)
        {
            for (int j = 0; j < srcSize.width; j++)
            {
                for (int c = 0; c < channel; c++)
                {   
                    convolution_kernel_host(srcPtrWindow, dstPtrTemp, srcSize, 
                                                 kernel, kernelSize, remainingElementsInRow, maxVal, minVal, 
                                                 chnFormat, channel);
                    srcPtrWindow++;
                    dstPtrTemp++;
                }
            }
            srcPtrWindow += windowRowIncrement;
        }
    }
    
    return RPP_SUCCESS;
}

template<typename T>
RppStatus convolve_subimage_host(T* srcPtrMod, RppiSize srcSizeMod, T* dstPtr, RppiSize srcSizeSubImage, RppiSize srcSize, 
                        Rpp32f* kernel, RppiSize kernelSize, 
                        RppiChnFormat chnFormat, Rpp32u channel)
{
    int widthDiffPlanar = srcSize.width - srcSizeSubImage.width;
    int widthDiffPacked = (srcSize.width - srcSizeSubImage.width) * channel;

    T *srcPtrWindow, *dstPtrTemp;

    T maxVal = (T)(std::numeric_limits<T>::max());
    T minVal = (T)(std::numeric_limits<T>::min());
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        Rpp32u remainingElementsInRow = srcSize.width - kernelSize.width;

        for (int c = 0; c < channel; c++)
        {
            srcPtrWindow = srcPtrMod + (c * srcSize.height * srcSize.width);
            dstPtrTemp = dstPtr + (c * srcSize.height * srcSize.width);
            for (int i = 0; i < srcSizeSubImage.height; i++)
            {
                for (int j = 0; j < srcSizeSubImage.width; j++)
                {
                    convolution_kernel_host(srcPtrWindow, dstPtrTemp, srcSize, 
                                                 kernel, kernelSize, remainingElementsInRow, maxVal, minVal, 
                                                 chnFormat, channel);
                    srcPtrWindow++;
                    dstPtrTemp++;
                }
                srcPtrWindow += widthDiffPlanar;
                dstPtrTemp += widthDiffPlanar;
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        Rpp32u remainingElementsInRow = (srcSize.width - kernelSize.width) * channel;

        srcPtrWindow = srcPtrMod;
        dstPtrTemp = dstPtr;
        for (int i = 0; i < srcSizeSubImage.height; i++)
        {
            for (int j = 0; j < srcSizeSubImage.width; j++)
            {
                for (int c = 0; c < channel; c++)
                {   
                    convolution_kernel_host(srcPtrWindow, dstPtrTemp, srcSize, 
                                                 kernel, kernelSize, remainingElementsInRow, maxVal, minVal, 
                                                 chnFormat, channel);
                    srcPtrWindow++;
                    dstPtrTemp++;
                }
            }
            srcPtrWindow += widthDiffPacked;
            dstPtrTemp += widthDiffPacked;
        }
    }
    
    return RPP_SUCCESS;
}
















// Compute Functions

template<typename T>
RppStatus compute_subimage_location_host(T* ptr, T** ptrSubImage, 
                                         RppiSize size, RppiSize *sizeSubImage, 
                                         Rpp32u x1, Rpp32u y1, Rpp32u x2, Rpp32u y2, 
                                         RppiChnFormat chnFormat, Rpp32u channel)
{
    if ((RPPINRANGE(x1, 0, size.width - 1) == 0) 
        || (RPPINRANGE(x2, 0, size.width - 1) == 0) 
        || (RPPINRANGE(y1, 0, size.height - 1) == 0) 
        || (RPPINRANGE(y2, 0, size.height - 1) == 0))
    {
        return RPP_ERROR;
    }
    
    int yDiff = (int) y2 - (int) y1;
    int xDiff = (int) x2 - (int) x1;

    sizeSubImage->height = (Rpp32u) RPPABS(yDiff) + 1;
    sizeSubImage->width = (Rpp32u) RPPABS(xDiff) + 1;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        *ptrSubImage = ptr + (RPPMIN2(y1, y2) * size.width) + RPPMIN2(x1, x2);
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        *ptrSubImage = ptr + (RPPMIN2(y1, y2) * size.width * channel) + (RPPMIN2(x1, x2) * channel);
    }

    return RPP_SUCCESS;
}

template<typename T>
RppStatus compute_transpose_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize, 
                                 RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            srcPtrTemp = srcPtr + (c * srcSize.height * srcSize.width);
            for (int i = 0; i < dstSize.height; i++)
            {
                for (int j = 0; j < dstSize.width; j++)
                {
                    *dstPtrTemp = *(srcPtrTemp + (j * srcSize.width) + i);
                    dstPtrTemp++;
                }
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int i = 0; i < dstSize.height; i++)
        {
            for (int j = 0; j < dstSize.width; j++)
            {
                srcPtrTemp = srcPtr + (channel * ((j * srcSize.width) + i));
                for (int c = 0; c < channel; c++)
                {
                    *dstPtrTemp = *(srcPtrTemp + c);
                    dstPtrTemp++;
                }
            }
        }
    }

    return RPP_SUCCESS;
}

template <typename T, typename U>
RppStatus compute_subtract_host(T* srcPtr1, U* srcPtr2, RppiSize srcSize, T* dstPtr,
                        Rpp32u channel)
{
    T *srcPtr1Temp, *dstPtrTemp;
    U *srcPtr2Temp;
    srcPtr1Temp = srcPtr1;
    srcPtr2Temp = srcPtr2;
    dstPtrTemp = dstPtr;

    Rpp32s pixel;

    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        pixel = ((Rpp32s) (*srcPtr1Temp)) - ((Rpp32s) (*srcPtr2Temp));
        pixel = RPPPIXELCHECK(pixel);
        *dstPtrTemp =(T) pixel;
        srcPtr1Temp++;
        srcPtr2Temp++;
        dstPtrTemp++;
    }

    return RPP_SUCCESS;

}

template <typename T, typename U>
RppStatus compute_multiply_host(T* srcPtr1, U* srcPtr2, RppiSize srcSize, T* dstPtr,
                                   Rpp32u channel)
{
    T *srcPtr1Temp, *dstPtrTemp;
    U *srcPtr2Temp;
    srcPtr1Temp = srcPtr1;
    srcPtr2Temp = srcPtr2;
    dstPtrTemp = dstPtr;

    Rpp32f pixel;

    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        pixel = ((Rpp32f) (*srcPtr1Temp)) * ((Rpp32f) (*srcPtr2Temp));
        pixel = RPPPIXELCHECK(pixel);
        *dstPtrTemp = (T) pixel;
        srcPtr1Temp++;
        srcPtr2Temp++;
        dstPtrTemp++;
    }

    return RPP_SUCCESS;
}

template <typename T, typename U>
RppStatus compute_rgb_to_hsv_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTempR, *srcPtrTempG, *srcPtrTempB;
    U *dstPtrTempH, *dstPtrTempS, *dstPtrTempV;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        Rpp32u imageDim = srcSize.height * srcSize.width;
        srcPtrTempR = srcPtr;
        srcPtrTempG = srcPtr + (imageDim);
        srcPtrTempB = srcPtr + (2 * imageDim);
        dstPtrTempH = dstPtr;
        dstPtrTempS = dstPtr + (imageDim);
        dstPtrTempV = dstPtr + (2 * imageDim);

        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            Rpp32f rf, gf, bf, cmax, cmin, delta;
            rf = ((Rpp32f) *srcPtrTempR) / 255;
            gf = ((Rpp32f) *srcPtrTempG) / 255;
            bf = ((Rpp32f) *srcPtrTempB) / 255;
            cmax = RPPMAX3(rf, gf, bf);
            cmin = RPPMIN3(rf, gf, bf);
            delta = cmax - cmin;

            if (delta == 0)
            {
                *dstPtrTempH = 0;
            }
            else if (cmax == rf)
            {
                *dstPtrTempH = round(60 * fmod(((gf - bf) / delta),6));
            }
            else if (cmax == gf)
            {
                *dstPtrTempH = round(60 * (((bf - rf) / delta) + 2));
            }
            else if (cmax == bf)
            {
                *dstPtrTempH = round(60 * (((rf - gf) / delta) + 4));
            }
            
            while (*dstPtrTempH > 360)
            {
                *dstPtrTempH = *dstPtrTempH - 360;
            }
            while (*dstPtrTempH < 0)
            {
                *dstPtrTempH = 360 + *dstPtrTempH;
            }

            if (cmax == 0)
            {
                *dstPtrTempS = 0;
            }
            else
            {
                *dstPtrTempS = delta / cmax;
            }

            *dstPtrTempV = cmax;
            
            srcPtrTempR++;
            srcPtrTempG++;
            srcPtrTempB++;
            dstPtrTempH++;
            dstPtrTempS++;
            dstPtrTempV++;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        srcPtrTempR = srcPtr;
        srcPtrTempG = srcPtr + 1;
        srcPtrTempB = srcPtr + 2;
        dstPtrTempH = dstPtr;
        dstPtrTempS = dstPtr + 1;
        dstPtrTempV = dstPtr + 2;

        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            Rpp32f rf, gf, bf, cmax, cmin, delta;
            rf = ((Rpp32f) *srcPtrTempR) / 255;
            gf = ((Rpp32f) *srcPtrTempG) / 255;
            bf = ((Rpp32f) *srcPtrTempB) / 255;
            cmax = RPPMAX3(rf, gf, bf);
            cmin = RPPMIN3(rf, gf, bf);
            delta = cmax - cmin;

            if (delta == 0)
            {
                *dstPtrTempH = 0;
            }
            else if (cmax == rf)
            {
                *dstPtrTempH = round(60 * fmod(((gf - bf) / delta),6));
            }
            else if (cmax == gf)
            {
                *dstPtrTempH = round(60 * (((bf - rf) / delta) + 2));
            }
            else if (cmax == bf)
            {
                *dstPtrTempH = round(60 * (((rf - gf) / delta) + 4));
            }
            
            while (*dstPtrTempH > 360)
            {
                *dstPtrTempH = *dstPtrTempH - 360;
            }
            while (*dstPtrTempH < 0)
            {
                *dstPtrTempH = 360 + *dstPtrTempH;
            }

            if (cmax == 0)
            {
                *dstPtrTempS = 0;
            }
            else
            {
                *dstPtrTempS = delta / cmax;
            }

            *dstPtrTempV = cmax;

            srcPtrTempR += 3;
            srcPtrTempG += 3;
            srcPtrTempB += 3;
            dstPtrTempH += 3;
            dstPtrTempS += 3;
            dstPtrTempV += 3;
        }
    }
    
    return RPP_SUCCESS;
}

template <typename T, typename U>
RppStatus compute_hsv_to_rgb_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTempH, *srcPtrTempS, *srcPtrTempV;
    U *dstPtrTempR, *dstPtrTempG, *dstPtrTempB;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        Rpp32u imageDim = srcSize.height * srcSize.width;
        srcPtrTempH = srcPtr;
        srcPtrTempS = srcPtr + (imageDim);
        srcPtrTempV = srcPtr + (2 * imageDim);
        dstPtrTempR = dstPtr;
        dstPtrTempG = dstPtr + (imageDim);
        dstPtrTempB = dstPtr + (2 * imageDim);

        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            Rpp32f c, x, m, rf, gf, bf;
            c = *srcPtrTempV * *srcPtrTempS;
            x = c * (1 - abs((fmod((*srcPtrTempH / 60), 2)) - 1));
            m = *srcPtrTempV - c;
            
            if ((0 <= *srcPtrTempH) && (*srcPtrTempH < 60))
            {
                rf = c;
                gf = x;
                bf = 0;
            }
            else if ((60 <= *srcPtrTempH) && (*srcPtrTempH < 120))
            {
                rf = x;
                gf = c;
                bf = 0;
            }
            else if ((120 <= *srcPtrTempH) && (*srcPtrTempH < 180))
            {
                rf = 0;
                gf = c;
                bf = x;
            }
            else if ((180 <= *srcPtrTempH) && (*srcPtrTempH < 240))
            {
                rf = 0;
                gf = x;
                bf = c;
            }
            else if ((240 <= *srcPtrTempH) && (*srcPtrTempH < 300))
            {
                rf = x;
                gf = 0;
                bf = c;
            }
            else if ((300 <= *srcPtrTempH) && (*srcPtrTempH < 360))
            {
                rf = c;
                gf = 0;
                bf = x;
            }

            *dstPtrTempR = (Rpp8u) round((rf + m) * 255);
            *dstPtrTempG = (Rpp8u) round((gf + m) * 255);
            *dstPtrTempB = (Rpp8u) round((bf + m) * 255);

            srcPtrTempH++;
            srcPtrTempS++;
            srcPtrTempV++;
            dstPtrTempR++;
            dstPtrTempG++;
            dstPtrTempB++;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        srcPtrTempH = srcPtr;
        srcPtrTempS = srcPtr + 1;
        srcPtrTempV = srcPtr + 2;
        dstPtrTempR = dstPtr;
        dstPtrTempG = dstPtr + 1;
        dstPtrTempB = dstPtr + 2;

        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            Rpp32f c, x, m, rf, gf, bf;
            c = *srcPtrTempV * *srcPtrTempS;
            x = c * (1 - abs((fmod((*srcPtrTempH / 60), 2)) - 1));
            m = *srcPtrTempV - c;
            
            if ((0 <= *srcPtrTempH) && (*srcPtrTempH < 60))
            {
                rf = c;
                gf = x;
                bf = 0;
            }
            else if ((60 <= *srcPtrTempH) && (*srcPtrTempH < 120))
            {
                rf = x;
                gf = c;
                bf = 0;
            }
            else if ((120 <= *srcPtrTempH) && (*srcPtrTempH < 180))
            {
                rf = 0;
                gf = c;
                bf = x;
            }
            else if ((180 <= *srcPtrTempH) && (*srcPtrTempH < 240))
            {
                rf = 0;
                gf = x;
                bf = c;
            }
            else if ((240 <= *srcPtrTempH) && (*srcPtrTempH < 300))
            {
                rf = x;
                gf = 0;
                bf = c;
            }
            else if ((300 <= *srcPtrTempH) && (*srcPtrTempH < 360))
            {
                rf = c;
                gf = 0;
                bf = x;
            }

            *dstPtrTempR = (Rpp8u) round((rf + m) * 255);
            *dstPtrTempG = (Rpp8u) round((gf + m) * 255);
            *dstPtrTempB = (Rpp8u) round((bf + m) * 255);

            srcPtrTempH += 3;
            srcPtrTempS += 3;
            srcPtrTempV += 3;
            dstPtrTempR += 3;
            dstPtrTempG += 3;
            dstPtrTempB += 3;
        }
    }

    return RPP_SUCCESS;
}

template <typename T, typename U>
RppStatus compute_rgb_to_hsl_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTempR, *srcPtrTempG, *srcPtrTempB;
    U *dstPtrTempH, *dstPtrTempS, *dstPtrTempL;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        Rpp32u imageDim = srcSize.height * srcSize.width;
        srcPtrTempR = srcPtr;
        srcPtrTempG = srcPtr + (imageDim);
        srcPtrTempB = srcPtr + (2 * imageDim);
        dstPtrTempH = dstPtr;
        dstPtrTempS = dstPtr + (imageDim);
        dstPtrTempL = dstPtr + (2 * imageDim);

        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            Rpp32f rf, gf, bf, cmax, cmin, delta, divisor;
            rf = ((Rpp32f) *srcPtrTempR) / 255;
            gf = ((Rpp32f) *srcPtrTempG) / 255;
            bf = ((Rpp32f) *srcPtrTempB) / 255;
            cmax = RPPMAX3(rf, gf, bf);
            cmin = RPPMIN3(rf, gf, bf);
            divisor = cmax + cmin - 1;
            delta = cmax - cmin;

            if (delta == 0)
            {
                *dstPtrTempH = 0;
            }
            else if (cmax == rf)
            {
                *dstPtrTempH = round(60 * fmod(((gf - bf) / delta),6));
            }
            else if (cmax == gf)
            {
                *dstPtrTempH = round(60 * (((bf - rf) / delta) + 2));
            }
            else if (cmax == bf)
            {
                *dstPtrTempH = round(60 * (((rf - gf) / delta) + 4));
            }
            
            while (*dstPtrTempH > 360)
            {
                *dstPtrTempH = *dstPtrTempH - 360;
            }
            while (*dstPtrTempH < 0)
            {
                *dstPtrTempH = 360 + *dstPtrTempH;
            }

            if (delta == 0)
            {
                *dstPtrTempS = 0;
            }
            else
            {
                *dstPtrTempS = delta / (1 - RPPABS(divisor));
            }

            *dstPtrTempL = (cmax + cmin) / 2;

            srcPtrTempR++;
            srcPtrTempG++;
            srcPtrTempB++;
            dstPtrTempH++;
            dstPtrTempS++;
            dstPtrTempL++;

        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        srcPtrTempR = srcPtr;
        srcPtrTempG = srcPtr + 1;
        srcPtrTempB = srcPtr + 2;
        dstPtrTempH = dstPtr;
        dstPtrTempS = dstPtr + 1;
        dstPtrTempL = dstPtr + 2;

        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            Rpp32f rf, gf, bf, cmax, cmin, delta, divisor;
            rf = ((Rpp32f) *srcPtrTempR) / 255;
            gf = ((Rpp32f) *srcPtrTempG) / 255;
            bf = ((Rpp32f) *srcPtrTempB) / 255;
            cmax = RPPMAX3(rf, gf, bf);
            cmin = RPPMIN3(rf, gf, bf);
            divisor = cmax + cmin - 1;
            delta = cmax - cmin;

            if (delta == 0)
            {
                *dstPtrTempH = 0;
            }
            else if (cmax == rf)
            {
                *dstPtrTempH = round(60 * fmod(((gf - bf) / delta),6));
            }
            else if (cmax == gf)
            {
                *dstPtrTempH = round(60 * (((bf - rf) / delta) + 2));
            }
            else if (cmax == bf)
            {
                *dstPtrTempH = round(60 * (((rf - gf) / delta) + 4));
            }
            
            while (*dstPtrTempH > 360)
            {
                *dstPtrTempH = *dstPtrTempH - 360;
            }
            while (*dstPtrTempH < 0)
            {
                *dstPtrTempH = 360 + *dstPtrTempH;
            }

            if (delta == 0)
            {
                *dstPtrTempS = 0;
            }
            else
            {
                *dstPtrTempS = delta / (1 - RPPABS(divisor));
            }

            *dstPtrTempL = (cmax + cmin) / 2;

            srcPtrTempR += 3;
            srcPtrTempG += 3;
            srcPtrTempB += 3;
            dstPtrTempH += 3;
            dstPtrTempS += 3;
            dstPtrTempL += 3;

        }
    }

    return RPP_SUCCESS;
}

template <typename T, typename U>
RppStatus compute_hsl_to_rgb_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTempH, *srcPtrTempS, *srcPtrTempL;
    U *dstPtrTempR, *dstPtrTempG, *dstPtrTempB;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        Rpp32u imageDim = srcSize.height * srcSize.width;
        srcPtrTempH = srcPtr;
        srcPtrTempS = srcPtr + (imageDim);
        srcPtrTempL = srcPtr + (2 * imageDim);
        dstPtrTempR = dstPtr;
        dstPtrTempG = dstPtr + (imageDim);
        dstPtrTempB = dstPtr + (2 * imageDim);

        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            Rpp32f c, x, m, rf, gf, bf;
            c = (2 * *srcPtrTempL) - 1;
            c = (1 - RPPABS(c)) * *srcPtrTempS;
            x = c * (1 - abs((fmod((*srcPtrTempH / 60), 2)) - 1));
            m = *srcPtrTempL - c / 2;
            
            if ((0 <= *srcPtrTempH) && (*srcPtrTempH < 60))
            {
                rf = c;
                gf = x;
                bf = 0;
            }
            else if ((60 <= *srcPtrTempH) && (*srcPtrTempH < 120))
            {
                rf = x;
                gf = c;
                bf = 0;
            }
            else if ((120 <= *srcPtrTempH) && (*srcPtrTempH < 180))
            {
                rf = 0;
                gf = c;
                bf = x;
            }
            else if ((180 <= *srcPtrTempH) && (*srcPtrTempH < 240))
            {
                rf = 0;
                gf = x;
                bf = c;
            }
            else if ((240 <= *srcPtrTempH) && (*srcPtrTempH < 300))
            {
                rf = x;
                gf = 0;
                bf = c;
            }
            else if ((300 <= *srcPtrTempH) && (*srcPtrTempH < 360))
            {
                rf = c;
                gf = 0;
                bf = x;
            }

            *dstPtrTempR = (Rpp8u) round((rf + m) * 255);
            *dstPtrTempG = (Rpp8u) round((gf + m) * 255);
            *dstPtrTempB = (Rpp8u) round((bf + m) * 255);

            srcPtrTempH++;
            srcPtrTempS++;
            srcPtrTempL++;
            dstPtrTempR++;
            dstPtrTempG++;
            dstPtrTempB++;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        srcPtrTempH = srcPtr;
        srcPtrTempS = srcPtr + 1;
        srcPtrTempL = srcPtr + 2;
        dstPtrTempR = dstPtr;
        dstPtrTempG = dstPtr + 1;
        dstPtrTempB = dstPtr + 2;

        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            Rpp32f c, x, m, rf, gf, bf;
            c = (2 * *srcPtrTempL) - 1;
            c = (1 - RPPABS(c)) * *srcPtrTempS;
            x = c * (1 - abs((fmod((*srcPtrTempH / 60), 2)) - 1));
            m = *srcPtrTempL - c / 2;
            
            if ((0 <= *srcPtrTempH) && (*srcPtrTempH < 60))
            {
                rf = c;
                gf = x;
                bf = 0;
            }
            else if ((60 <= *srcPtrTempH) && (*srcPtrTempH < 120))
            {
                rf = x;
                gf = c;
                bf = 0;
            }
            else if ((120 <= *srcPtrTempH) && (*srcPtrTempH < 180))
            {
                rf = 0;
                gf = c;
                bf = x;
            }
            else if ((180 <= *srcPtrTempH) && (*srcPtrTempH < 240))
            {
                rf = 0;
                gf = x;
                bf = c;
            }
            else if ((240 <= *srcPtrTempH) && (*srcPtrTempH < 300))
            {
                rf = x;
                gf = 0;
                bf = c;
            }
            else if ((300 <= *srcPtrTempH) && (*srcPtrTempH < 360))
            {
                rf = c;
                gf = 0;
                bf = x;
            }

            *dstPtrTempR = (Rpp8u) round((rf + m) * 255);
            *dstPtrTempG = (Rpp8u) round((gf + m) * 255);
            *dstPtrTempB = (Rpp8u) round((bf + m) * 255);

            srcPtrTempH += 3;
            srcPtrTempS += 3;
            srcPtrTempL += 3;
            dstPtrTempR += 3;
            dstPtrTempG += 3;
            dstPtrTempB += 3;
        }
    }

    return RPP_SUCCESS;
}

template <typename T, typename U>
RppStatus compute_magnitude_host(T* srcPtr1, T* srcPtr2, RppiSize srcSize, U* dstPtr,
                         RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtr1Temp, *srcPtr2Temp;
    U *dstPtrTemp;
    srcPtr1Temp = srcPtr1;
    srcPtr2Temp = srcPtr2;
    dstPtrTemp = dstPtr;

    Rpp32f pixel;
    Rpp32s srcPtr1Value, srcPtr2Value;
    
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        srcPtr1Value = (Rpp32s) *srcPtr1Temp;
        srcPtr2Value = (Rpp32s) *srcPtr2Temp;
        pixel = sqrt((srcPtr1Value * srcPtr1Value) + (srcPtr2Value * srcPtr2Value));
        pixel = RPPPIXELCHECK(pixel);
        *dstPtrTemp =(U) round(pixel);
        srcPtr1Temp++;
        srcPtr2Temp++;
        dstPtrTemp++;
    }

    return RPP_SUCCESS;

}

template <typename T, typename U>
RppStatus compute_threshold_host(T* srcPtr, RppiSize srcSize, U* dstPtr, 
                                 U min, U max, Rpp32u type, 
                                 RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTemp;
    U *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    if (type == 1)
    {
        for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
        {
            if (*srcPtrTemp < min)
            {
                *dstPtrTemp = (U) 0;
            }
            else if (*srcPtrTemp <= max)
            {
                *dstPtrTemp = (U) 255;
            }
            else
            {
                *dstPtrTemp = (U) 0;
            }
            
            srcPtrTemp++;
            dstPtrTemp++;
        }
    }
    else if (type == 2)
    {
        for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
        {
            if (RPPABS(*srcPtrTemp) < min)
            {
                *dstPtrTemp = (U) 0;
            }
            else if (RPPABS(*srcPtrTemp) <= max)
            {
                *dstPtrTemp = (U) 255;
            }
            else
            {
                *dstPtrTemp = (U) 0;
            }
            
            srcPtrTemp++;
            dstPtrTemp++;
        }
    }

    return RPP_SUCCESS;

}

template <typename T>
RppStatus compute_data_object_copy_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    memcpy(dstPtr, srcPtr, srcSize.height * srcSize.width * channel * sizeof(T));
    
    return RPP_SUCCESS;
}

template <typename T>
RppStatus compute_downsampled_image_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize, 
                                         RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    Rpp8u checkEven;
    checkEven = (Rpp8u) RPPISEVEN(srcSize.width);

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            srcPtrTemp = srcPtr + (c * srcSize.height * srcSize.width);

            for (int i = 0; i < dstSize.height; i++)
            {
                for (int j = 0; j < dstSize.width; j++)
                {
                    *dstPtrTemp = *srcPtrTemp;
                    srcPtrTemp += 2;
                    dstPtrTemp++;
                }
                if (checkEven == 0)
                {
                    srcPtrTemp--;
                }
                srcPtrTemp += srcSize.width;
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        Rpp32u elementsInRow = srcSize.width * channel;    
        
        for (int i = 0; i < dstSize.height; i++)
        {
            for (int j = 0; j < dstSize.width; j++)
            {
                for (int c = 0; c < channel; c++)
                {
                    *dstPtrTemp = *srcPtrTemp;
                    srcPtrTemp++;
                    dstPtrTemp++;
                }
                srcPtrTemp += channel;
            }
            if (checkEven == 0)
            {
                srcPtrTemp -= channel;
            }
            srcPtrTemp += elementsInRow;
        }
    }

    return RPP_SUCCESS;
}

#endif //RPP_CPU_COMMON_H