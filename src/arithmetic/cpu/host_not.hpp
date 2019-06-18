#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus not_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                                   unsigned int channel)
{
    for (int i = 0; i < (channel * srcSize.width * srcSize.height); i++)
    {
        Rpp8u pixel = ~(srcPtr[i]);
        pixel = (pixel < (Rpp8u) 255) ? pixel : ((Rpp8u) 255);
        pixel = (pixel > (Rpp8u) 0) ? pixel : ((Rpp8u) 0);
        dstPtr[i] = pixel;
    }

    return RPP_SUCCESS;

}