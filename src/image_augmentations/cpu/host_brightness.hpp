#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus brightness_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                                   Rpp32f alpha, Rpp32f beta,
                                   Rpp32u channel)
{
    for (int i = 0; i < (channel * srcSize.width * srcSize.height); i++)
    {
        Rpp32f pixel = ((Rpp32f) srcPtr[i]) * alpha + beta;
        pixel = RPPPIXELCHECK(pixel);
        dstPtr[i] =(Rpp8u) pixel;
    }

    return RPP_SUCCESS;

}
