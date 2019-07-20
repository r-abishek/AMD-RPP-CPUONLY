#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus minMax_host(T* srcPtr, RppiSize srcSize, T* maskPtr,
                      Rpp8u* min, Rpp8u* max, 
                      RppiChnFormat chnFormat, Rpp32u channel)
{
    *min = 255;
    *max = 0;
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
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
                }
                if (srcPtr[i] < *min)
                {
                    *min = srcPtr[i];
                }
            }
        }
    }


    return RPP_SUCCESS;

}