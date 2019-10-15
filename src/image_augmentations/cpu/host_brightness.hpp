#include <cpu/rpp_cpu_common.hpp>

// With OMP - Running without indexing (with pointers) - #pragma omp parallel for simd firstprivate
/*
#include <omp.h>
template <typename T>
RppStatus brightness_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                                   Rpp32f alpha, Rpp32f beta,
                                   RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    Rpp32f pixel;

    #pragma omp parallel for simd firstprivate(pixel)
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












// With OMP - Running without indexing (with pointers) - #pragma omp parallel for firstprivate
///*
#include <omp.h>
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
//*/
















// With OMP - Running by indexing (single increment) - #pragma omp parallel for simd firstprivate
/*
#include <omp.h>
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
#include <omp.h>
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
#include <omp.h>
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
#include <omp.h>
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