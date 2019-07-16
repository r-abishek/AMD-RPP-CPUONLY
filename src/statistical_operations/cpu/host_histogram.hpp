#include <cpu/rpp_cpu_common.hpp>
#include <limits>

template <typename T>
RppStatus histogram_host(T* srcPtr, RppiSize srcSize, Rpp32u* outputHistogram, Rpp32u bins, 
                         unsigned int channel)
{
    histogram_kernel_host(srcPtr, srcSize, outputHistogram, bins, channel);

    return RPP_SUCCESS;

}