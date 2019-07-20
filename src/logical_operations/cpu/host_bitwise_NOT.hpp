#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus bitwise_NOT_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                                   unsigned int channel)
{
    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    Rpp8u pixel;

    for (int i = 0; i < (channel * srcSize.width * srcSize.height); i++)
    {
        pixel = ~((Rpp8u) (*srcPtrTemp));
        pixel = RPPPIXELCHECK(pixel);
        *dstPtrTemp =(T) pixel;
        srcPtrTemp++;
        dstPtrTemp++;
    }

    return RPP_SUCCESS;

}