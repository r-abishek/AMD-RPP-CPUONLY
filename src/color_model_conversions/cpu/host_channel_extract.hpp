#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus channel_extract_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    Rpp32u extractChannelNumber, 
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    compute_channel_extract_host(srcPtr, srcSize, dstPtr, extractChannelNumber, chnFormat, channel);

    return RPP_SUCCESS;
}
