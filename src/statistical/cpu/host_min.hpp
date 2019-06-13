#include "host_declarations.hpp"

template <typename T>
RppStatus host_min(T* srcPtr1, T* srcPtr2, RppiSize srcSize, T* dstPtr,
                                   unsigned int channel)
{
    for (int i = 0; i < (channel * srcSize.width * srcSize.height); i++)
    {
        dstPtr[i] = RPPMIN2(srcPtr1[i], srcPtr2[i]);
    }

    return RPP_SUCCESS;

}