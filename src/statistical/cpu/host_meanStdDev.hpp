#include "host_declarations.hpp"
#include <math.h>

template <typename T>
RppStatus host_meanStd(T* srcPtr, RppiSize srcSize,
                      Rpp32f* mean, Rpp32f* stdDev, 
                      RppiChnFormat chnFormat, unsigned int channel)
{
    for (int i = 0; i < (channel * srcSize.width * srcSize.height); i++)
    {
        *mean += (Rpp32f) srcPtr[i];
    }
    *mean /= ((Rpp32f)(channel * srcSize.width * srcSize.height));
    for (int i = 0; i < (channel * srcSize.width * srcSize.height); i++)
    {
        *stdDev += (((Rpp32f) srcPtr[i] - *mean) * ((Rpp32f) srcPtr[i] - *mean));
    }
    *stdDev /= ((Rpp32f)((channel * srcSize.width * srcSize.height) - 1));
    *stdDev = sqrt(*stdDev);


    return RPP_SUCCESS;

}