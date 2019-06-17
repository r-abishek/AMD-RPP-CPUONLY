#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus host_minMaxLoc(T* srcPtr, RppiSize srcSize, T* maskPtr,
                      Rpp8u* min, Rpp8u* max, Rpp8u** minLoc, Rpp8u** maxLoc,
                      RppiChnFormat chnFormat, unsigned int channel)
{
    *min = 255;
    *max = 0;
    for (int i = 0; i < (channel * srcSize.width * srcSize.height); i++)
    {
        if (maskPtr[i] != 0 && maskPtr[i] != 1)
        {
            return RPP_ERROR;
        }
        else
        {
            if (maskPtr[i] == 1)
            {
                if (srcPtr[i] > *max)
                {
                    *max = srcPtr[i];
                    *maxLoc = &srcPtr[i];
                }
                if (srcPtr[i] < *min)
                {
                    *min = srcPtr[i];
                    *minLoc = &srcPtr[i];
                }
            }
        }
    }

    return RPP_SUCCESS;

}