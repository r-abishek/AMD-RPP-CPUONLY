#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus resize_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           RppiChnFormat chnFormat, unsigned int channel)
{
    if (dstSize.height < 0 || dstSize.width < 0)
    {
        return RPP_ERROR;
    }

// Raj - Pointers Version - More efficient with if-statement for last row/column (more optimized inner loop) - NEW TRIAL
/*
    printf("\nRaj - Pointers Version - More efficient with if-statement for last row/column (more optimized inner loop) - NEW TRIAL");
    Rpp32f hRatio = (((Rpp32f) dstSize.height) / ((Rpp32f) srcSize.height));
    Rpp32f wRatio = (((Rpp32f) dstSize.width) / ((Rpp32f) srcSize.width));
    Rpp32f srcLocationRow, srcLocationColumn, pixel;
    Rpp32s srcLocationRowFloor, srcLocationColumnFloor;
    Rpp32s srcLocationRowFloorPrevious = -1, srcLocationColumnFloorPrevious = -1;
    T *srcPtrTemp, *dstPtrTemp, *srcPtrTopRow, *srcPtrBottomRow;
    T *srcPtrFirst, *srcPtrSecond, *srcPtrThird, *srcPtrFourth;
    Rpp32f wFirst, wSecond, wThird, wFourth;
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
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
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

            srcLocationColumnFloorPrevious = -1;
            Rpp32s srcLocColFloorChanneled;
            for (int j = 0; j < dstSize.width; j++)
            {   
                srcLocationColumn = ((Rpp32f) j) / wRatio;
                srcLocationColumnFloor = (Rpp32s) RPPFLOOR(srcLocationColumn);
                Rpp32f weightedWidth = srcLocationColumn - srcLocationColumnFloor;

                if (srcLocationColumnFloor > (srcSize.width - 2))
                {
                    srcLocationColumnFloor = srcSize.width - 2;
                }

                if (srcLocationColumnFloorPrevious != srcLocationColumnFloor){
                    srcLocColFloorChanneled = channel * srcLocationColumnFloor;
                    srcPtrFirst = srcPtrTopRow + srcLocColFloorChanneled;
                    //srcPtrSecond = srcPtrTopRow + srcLocColFloorChanneled + channel;
                    srcPtrThird = srcPtrBottomRow + srcLocColFloorChanneled;
                    //srcPtrFourth = srcPtrBottomRow + srcLocColFloorChanneled + channel;
                    srcLocationColumnFloorPrevious = srcLocationColumnFloor;
                }
                
                wFirst = (1 - weightedHeight) * (1 - weightedWidth);
                wSecond = (1 - weightedHeight) * (weightedWidth);
                wThird = (weightedHeight) * (1 - weightedWidth);
                wFourth = (weightedHeight) * (weightedWidth);
                
                for (int c = 0; c < channel; c++)
                {
                    
                    pixel = *(srcPtrFirst) * wFirst 
                            + *(srcPtrFirst + channel) * wSecond  
                            + *(srcPtrThird ) * wThird  
                            + *(srcPtrThird + channel) * wFourth;  

                    srcPtrFirst++; 
                    //srcPtrSecond++; 
                    srcPtrThird++; 
                    //srcPtrFourth++;
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
                    dstPtrTemp ++;
                }
            }
        }
    }
*/
















// Raj - Pointers Version - More efficient with if-statement for last row/column (more optimized inner loop)
/*
    printf("\nRaj - Pointers Version - More efficient with if-statement for last row/column (more optimized inner loop)");
    Rpp32f hRatio = (((Rpp32f) dstSize.height) / ((Rpp32f) srcSize.height));
    Rpp32f wRatio = (((Rpp32f) dstSize.width) / ((Rpp32f) srcSize.width));
    Rpp32f srcLocationRow, srcLocationColumn, pixel;
    Rpp32s srcLocationRowFloor, srcLocationColumnFloor;
    T *srcPtrTemp, *dstPtrTemp, *srcPtrTopRow, *srcPtrBottomRow;
    T *srcPtrFirst, *srcPtrSecond, *srcPtrThird, *srcPtrFourth;
    Rpp32f wFirst, wSecond, wThird, wFourth;
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
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
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
                srcPtrFirst = srcPtrTopRow + srcLocColFloorChanneled;
                srcPtrSecond = srcPtrTopRow + srcLocColFloorChanneled + channel;
                srcPtrThird = srcPtrBottomRow + srcLocColFloorChanneled;
                srcPtrFourth = srcPtrBottomRow + srcLocColFloorChanneled + channel;
                wFirst = (1 - weightedHeight) * (1 - weightedWidth);
                wSecond = (1 - weightedHeight) * (weightedWidth);
                wThird = (weightedHeight) * (1 - weightedWidth);
                wFourth = (weightedHeight) * (weightedWidth);
                
                for (int c = 0; c < channel; c++)
                {
                    
                    //pixel = ((*(srcPtrTopRow + c + srcLocColFloorChanneled)) * (1 - weightedHeight) * (1 - weightedWidth)) 
                    //        + ((*(srcPtrTopRow + c + srcLocColFloorChanneled + channel)) * (1 - weightedHeight) * (weightedWidth)) 
                    //        + ((*(srcPtrBottomRow + c + srcLocColFloorChanneled)) * (weightedHeight) * (1 - weightedWidth)) 
                    //        + ((*(srcPtrBottomRow + c + srcLocColFloorChanneled + channel)) * (weightedHeight) * (weightedWidth));
                    pixel = *srcPtrFirst * wFirst 
                            + *srcPtrSecond * wSecond  
                            + *srcPtrThird * wThird  
                            + *srcPtrFourth * wFourth;  

                    srcPtrFirst++; srcPtrSecond++; srcPtrThird++; srcPtrFourth++;
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
                    dstPtrTemp ++;
                }
            }
        }
    }
*/


















// Abishek - Pointers Version - More efficient with if-statement for last row/column

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
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
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
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
                    dstPtrTemp ++;
                }
            }
        }
    }


















