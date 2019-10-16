#include <cpu/rpp_cpu_common.hpp>

// With OMP
///*
template <typename T>
RppStatus accumulate_squared_host(T* srcPtr, RppiSize srcSize,
                                  RppiChnFormat chnFormat, Rpp32u channel)
{
    Rpp32s pixel;
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

                    pixel = (Rpp32s) srcPtr[jLoc];
                    pixel = pixel * pixel;
                    pixel = RPPPIXELCHECK(pixel);
                    srcPtr[jLoc] =(T) pixel;
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

                    pixel = (Rpp32s) srcPtr[cLoc];
                    pixel = pixel * pixel;
                    pixel = RPPPIXELCHECK(pixel);
                    srcPtr[cLoc] =(T) pixel;
                }
            }
        }
    }

    return RPP_SUCCESS;

}
//*/


















// Without OMP
/*
template <typename T>
RppStatus accumulate_squared_host(T* srcPtr, RppiSize srcSize,
                                  RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTemp;
    srcPtrTemp = srcPtr;

    Rpp32s pixel;
    
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        pixel = (Rpp32s) *srcPtrTemp;
        pixel = pixel * pixel;
        pixel = RPPPIXELCHECK(pixel);
        *srcPtrTemp =(T) pixel;
        srcPtrTemp++;
    }

    return RPP_SUCCESS;

}
*/