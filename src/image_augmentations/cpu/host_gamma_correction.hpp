#include <cpu/rpp_cpu_common.hpp>

template <typename T>

// With OMP
///*
RppStatus gamma_correction_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                                   Rpp32f gamma,
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

                    pixel = ((Rpp32f) srcPtr[jLoc]) / 255.0;
                    pixel = pow(pixel, gamma);
                    pixel = pixel * 255.0;
                    pixel = RPPPIXELCHECK(pixel);
                    dstPtr[jLoc] = (T) pixel;
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

                    pixel = ((Rpp32f) srcPtr[cLoc]) / 255.0;
                    pixel = pow(pixel, gamma);
                    pixel = pixel * 255.0;
                    pixel = RPPPIXELCHECK(pixel);
                    dstPtr[cLoc] = (T) pixel;
                }
            }
        }
    }

    return RPP_SUCCESS;

}
//*/













// Without OMP
/*
RppStatus gamma_correction_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                                   Rpp32f gamma,
                                   RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    Rpp32f pixel;

    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        pixel = ((Rpp32f) (*srcPtrTemp)) / 255.0;
        pixel = pow(pixel, gamma);
        pixel = pixel * 255.0;
        pixel = RPPPIXELCHECK(pixel);
        *dstPtrTemp = (T) pixel;
        srcPtrTemp++;
        dstPtrTemp++;
    }

    return RPP_SUCCESS;

}
*/