// Abishek - Pointers Version - More efficient with if-statement for last row/column
/*
    Rpp32f hRatio = (((Rpp32f) dstSize.height) / ((Rpp32f) srcSize.height));
    Rpp32f wRatio = (((Rpp32f) dstSize.width) / ((Rpp32f) srcSize.width));
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
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
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
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
                    dstPtrTemp ++;
                }
            }
        }
    }
*/













// Abishek - Pointers Version - Inefficient
/*
    Rpp32f hRatio = (((Rpp32f) dstSize.height) / ((Rpp32f) srcSize.height));
    Rpp32f wRatio = (((Rpp32f) dstSize.width) / ((Rpp32f) srcSize.width));
    Rpp32f srcLocationRow, srcLocationColumn, pixel;
    T *dstPtrTemp, *srcPtrTopRow, *srcPtrBottomRow;
    dstPtrTemp = dstPtr;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < dstSize.height; i++)
            {   
                srcLocationRow = ((Rpp32f) i) / hRatio;
                Rpp32f weightedHeight = srcLocationRow - RPPFLOOR(srcLocationRow);
                
                for (int j = 0; j < dstSize.width; j++)
                {
                    srcLocationColumn = ((Rpp32f) j) / wRatio;
                    Rpp32f weightedWidth = srcLocationColumn - RPPFLOOR(srcLocationColumn);

                    if ((Rpp32s) RPPFLOOR(srcLocationRow) > (srcSize.height - 2))
                    {
                        srcLocationRow = srcSize.height - 2;
                    }
                    if ((Rpp32s) RPPFLOOR(srcLocationColumn) > (srcSize.width - 2))
                    {
                        srcLocationColumn = srcSize.width - 2;
                    }

                    srcPtrTopRow = srcPtr + (c * srcSize.height * srcSize.width) 
                                            + ((Rpp32s) RPPFLOOR(srcLocationRow) * srcSize.width) 
                                            + (Rpp32s) RPPFLOOR(srcLocationColumn);
                    srcPtrBottomRow = srcPtr + (c * srcSize.height * srcSize.width) 
                                                + ((Rpp32s) RPPFLOOR(srcLocationRow + 1) * srcSize.width) 
                                                + (Rpp32s) RPPFLOOR(srcLocationColumn);

                    pixel = ((*srcPtrTopRow) * (1 - weightedWidth) * (1 - weightedHeight)) 
                            + ((*(srcPtrTopRow + 1)) * (weightedWidth) * (1 - weightedHeight)) 
                            + ((*srcPtrBottomRow) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrBottomRow + 1)) * (weightedWidth) * (weightedHeight));
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
                    dstPtrTemp ++;
                }
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int i = 0; i < dstSize.height; i++)
        {
            srcLocationRow = ((Rpp32f) i) / hRatio;
            Rpp32f weightedHeight = srcLocationRow - RPPFLOOR(srcLocationRow);

            for (int j = 0; j < dstSize.width; j++)
            {   
                for (int c = 0; c < channel; c++)
                {
                    srcLocationColumn = ((Rpp32f) j) / wRatio;
                    Rpp32f weightedWidth = srcLocationColumn - RPPFLOOR(srcLocationColumn);

                    if ((Rpp32s) RPPFLOOR(srcLocationRow) > (srcSize.height - 2))
                    {
                        srcLocationRow = srcSize.height - 2;
                    }
                    if ((Rpp32s) RPPFLOOR(srcLocationColumn) > (srcSize.width - 2))
                    {
                        srcLocationColumn = srcSize.width - 2;
                    }

                    srcPtrTopRow = srcPtr + c + channel * (((Rpp32s) RPPFLOOR(srcLocationRow) * srcSize.width) 
                                                                    + (Rpp32s) RPPFLOOR(srcLocationColumn));
                    srcPtrBottomRow = srcPtr + c + channel * (((Rpp32s) RPPFLOOR(srcLocationRow + 1) * srcSize.width) 
                                                                        + (Rpp32s) RPPFLOOR(srcLocationColumn));
                    
                    pixel = ((*srcPtrTopRow) * (1 - weightedWidth) * (1 - weightedHeight)) 
                            + ((*(srcPtrTopRow + channel)) * (weightedWidth) * (1 - weightedHeight)) 
                            + ((*srcPtrBottomRow) * (weightedHeight) * (1 - weightedWidth)) 
                            + ((*(srcPtrBottomRow + channel)) * (weightedWidth) * (weightedHeight));
                    
                    *dstPtrTemp = (Rpp8u) round(pixel);
                    dstPtrTemp ++;
                }
            }
        }
    }
*/






















