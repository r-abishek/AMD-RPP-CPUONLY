#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus exposure_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                    Rpp32f exposureFactor,
                    RppiChnFormat chnFormat, unsigned channel)
{
    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    Rpp32f pixel;

    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        pixel = ((Rpp32f) (*srcPtrTemp)) * (pow(2, exposureFactor));
        pixel = RPPPIXELCHECK(pixel);
        *dstPtrTemp = (T) round(pixel);
        dstPtrTemp++;
        srcPtrTemp++;
    }

    return RPP_SUCCESS;
}