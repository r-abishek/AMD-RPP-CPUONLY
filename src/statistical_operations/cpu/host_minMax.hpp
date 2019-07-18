#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus minMax_host(T* srcPtr, RppiSize srcSize, 
                         Rpp8u* min, Rpp8u* max, 
                         RppiChnFormat chnFormat, unsigned int channel)
{
    *min = 255;
    *max = 0;

    T *srcPtrTemp;
    srcPtrTemp = srcPtr;
    for (int i = 0; i < (channel * srcSize.width * srcSize.height); i++)
    {
        if (*srcPtrTemp > *max)
        {
            *max = *srcPtrTemp;
        }
        if (*srcPtrTemp < *min)
        {
            *min = *srcPtrTemp;
        }
        srcPtrTemp++;
    }

    return RPP_SUCCESS;

}