// Raj Version
/*
    Rpp32f hRatio = (((Rpp32f) dstSize.height) / ((Rpp32f) srcSize.height));
    Rpp32f wRatio = (((Rpp32f) dstSize.width) / ((Rpp32f) srcSize.width));
    Rpp32f srcLocI, srcLocJ;

    T *srcPtrTemp, *dstPtrTemp;

    Rpp32f pixel;
    T *srcPtrTop, *dstPtrBottom;

    if (chnFormat == RPPI_CHN_PLANAR)

    {
        srcPtrTemp = srcPtr;
        dstPtrTemp = dstPtr;
        for (int c = 0; c < channel; c++)
        {
            srcPtrTemp += srcSize.height * srcSize.width;

            dstPtrTemp += dstSize.height * dstSize.width;

            srcLocI = 0;

 

            for (unsigned int i = 0; i < dstSize.height - 1; i++)

            {

                Rpp32s locSrcHeight = (Rpp32s) RPPFLOOR(srcLocI);

                Rpp32f w_height = srcLocI - (Rpp32f) topLocSrc;

 

                srcPtrTop = srcPtrTemp + locSrcHeight * srcSize.width;

                srcPtrBottom = srcPtrTop + srcSize.width;

 

                srcLocJ = 0;

                for (unsigned int j = 0; j < dstSize.width - 1; j++)

                {

                    Rpp32s locSrcWidth = (Rpp32s) RPPFLOOR(srcLocJ);

                    Rpp32f w_width = srcLocJ - (Rpp32f) locSrcWidth;

 

                    pixel = ((Rpp32f) srcPtrTop[locSrcWidth]) * (1 - w_width) * (1 - w_height))

                        + (((Rpp32f) srcPtrTop[locSrcWidth + 1]) * (w_width) * (1 - w_height))

                        + (((Rpp32f) srcPtrBottom[locSrcWidth]) * (1 - w_width) * (w_height))

                        + (((Rpp32f) srcPtrBottom[locSrcWidth + 1]) * (w_width) * (w_height));

                    *dstPtrTemp = (Rpp8u) round(pixel);

                    dstPtrTemp++;


                    srcLocJ += wRatio;

                }

                srcLocI += hRatio;

            }

            for (unsigned int i = 0; i < dstSize.height - 1; i++)

            {

                Rpp32s locSrcHeight = (Rpp32s) RPPFLOOR(((Rpp32f) i) / hRatio);

                dstPtr[(c * dstSize.height * dstSize.width) + (i * dstSize.width) + dstSize.width - 1]

                    = srcPtr[(c * srcSize.height * srcSize.width) + (locSrcHeight * srcSize.width) + srcSize.width - 1]

            }

            for (unsigned int i = 0; i < dstSize.width - 1; i++)

            {

                Rpp32s locSrcWidth = (Rpp32s) RPPFLOOR(((Rpp32f) i) / wRatio);

                dstPtr[(c * dstSize.height * dstSize.width) + ( (dstSize.height - 1) * dstSize.width) + i]

                    = srcPtr[(c * srcSize.height * srcSize.width) + ( (srcSize.height - 1) * srcSize.width) + locSrcWidth]

            }

 

        }

    }
*/















