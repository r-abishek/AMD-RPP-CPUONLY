#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus match_template_host(T* srcPtr, RppiSize srcSize, U* dstPtr, 
                                  T *templateImage, RppiSize templateImageSize, 
                                  RppiChnFormat chnFormat, Rpp32u channel)
{
    // RGB to Greyscale Conversion for Image

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

    // RGB to Greyscale Conversion for Template

    Rpp32u templateImageDim = templateImageSize.height * templateImageSize.width;

    T *templateImageGreyscale = (T *)calloc(templateImageDim, sizeof(T));
    T *templateImageGreyscaleTemp;
    templateImageGreyscaleTemp = templateImageGreyscale;

    if (channel == 3)
    {
        if (chnFormat == RPPI_CHN_PLANAR)
        {
            T *templateImageTempR, *templateImageTempG, *templateImageTempB;
            templateImageTempR = templateImage;
            templateImageTempG = templateImage + templateImageDim;
            templateImageTempB = templateImageTempG + templateImageDim;

            for (int i = 0; i < templateImageDim; i++)
            {
                *templateImageGreyscaleTemp = (T) (((Rpp32u)(*templateImageTempR) + (Rpp32u)(*templateImageTempG) + (Rpp32u)(*templateImageTempB)) / 3);
                templateImageGreyscaleTemp++;
                templateImageTempR++;
                templateImageTempG++;
                templateImageTempB++;
            }
        }
        else if (chnFormat == RPPI_CHN_PACKED)
        {
            T *templateImageTempR, *templateImageTempG, *templateImageTempB;
            templateImageTempR = templateImage;
            templateImageTempG = templateImage + 1;
            templateImageTempB = templateImageTempG + 1;
            
            for (int i = 0; i < templateImageDim; i++)
            {
                *templateImageGreyscaleTemp = (T) (((Rpp32u)(*templateImageTempR) + (Rpp32u)(*templateImageTempG) + (Rpp32u)(*templateImageTempB)) / 3);
                templateImageGreyscaleTemp++;
                templateImageTempR += channel;
                templateImageTempG += channel;
                templateImageTempB += channel;
            }
        }
    }
    else if (channel == 1)
    {
        memcpy(templateImageGreyscale, templateImage, templateImageDim * sizeof(T));
    }

    // Template Matching
    
    U *dstPtrTemp;
    dstPtrTemp = dstPtr;
    U peakValue = (U) 65535;
    //for (int i = 0; i < dstSize.height * dstSize.width; i++)
    for (int i = 0; i < srcSize.height * srcSize.width; i++)
    {
        *dstPtrTemp = peakValue;
        dstPtrTemp++;
    }
    
    T *srcPtrWindow;
    U *dstPtrWindow;
    srcPtrWindow = srcPtrGreyscale;
    dstPtrWindow = dstPtr;
    
    Rpp32u rowIter = srcSize.height - templateImageSize.height + 1;
    //rowIter = RPPMIN2(rowIter, dstSize.height);
    rowIter = RPPMIN2(rowIter, srcSize.height);
    Rpp32u colIter = srcSize.width - templateImageSize.width + 1;
    //colIter = RPPMIN2(colIter, dstSize.width);
    colIter = RPPMIN2(colIter, srcSize.width);

    //Rpp32u widthDiff = RPPABS(srcSize.width - dstSize.width);
    Rpp32u remainingElementsInRow = srcSize.width - templateImageSize.width;
    
    for (int i = 0; i < rowIter; i++)
    {
        for (int j = 0; j < colIter; j++)
        {
            match_template_kernel_host(srcPtrWindow, dstPtrWindow, 
                                        templateImageGreyscale, templateImageSize, remainingElementsInRow);
            dstPtrWindow++;
            srcPtrWindow++;
        }
        srcPtrWindow += (templateImageSize.width - 1);
        //dstPtrWindow += (templateImageSize.width - 1 - widthDiff);
        dstPtrWindow += (templateImageSize.width - 1);
    }
    
    return RPP_SUCCESS;
}
