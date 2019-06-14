#include "host_declarations.hpp"

template <typename T>
RppStatus host_minMax(T* srcPtr, RppiSize srcSize, T* maskPtr,
                      Rpp8u* min, Rpp8u* max, 
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