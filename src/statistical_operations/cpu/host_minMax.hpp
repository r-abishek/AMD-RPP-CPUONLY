#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus minMax_host(T* srcPtr, RppiSize srcSize, 
                         T* min, T* max, 
                         RppiChnFormat chnFormat, Rpp32u channel)
{
    *min = *srcPtr;
    *max = *srcPtr;

    T *srcPtrTemp;
    srcPtrTemp = srcPtr;
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
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