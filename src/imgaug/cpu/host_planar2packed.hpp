#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus planar2packed_host(T* srcPtr, RppiSize srcSize, T* dstPtr, 
                    unsigned channel)
{
    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    for (int c = 0; c < channel; c++)
    {
        dstPtrTemp += c;
        for (int i = 0; i < srcSize.height; i++)
        {
            for (int j = 0; j < srcSize.width; j++)
            {
                *dstPtrTemp = *srcPtrTemp;
                srcPtrTemp++;
                dstPtrTemp += 3;
            }
        }
        dstPtrTemp = dstPtr;
    }

    return RPP_SUCCESS;
}