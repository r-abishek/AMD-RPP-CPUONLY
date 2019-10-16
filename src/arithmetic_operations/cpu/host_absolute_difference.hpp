#include <cpu/rpp_cpu_common.hpp>

// With OMP
///*
template <typename T, typename U>
RppStatus absolute_difference_host(T* srcPtr1, U* srcPtr2, RppiSize srcSize, T* dstPtr,
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

                    pixel = ((Rpp32s) srcPtr1[jLoc]) - ((Rpp32s) srcPtr2[jLoc]);
                    pixel = RPPABS(pixel);
                    pixel = RPPPIXELCHECK(pixel);
                    dstPtr[jLoc] =(T) pixel;
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

                    pixel = ((Rpp32s) srcPtr1[cLoc]) - ((Rpp32s) srcPtr2[cLoc]);
                    pixel = RPPABS(pixel);
                    pixel = RPPPIXELCHECK(pixel);
                    dstPtr[cLoc] =(T) pixel;
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
RppStatus absolute_difference_host(T* srcPtr1, U* srcPtr2, RppiSize srcSize, T* dstPtr,
                                   RppiChnFormat chnFormat, Rpp32u channel)
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
        pixel = RPPABS(pixel);
        pixel = RPPPIXELCHECK(pixel);
        *dstPtrTemp =(T) pixel;
        srcPtr1Temp++;
        srcPtr2Temp++;
        dstPtrTemp++;
    }

    return RPP_SUCCESS;

}
*/