#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus magnitude_host(T* srcPtr1, T* srcPtr2, RppiSize srcSize, T* dstPtr,
                         RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtr1Temp, *srcPtr2Temp, *dstPtrTemp;
    srcPtr1Temp = srcPtr1;
    srcPtr2Temp = srcPtr2;
    dstPtrTemp = dstPtr;

    Rpp32f pixel;
    
    for (int i = 0; i < (channel * srcSize.width * srcSize.height); i++)
    {
        pixel = sqrt(((Rpp32f)(*srcPtr1Temp) * (Rpp32f)(*srcPtr1Temp)) + ((Rpp32f)(*srcPtr2Temp) * (Rpp32f)(*srcPtr2Temp)));
        pixel = (pixel < (Rpp32f) 255) ? pixel : ((Rpp32f) 255);
        pixel = (pixel > (Rpp32f) 0) ? pixel : ((Rpp32f) 0);
        *dstPtrTemp =(Rpp8u) round(pixel);
        srcPtr1Temp++;
        srcPtr2Temp++;
        dstPtrTemp++;
    }

    return RPP_SUCCESS;

}