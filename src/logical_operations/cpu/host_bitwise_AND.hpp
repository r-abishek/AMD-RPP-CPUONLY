#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus bitwise_AND_host(T* srcPtr1, U* srcPtr2, RppiSize srcSize, T* dstPtr,
                                   Rpp32u channel)
{
    T *srcPtr1Temp, *dstPtrTemp;
    U *srcPtr2Temp;
    srcPtr1Temp = srcPtr1;
    srcPtr2Temp = srcPtr2;
    dstPtrTemp = dstPtr;

    Rpp8u pixel;

    for (int i = 0; i < (channel * srcSize.width * srcSize.height); i++)
    {
        pixel = ((Rpp8u) (*srcPtr1Temp)) & ((Rpp8u) (*srcPtr2Temp));
        pixel = RPPPIXELCHECK(pixel);
        *dstPtrTemp =(T) pixel;
        srcPtr1Temp++;
        srcPtr2Temp++;
        dstPtrTemp++;
    }

    return RPP_SUCCESS;

}