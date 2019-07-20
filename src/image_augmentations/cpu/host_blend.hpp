#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus blend_host(T* srcPtr1, T* srcPtr2, RppiSize srcSize, T* dstPtr,
                                   Rpp32f alpha,
                                   Rpp32u channel)
{
    for (int i = 0; i < (channel * srcSize.width * srcSize.height); i++)
    {
        Rpp32f pixel = ((1 - alpha) * ((Rpp32f) srcPtr1[i])) + (alpha * ((Rpp32s) srcPtr2[i]));
        pixel = RPPPIXELCHECK(pixel);
        dstPtr[i] =(Rpp8u) pixel;
    }

    return RPP_SUCCESS;

}