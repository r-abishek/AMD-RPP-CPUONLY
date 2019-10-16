#include <cpu/rpp_cpu_common.hpp>

// With OMP - Running without indexing (with pointers) - #pragma omp parallel for simd firstprivate - MULTIPLE LOOPS
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

    //#pragma omp parallel for simd firstprivate(pixel)
    #pragma omp parallel
    #pragma omp taskloop simd firstprivate(pixel) lastprivate(srcPtrTemp, dstPtrTemp)
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        pixel = ((Rpp32f) (*srcPtrTemp)) * alpha + beta;
        // pixel = ((Rpp32f) (srcPtr[i])) * alpha + beta;
        pixel = RPPPIXELCHECK(pixel);
        *dstPtrTemp = (T) round(pixel);
        //dstPtr[i] = (T) round(pixel);
        dstPtrTemp++;
        srcPtrTemp++;
    }

    return RPP_SUCCESS;

}
*/













// With OMP - Running by indexing (single increment) - #pragma omp parallel for simd firstprivate - MULTIPLE LOOPS
/*
template <typename T>
RppStatus brightness_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                                   Rpp32f alpha, Rpp32f beta,
                                   RppiChnFormat chnFormat, Rpp32u channel)
{
    //T *srcPtrTemp, *dstPtrTemp;
    //srcPtrTemp = srcPtr;
    //dstPtrTemp = dstPtr;

    Rpp32f pixel;
    Rpp32u imageDim = srcSize.height * srcSize.width;

    //int c = 0, i = 0, j = 0;
    //#pragma omp parallel for private(pixel, c, i, j) collapse(2)
    
    
    for (int c = 0; c < channel; c++)
    {
        Rpp32u cLoc = c * imageDim;
        #pragma omp parallel
        #pragma omp taskloop firstprivate(pixel, cLoc)
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


    //for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    //{
    //    // pixel = ((Rpp32f) (*srcPtrTemp)) * alpha + beta;
    //    pixel = ((Rpp32f) (srcPtr[i])) * alpha + beta;
    //    pixel = RPPPIXELCHECK(pixel);
    //    // *dstPtrTemp = (T) round(pixel);
    //    dstPtr[i] = (T) round(pixel);
    //}

    return RPP_SUCCESS;

}
*/




















// With OMP - Running without indexing (with pointers) - #pragma omp parallel for simd firstprivate
/*
template <typename T>
RppStatus brightness_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                                   Rpp32f alpha, Rpp32f beta,
                                   RppiChnFormat chnFormat, Rpp32u channel)
{
    // T *srcPtrTemp, *dstPtrTemp;
    // srcPtrTemp = srcPtr;
    // dstPtrTemp = dstPtr;
    // Rpp32f pixel;
    // //#pragma omp parallel for simd firstprivate(pixel)
    // #pragma omp parallel
    // #pragma omp taskloop simd firstprivate(pixel, srcPtrTemp, dstPtrTemp)
    // for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    // {
    //     pixel = ((Rpp32f) (*srcPtrTemp)) * alpha + beta;
    //     // pixel = ((Rpp32f) (srcPtr[i])) * alpha + beta;
    //     pixel = RPPPIXELCHECK(pixel);
    //     *dstPtrTemp = (T) round(pixel);
    //     //dstPtr[i] = (T) round(pixel);
    //     dstPtrTemp++;
    //     srcPtrTemp++;
    // }

    T *srcPtrTemp, *dstPtrTemp;
    #pragma omp parallel
    {
        srcPtrTemp = srcPtr;
        dstPtrTemp = dstPtr;
        #pragma omp single
        {
            for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
            {
                //#pragma omp task firstprivate(srcPtrTemp, dstPtrTemp)
                #pragma omp task depend(in: srcPtrTemp, dstPtrTemp)
                {
                    Rpp32f pixel;
                    pixel = ((Rpp32f) (*srcPtrTemp)) * alpha + beta;
                    pixel = RPPPIXELCHECK(pixel);
                    *dstPtrTemp = (T) round(pixel);
                    dstPtrTemp++;
                    srcPtrTemp++;
                }
                
            }
        }
    }







    return RPP_SUCCESS;

}
*/








// With OMP - Running without indexing (with pointers) - #pragma omp parallel for firstprivate
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

    #pragma omp parallel for firstprivate(pixel)
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        pixel = ((Rpp32f) (*srcPtrTemp)) * alpha + beta;
        // pixel = ((Rpp32f) (srcPtr[i])) * alpha + beta;
        pixel = RPPPIXELCHECK(pixel);
        *dstPtrTemp = (T) round(pixel);
        //dstPtr[i] = (T) round(pixel);
        *dstPtrTemp++;
        *srcPtrTemp++;
    }

    return RPP_SUCCESS;

}
*/
























// ----------------------------------------------------------------------------------------------------------------------------
// TESTING PART
// ----------------------------------------------------------------------------------------------------------------------------

// With OMP - Running by indexing (single increment) - #pragma omp parallel for simd firstprivate - MULTIPLE LOOPS
///*
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
                //#pragma omp parallel for simd firstprivate(iLoc, pixel)
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
            //#pragma omp parallel for firstprivate(iLoc, pixel)
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
//*/































// ----------------------------------------------------------------------------------------------------------------------------
// WORKING PART
// ----------------------------------------------------------------------------------------------------------------------------

