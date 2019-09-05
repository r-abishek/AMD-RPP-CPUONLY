#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus match_template_host(T* srcPtr, RppiSize srcSize, U* dstPtr, RppiSize dstSize, 
                                  T *templateImage, RppiSize templateImageSize, 
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
    
    T *srcPtrWindow;
    U *dstPtrWindow;
    srcPtrWindow = srcPtrGreyscale;
    dstPtrWindow = dstPtr;
    
    Rpp32u rowIter = srcSize.height - templateImageSize.height + 1;
    rowIter = RPPMIN2(rowIter, dstSize.height);
    Rpp32u colIter = srcSize.width - templateImageSize.width + 1;
    Rpp32u colIterDiff = 0;
    if (dstSize.width < colIter)
    {
        colIterDiff = RPPABS(colIter - dstSize.width);

    }
    //Rpp32u colIterDiff = RPPABS(colIter - dstSize.width);
    colIter = RPPMIN2(colIter, dstSize.width);

    printf("\nrowIter = %d, colIter = %d", rowIter, colIter);

    U *dstPtrTemp;
    dstPtrTemp = dstPtr;
    for (int i = 0; i < dstSize.height * dstSize.width; i++)
    {
        *dstPtrTemp = -1;
        dstPtrTemp++;
    }
    //memset(dstPtr, -1, dstSize.height * dstSize.width * sizeof(U));

    Rpp32u widthDiff = RPPABS(srcSize.width - dstSize.width);
    Rpp32u remainingElementsInRow = srcSize.width - templateImageSize.width;
    
    for (int i = 0; i < rowIter; i++)
    {
        for (int j = 0; j < colIter; j++)
        {
            match_template_kernel_host(srcPtrWindow, dstPtrWindow, 
                                        templateImage, templateImageSize, remainingElementsInRow);
            //*dstPtrTemp = *(srcPtrTemp + (*rowRemapTableTemp * srcSize.width) + *colRemapTableTemp);
            dstPtrWindow++;
            srcPtrWindow++;
        }
        srcPtrWindow += (templateImageSize.width - 1);
        dstPtrWindow += (templateImageSize.width - 1 - widthDiff + colIterDiff);
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    //if (chnFormat == RPPI_CHN_PLANAR)
    //{
    //    printf("\nEnter PLANAR");
    //    dstPtrWindow = dstPtr;
    //    srcPtrWindow = srcPtr;
    //    Rpp32u remainingElementsInRow = srcSize.width * channel;
    //    Rpp32u increment = (templateImageSize.width - 1) + ((templateImageSize.height - 1) * srcSize.width);
    //    
    //    //templateImageTemp = templateImage;
    //    for (int c = 0; c < channel; c++)
    //    {
    //        printf("\nEnter channel");
    //        for (int i = 0; i < rowIter; i++)
    //        {
    //            for (int j = 0; j < colIter; j++)
    //            {
    //                match_template_kernel_host(srcPtrWindow, dstPtrWindow, srcSize, 
    //                                           templateImage, templateImageSize, remainingElementsInRow, 
    //                                           chnFormat, channel);
    //                dstPtrWindow++;
    //                srcPtrWindow++;
    //            }
    //            srcPtrWindow += (templateImageSize.width - 1);
    //            dstPtrWindow += (templateImageSize.width - 1);
    //        }
    //        srcPtrWindow += increment;
    //        dstPtrWindow += increment;
    //    }
    //}
    //else if (chnFormat == RPPI_CHN_PACKED)
    //{
    //    dstPtrTemp = dstPtr;
    //    rowRemapTableTemp = rowRemapTable;
    //    colRemapTableTemp = colRemapTable;
    //    Rpp32u elementsInRow = srcSize.width * channel;
    //    for (int i = 0; i < srcSize.height; i++)
    //    {
    //        for (int j = 0; j < srcSize.width; j++)
    //        {
    //            srcPtrTemp = srcPtr;
    //            for (int c = 0; c < channel; c++)
    //            {
    //                *dstPtrTemp = *(srcPtrTemp + (*rowRemapTableTemp * elementsInRow) + (*colRemapTableTemp * channel));
    //                dstPtrTemp++;
    //                srcPtrTemp++;
    //            }
    //            rowRemapTableTemp++;
    //            colRemapTableTemp++;
    //        }
    //    }
    //}
    
    return RPP_SUCCESS;
}
