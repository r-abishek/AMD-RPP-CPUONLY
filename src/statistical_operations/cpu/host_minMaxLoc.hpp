#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus minMaxLoc_host(T* srcPtr, RppiSize srcSize, 
                         Rpp8u* min, Rpp8u* max, Rpp32s* minLoc, Rpp32s* maxLoc, 
                         RppiChnFormat chnFormat, unsigned int channel)
{
    *min = 255;
    *max = 0;

    T *srcPtrTemp, *minLocPtrTemp, *maxLocPtrTemp;
    srcPtrTemp = srcPtr;
    for (int i = 0; i < (channel * srcSize.width * srcSize.height); i++)
    {
        if (*srcPtrTemp > *max)
        {
            *max = *srcPtrTemp;
            maxLocPtrTemp = srcPtrTemp;
        }
        if (*srcPtrTemp < *min)
        {
            *min = *srcPtrTemp;
            minLocPtrTemp = srcPtrTemp;
        }
        srcPtrTemp++;
    }
    *minLoc = minLocPtrTemp - srcPtr;
    *maxLoc = maxLocPtrTemp - srcPtr;

    return RPP_SUCCESS;

}