// With OMP - Running by indexing (single increment) - #pragma omp parallel for simd firstprivate - MULTIPLE LOOPS
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
                //#pragma omp parallel for simd firstprivate(iLoc, pixel)
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
            //#pragma omp parallel for firstprivate(iLoc, pixel)
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














// With OMP - Running by indexing (single increment) - #pragma omp parallel for simd firstprivate
/*
template <typename T>
RppStatus brightness_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                                   Rpp32f alpha, Rpp32f beta,
                                   RppiChnFormat chnFormat, Rpp32u channel)
{
    //T *srcPtrTemp, *dstPtrTemp;
    //srcPtrTemp = srcPtr;
    //dstPtrTemp = dstPtr;

    Rpp32f pixel;

    //#pragma omp parallel
    //{
    //    printf("\nNumber of processors = %d", omp_get_max_threads());
    //    printf("\nNumber of threads given = %d", omp_get_num_threads());
    //    #pragma omp for simd firstprivate(pixel)
    //    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    //    {
    //        // pixel = ((Rpp32f) (*srcPtrTemp)) * alpha + beta;
    //        pixel = ((Rpp32f) (srcPtr[i])) * alpha + beta;
    //        pixel = RPPPIXELCHECK(pixel);
    //        // *dstPtrTemp = (T) round(pixel);
    //        dstPtr[i] = (T) round(pixel);
    //    }
    //}

    #pragma omp parallel for simd firstprivate(pixel)
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        // pixel = ((Rpp32f) (*srcPtrTemp)) * alpha + beta;
        pixel = ((Rpp32f) (srcPtr[i])) * alpha + beta;
        pixel = RPPPIXELCHECK(pixel);
        // *dstPtrTemp = (T) round(pixel);
        dstPtr[i] = (T) round(pixel);
    }

    return RPP_SUCCESS;

}
*/











// With OMP - Running by indexing (single increment) - #pragma omp parallel for firstprivate
/*
template <typename T>
RppStatus brightness_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                                   Rpp32f alpha, Rpp32f beta,
                                   RppiChnFormat chnFormat, Rpp32u channel)
{
    //T *srcPtrTemp, *dstPtrTemp;
    //srcPtrTemp = srcPtr;
    //dstPtrTemp = dstPtr;

    Rpp32f pixel;

    #pragma omp parallel for firstprivate(pixel)
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        // pixel = ((Rpp32f) (*srcPtrTemp)) * alpha + beta;
        pixel = ((Rpp32f) (srcPtr[i])) * alpha + beta;
        pixel = RPPPIXELCHECK(pixel);
        // *dstPtrTemp = (T) round(pixel);
        dstPtr[i] = (T) round(pixel);
    }

    return RPP_SUCCESS;

}
*/











// With OMP - Running by indexing (single increment) - #pragma omp parallel for
/*
template <typename T>
RppStatus brightness_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                                   Rpp32f alpha, Rpp32f beta,
                                   RppiChnFormat chnFormat, Rpp32u channel)
{
    //T *srcPtrTemp, *dstPtrTemp;
    //srcPtrTemp = srcPtr;
    //dstPtrTemp = dstPtr;

    Rpp32f pixel;

    #pragma omp parallel for
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        // pixel = ((Rpp32f) (*srcPtrTemp)) * alpha + beta;
        pixel = ((Rpp32f) (srcPtr[i])) * alpha + beta;
        pixel = RPPPIXELCHECK(pixel);
        // *dstPtrTemp = (T) round(pixel);
        dstPtr[i] = (T) round(pixel);
    }

    return RPP_SUCCESS;

}
*/










// With OMP - Running by indexing (single increment) - #pragma omp parallel for simd
/*
template <typename T>
RppStatus brightness_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                                   Rpp32f alpha, Rpp32f beta,
                                   RppiChnFormat chnFormat, Rpp32u channel)
{
    //T *srcPtrTemp, *dstPtrTemp;
    //srcPtrTemp = srcPtr;
    //dstPtrTemp = dstPtr;

    Rpp32f pixel;

    #pragma omp parallel for simd
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        // pixel = ((Rpp32f) (*srcPtrTemp)) * alpha + beta;
        pixel = ((Rpp32f) (srcPtr[i])) * alpha + beta;
        pixel = RPPPIXELCHECK(pixel);
        // *dstPtrTemp = (T) round(pixel);
        dstPtr[i] = (T) round(pixel);
    }

    return RPP_SUCCESS;

}
*/




// Running by indexing (single increment)
/*
template <typename T>
RppStatus brightness_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                                   Rpp32f alpha, Rpp32f beta,
                                   RppiChnFormat chnFormat, Rpp32u channel)
{
    //T *srcPtrTemp, *dstPtrTemp;
    //srcPtrTemp = srcPtr;
    //dstPtrTemp = dstPtr;

    Rpp32f pixel;

    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        // pixel = ((Rpp32f) (*srcPtrTemp)) * alpha + beta;
        pixel = ((Rpp32f) (srcPtr[i])) * alpha + beta;
        pixel = RPPPIXELCHECK(pixel);
        // *dstPtrTemp = (T) round(pixel);
        dstPtr[i] = (T) round(pixel);
    }

    return RPP_SUCCESS;

}
*/




// Current Version
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