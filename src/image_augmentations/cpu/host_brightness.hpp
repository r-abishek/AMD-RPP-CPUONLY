#include <cpu/rpp_cpu_common.hpp>

// Without OMP - With Batch
///*
template <typename T>
RppStatus brightness_host(T* srcPtr, RppiSize *srcSize, T* dstPtr, 
                          Rpp32f *alpha, Rpp32f *beta, RppiROI *roiPoints, Rpp32u nbatchSize,
                          RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    Rpp32f pixel;
    Rpp32u x1,y1,x2,y2;
    Rpp32f alphaBatch, betaBatch;
    if(chnFormat == RPPI_CHN_PLANAR)
    {
        for(int batch_cnt = 0; batch_cnt < nbatchSize; batch_cnt ++){
            x1 = roiPoints[batch_cnt].x;
            y1 = roiPoints[batch_cnt].y;
            x2 = x1 + roiPoints[batch_cnt].roiWidth;
            y2 = y1 + roiPoints[batch_cnt].roiHeight;
            if (x2 == 0) x2 = srcSize[batch_cnt].width;
            if (y2 == 0) y2 = srcSize[batch_cnt].height;
            alphaBatch = alpha[batch_cnt];
            betaBatch = beta[batch_cnt];
            for(int i = 0; i < channel; i++){
                for(int j = 0; j < srcSize[batch_cnt].height; j++){
                    for(int k = 0; k < srcSize[batch_cnt].width; k++){
                        if((x1 <= k) && (k <= x2 ) && ( y1 <= j) && (j <= y2)){
                            pixel = ((Rpp32f) (*srcPtrTemp)) * alphaBatch + betaBatch;
                            pixel = RPPPIXELCHECK(pixel);
                            *dstPtrTemp = (T) round(pixel);
                            srcPtrTemp++;
                            dstPtrTemp++;
                        } else {
                            *dstPtrTemp = *srcPtrTemp;
                            srcPtrTemp++;
                            dstPtrTemp++;
                        }
                    }
                }
            }
        }
    } else if (chnFormat == RPPI_CHN_PACKED)
    {
        for(int batch_cnt = 0; batch_cnt < nbatchSize; batch_cnt ++){
            x1 = roiPoints[batch_cnt].x;
            y1 = roiPoints[batch_cnt].y;
            x2 = x1 + roiPoints[batch_cnt].roiWidth;
            y2 = y1 + roiPoints[batch_cnt].roiHeight;
            if (x2 == 0) x2 = srcSize[batch_cnt].width;
            if (y2 == 0) y2 = srcSize[batch_cnt].height;
            alphaBatch = alpha[batch_cnt];
            betaBatch = beta[batch_cnt];
            //std::cerr<<"\n x1 "<<x1<<" x2 "<<x2<<" y1 "<<y1<<" y2 "<<y2<<std::endl;
            for(int j = 0; j < srcSize[batch_cnt].height; j++){
                for(int k = 0; k < srcSize[batch_cnt].width; k++){
                    for(int i = 0; i < channel; i++){
                        if((x1 <= k) && (k <= x2 ) && ( y1 <= j) && (j <= y2)){
                            pixel = ((Rpp32f) (*srcPtrTemp)) * alphaBatch + betaBatch;
                            pixel = RPPPIXELCHECK(pixel);
                            *dstPtrTemp = (T) round(pixel);
                            srcPtrTemp++;
                            dstPtrTemp++;
                        } else {
                            *dstPtrTemp = *srcPtrTemp;
                            srcPtrTemp++;
                            dstPtrTemp++;
                        }
                    }
                }
            }
        }
    }
    return RPP_SUCCESS;
}
//*/





















// With OMP
/*
template <typename T>
RppStatus brightness_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                                   Rpp32f alpha, Rpp32f beta,
                                   RppiChnFormat chnFormat, Rpp32u channel)
{
    Rpp32f pixel;
    Rpp32u imageDim = srcSize.height * srcSize.width;
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            Rpp32u cLoc = (c * imageDim);
            #pragma omp parallel for simd firstprivate(cLoc, pixel)
            for (int i = 0; i < srcSize.height; i++)
            {
                Rpp32u iLoc = cLoc + (i * srcSize.width);
                for (int j = 0; j < srcSize.width; j++)
                {
                    Rpp32u jLoc = iLoc + j;
                    pixel = ((Rpp32f) (srcPtr[jLoc])) * alpha + beta;
                    pixel = RPPPIXELCHECK(pixel);
                    dstPtr[jLoc] = (T) round(pixel);
                }
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        Rpp32u elementsInRow = channel * srcSize.width;
        #pragma omp parallel for simd firstprivate(pixel)
        for (int i = 0; i < srcSize.height; i++)
        {
            Rpp32u iLoc = (i * elementsInRow);
            for (int j = 0; j < srcSize.width; j++)
            {
                Rpp32u jLoc = iLoc + (j * channel);
                for (int c = 0; c < channel; c++)
                {
                    Rpp32u cLoc = jLoc + c;
                    pixel = ((Rpp32f) (srcPtr[cLoc])) * alpha + beta;
                    pixel = RPPPIXELCHECK(pixel);
                    dstPtr[cLoc] = (T) round(pixel);
                }
            }
        }
    }

    return RPP_SUCCESS;

}
*/







// Without OMP
/*
template <typename T>
RppStatus brightness_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                                   Rpp32f alpha, Rpp32f beta,
                                   RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    Rpp32f pixel;

    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        pixel = ((Rpp32f) (*srcPtrTemp)) * alpha + beta;
        pixel = RPPPIXELCHECK(pixel);
        *dstPtrTemp = (T) round(pixel);
        srcPtrTemp++;
        dstPtrTemp++;
    }

    return RPP_SUCCESS;

}
*/