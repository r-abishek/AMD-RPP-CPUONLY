#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus bressenham_line_host(T* dstPtr, RppiSize dstSize, 
                               Rpp32u *endpoints, Rpp32u *rasterCoordinates, 
                               RppiChnFormat chnFormat, Rpp32u channel)
{
    generate_bressenham_line_host(dstPtr, dstSize, endpoints, rasterCoordinates);

    return RPP_SUCCESS;
}