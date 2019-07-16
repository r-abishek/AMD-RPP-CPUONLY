#include <cpu/rpp_cpu_common.hpp>
#include <limits>

template <typename T>
RppStatus histogram_host(T* srcPtr, RppiSize srcSize, Rpp32u* outputHistogram, Rpp32u bins, 
                         unsigned int channel)
{
    histogram_kernel_host(srcPtr, srcSize, outputHistogram, bins, channel);
/*    
    for (int c = 0; c < channel; c++)
    {
        memset (histogram,0,bins * sizeof(Rpp32u));

        Rpp32u *histogramTemp;
        histogramTemp = histogram;

        histogram_kernel_host(srcPtrTemp, srcSize, histogram, bins, 0, chnFormat, channel);

        for (int i = 0; i < bins; i++)
        {
            *outputHistogramTemp = *histogramTemp;
            outputHistogramTemp++;
            histogramTemp++;
        }
        if (chnFormat == RPPI_CHN_PLANAR)
        {
            srcPtrTemp += (srcSize.height * srcSize.width);
        }
        else if (chnFormat == RPPI_CHN_PACKED)
        {
            srcPtrTemp += channel;
        }
    }
*/
    return RPP_SUCCESS;

}