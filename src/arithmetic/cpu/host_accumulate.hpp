#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus host_accumulate(T* srcPtr1, T* srcPtr2, RppiSize srcSize,
                                   unsigned int channel)
{
    for (int i = 0; i < (channel * srcSize.width * srcSize.height); i++)
    {
        Rpp32s pixel = ((Rpp32s) srcPtr1[i]) + ((Rpp32s) srcPtr2[i]);
        pixel = (pixel < (Rpp32s) 255) ? pixel : ((Rpp32s) 255);
        pixel = (pixel > (Rpp32s) 0) ? pixel : ((Rpp32s) 0);
        srcPtr1[i] =(Rpp8u) pixel;
    }

    return RPP_SUCCESS;

}