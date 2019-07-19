#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus accumulate_host(T* srcPtr1, U* srcPtr2, RppiSize srcSize,
                                   unsigned int channel)
{
    T *srcPtr1Temp;
    U *srcPtr2Temp;
    srcPtr1Temp = srcPtr1;
    srcPtr2Temp = srcPtr2;

    Rpp32f pixel;
    
    for (int i = 0; i < (channel * srcSize.width * srcSize.height); i++)
    {
        pixel = ((Rpp32f) (*srcPtr1Temp)) + ((Rpp32f) (*srcPtr2Temp));
        pixel = RPPPIXELCHECK(pixel);
        *srcPtr1Temp =(T) pixel;
        srcPtr1Temp++;
        srcPtr2Temp++;
    }

    return RPP_SUCCESS;

}