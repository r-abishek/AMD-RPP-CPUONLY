#include <cpu/rpp_cpu_common.hpp>

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