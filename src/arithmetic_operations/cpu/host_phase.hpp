#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus phase_host(T* srcPtr1, U* srcPtr2, RppiSize srcSize, T* dstPtr,
                   RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtr1Temp, *dstPtrTemp;
    U *srcPtr2Temp;
    srcPtr1Temp = srcPtr1;
    srcPtr2Temp = srcPtr2;
    dstPtrTemp = dstPtr;

    Rpp32f pixel;

    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        pixel = atan(((Rpp32f) (*srcPtr1Temp)) / ((Rpp32f) (*srcPtr2Temp)));
        //pixel = RPPPIXELCHECK(pixel);
        *dstPtrTemp =(T) pixel;
        srcPtr1Temp++;
        srcPtr2Temp++;
        dstPtrTemp++;
    }

    return RPP_SUCCESS;

}