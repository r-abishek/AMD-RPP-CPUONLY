#include <cpu/rpp_cpu_common.hpp>

// With OMP
///*
template <typename T, typename U>
RppStatus accumulate_weighted_host(T* srcPtr1, U* srcPtr2, RppiSize srcSize,
                                   Rpp32f alpha,
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

                    pixel = ((1 - alpha) * ((Rpp32f) srcPtr1[jLoc])) + (alpha * ((Rpp32f) srcPtr2[jLoc]));
                    pixel = RPPPIXELCHECK(pixel);
                    srcPtr1[jLoc] =(T) pixel;
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

                    pixel = ((1 - alpha) * ((Rpp32f) srcPtr1[cLoc])) + (alpha * ((Rpp32f) srcPtr2[cLoc]));
                    pixel = RPPPIXELCHECK(pixel);
                    srcPtr1[cLoc] =(T) pixel;
                }
            }
        }
    }

    return RPP_SUCCESS;

}
//*/



















// Without OMP
/*
template <typename T, typename U>
RppStatus accumulate_weighted_host(T* srcPtr1, U* srcPtr2, RppiSize srcSize,
                                   Rpp32f alpha,
                                   RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtr1Temp;
    U *srcPtr2Temp;
    srcPtr1Temp = srcPtr1;
    srcPtr2Temp = srcPtr2;

    Rpp32f pixel;

    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        Rpp32f pixel = ((1 - alpha) * ((Rpp32f) (*srcPtr1Temp))) + (alpha * ((Rpp32f) (*srcPtr2Temp)));
        pixel = RPPPIXELCHECK(pixel);
        *srcPtr1Temp =(T) pixel;
        srcPtr1Temp++;
        srcPtr2Temp++;
    }

    return RPP_SUCCESS;

}
*/