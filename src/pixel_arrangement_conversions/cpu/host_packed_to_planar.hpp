#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus packed_to_planar_host(T* srcPtr, RppiSize srcSize, T* dstPtr, 
                    Rpp32u channel)
{
    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    for (int c = 0; c < channel; c++)
    {
        srcPtrTemp += c;
        for (int i = 0; i < srcSize.height; i++)
        {
            for (int j = 0; j < srcSize.width; j++)
            {
                *dstPtrTemp = *srcPtrTemp;
                dstPtrTemp++;
                srcPtrTemp += 3;
            }
        }
        srcPtrTemp = srcPtr;
    }

    return RPP_SUCCESS;
}