#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus meanStdDev_host(T* srcPtr, RppiSize srcSize,
                      Rpp32f* mean, Rpp32f* stdDev, 
                      RppiChnFormat chnFormat, Rpp32u channel)
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