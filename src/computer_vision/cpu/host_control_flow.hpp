#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus control_flow_host(T* srcPtr1, U* srcPtr2, RppiSize srcSize, T* dstPtr, 
                            Rpp32u type, 
                            RppiChnFormat chnFormat, Rpp32u channel)
{
    switch(type)
    {
        case 1:
            compute_bitwise_AND_host(srcPtr1, srcPtr2, srcSize, dstPtr, channel);
            break;
        case 2:
            compute_inclusive_OR_host(srcPtr1, srcPtr2, srcSize, dstPtr, channel);
            break;
        case 3:
            compute_exclusive_OR_host(srcPtr1, srcPtr2, srcSize, dstPtr, channel);
            break;
        case 4:
            compute_add_host(srcPtr1, srcPtr2, srcSize, dstPtr, channel);
            break;
        case 5:
            compute_subtract_host(srcPtr1, srcPtr2, srcSize, dstPtr, channel);
            break;
        case 6:
            compute_multiply_host(srcPtr1, srcPtr2, srcSize, dstPtr, channel);
            break;
        case 7:
            compute_min_host(srcPtr1, srcPtr2, srcSize, dstPtr, channel);
            break;
        case 8:
            compute_max_host(srcPtr1, srcPtr2, srcSize, dstPtr, channel);
            break;
    }

    return RPP_SUCCESS;

}