// Abishek oldest version
/*
    Rpp32f resize[6] = {0};
    resize[0] = (((Rpp32f) dstSize.height) / ((Rpp32f) srcSize.height));
    resize[1] = 0;
    resize[2] = 0;
    resize[3] = 0;
    resize[4] = (((Rpp32f) dstSize.width) / ((Rpp32f) srcSize.width));
    resize[5] = 0;

    float minX = 0, minY = 0;
    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            Rpp32f newi = 0, newj = 0;
            newi = (resize[0] * i) + (resize[1] * j) + (resize[2] * 1);
            newj = (resize[3] * i) + (resize[4] * j) + (resize[5] * 1);
            if (newi < minX)
            {
                minX = newi;
            }
            if (newj < minY)
            {
                minY = newj;
            }
        }
    }

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    int k = (Rpp32s)((resize[0] * i) + (resize[1] * j) + (resize[2] * 1));
                    int l = (Rpp32s)((resize[3] * i) + (resize[4] * j) + (resize[5] * 1));
                    k -= (Rpp32s)minX;
                    l -= (Rpp32s)minY;
                    dstPtr[(c * dstSize.height * dstSize.width) + (k * dstSize.width) + l] = srcPtr[(c * srcSize.height * srcSize.width) + (i * srcSize.width) + j];
                }
            }
        }

        float w = ((float)dstSize.width - (float)srcSize.width) / ((float) (srcSize.width - 1));
        float h = ((float)dstSize.height - (float)srcSize.height) / ((float) (srcSize.height - 1));
        if (w <= 0)
        {
            w = -1;
        }
        if (h <= 0)
        {
            h = -1;
        }

        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height - 1; i++)
            {
                for (int j = 0; j < srcSize.width - 1; j++)
                {                    
                    int k = (Rpp32s)((resize[0] * i) + (resize[1] * j) + (resize[2] * 1));
                    int l = (Rpp32s)((resize[3] * i) + (resize[4] * j) + (resize[5] * 1));
                    k -= (Rpp32s)minX;
                    l -= (Rpp32s)minY;
                    
                    int pixel1 = (int) srcPtr[(c * srcSize.height * srcSize.width) + (i * srcSize.width) + j];
                    int pixel2 = (int) srcPtr[(c * srcSize.height * srcSize.width) + (i * srcSize.width) + (j + 1)];
                    int pixel3 = (int) srcPtr[(c * srcSize.height * srcSize.width) + ((i + 1) * srcSize.width) + j];
                    int pixel4 = (int) srcPtr[(c * srcSize.height * srcSize.width) + ((i + 1) * srcSize.width) + (j + 1)];
                    
                    for (float m = 0; m < h + 2; m++)
                    {
                        for (float n = 0; n < w + 2; n++)
                        {
                            Rpp32f pixel;
                            pixel = ((pixel1) * (1 - n) * (1 - m)) + ((pixel2) * (n) * (1 - m)) + ((pixel3) * (m) * (1 - n)) + ((pixel4) * (m) * (n));
                            pixel = (pixel < (Rpp32f) 255) ? pixel : ((Rpp32f) 255);
                            pixel = (pixel > (Rpp32f) 0) ? pixel : ((Rpp32f) 0);
                            dstPtr[(c * dstSize.height * dstSize.width) + ((int)(k + m) * dstSize.width) + (int)(l + n)] = (Rpp8u) round(pixel);
                        }
                    }
                }
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    int k = (Rpp32s)((resize[0] * i) + (resize[1] * j) + (resize[2] * 1));
                    int l = (Rpp32s)((resize[3] * i) + (resize[4] * j) + (resize[5] * 1));
                    k -= (Rpp32s)minX;
                    l -= (Rpp32s)minY;
                    dstPtr[c + (channel * k * dstSize.width) + (channel * l)] = srcPtr[c + (channel * i * srcSize.width) + (channel * j)];
                }
            }
        }

        float w = ((float)dstSize.width - (float)srcSize.width) / ((float) (srcSize.width - 1));
        float h = ((float)dstSize.height - (float)srcSize.height) / ((float) (srcSize.height - 1));
        if (w <= 0)
        {
            w = -1;
        }
        if (h <= 0)
        {
            h = -1;
        }

        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height - 1; i++)
            {
                for (int j = 0; j < srcSize.width - 1; j++)
                {                    
                    int k = (Rpp32s)((resize[0] * i) + (resize[1] * j) + (resize[2] * 1));
                    int l = (Rpp32s)((resize[3] * i) + (resize[4] * j) + (resize[5] * 1));
                    k -= (Rpp32s)minX;
                    l -= (Rpp32s)minY;

                    int pixel1 = (int) srcPtr[c + (channel * i * srcSize.width) + (channel * j)];
                    int pixel2 = (int) srcPtr[c + (channel * i * srcSize.width) + (channel * (j + 1))];
                    int pixel3 = (int) srcPtr[c + (channel * (i + 1) * srcSize.width) + (channel * j)];
                    int pixel4 = (int) srcPtr[c + (channel * (i + 1) * srcSize.width) + (channel * (j + 1))];
                    
                    for (float m = 0; m <= h + 1; m++)
                    {
                        for (float n = 0; n <= w + 1; n++)
                        {
                            Rpp32f pixel;
                            pixel = ((pixel1) * (1 - n) * (1 - m)) + ((pixel2) * (n) * (1 - m)) + ((pixel3) * (m) * (1 - n)) + ((pixel4) * (m) * (n));
                            pixel = (pixel < (Rpp32f) 255) ? pixel : ((Rpp32f) 255);
                            pixel = (pixel > (Rpp32f) 0) ? pixel : ((Rpp32f) 0);
                            dstPtr[c + (channel * (int)(k + m) * dstSize.width) + (channel * (int)(l + n))] = (Rpp8u) round(pixel);
                        }
                    }
                }
            }
        }
    }
*/




    return RPP_SUCCESS;
}






















                    