#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus color_temperature_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    Rpp8s adjustmentValue,
                    RppiChnFormat chnFormat, unsigned int channel)
{
    if (channel != 3)
    {
        return RPP_ERROR;
    }
    if (adjustmentValue < -100 || adjustmentValue > 100)
    {
        return RPP_ERROR;
    }   

    Rpp32s pixel;
    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            pixel = (Rpp32s) *srcPtrTemp + (Rpp32s) adjustmentValue;
            pixel = (pixel < (Rpp32s) 255) ? pixel : ((Rpp32s) 255);
            pixel = (pixel > (Rpp32s) 0) ? pixel : ((Rpp32s) 0);
            *dstPtrTemp = (T) pixel;
            dstPtrTemp++;
            srcPtrTemp++;
        }
        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            *dstPtrTemp = *srcPtrTemp;
            dstPtrTemp++;
            srcPtrTemp++;
        }
        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            pixel = (Rpp32s) *srcPtrTemp + (Rpp32s) adjustmentValue;
            pixel = (pixel < (Rpp32s) 255) ? pixel : ((Rpp32s) 255);
            pixel = (pixel > (Rpp32s) 0) ? pixel : ((Rpp32s) 0);
            *dstPtrTemp = (T) pixel;
            dstPtrTemp++;
            srcPtrTemp++;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            pixel = (Rpp32s) *srcPtrTemp + (Rpp32s) adjustmentValue;
            pixel = (pixel < (Rpp32s) 255) ? pixel : ((Rpp32s) 255);
            pixel = (pixel > (Rpp32s) 0) ? pixel : ((Rpp32s) 0);
            *dstPtrTemp = (T) pixel;
            dstPtrTemp++;
            srcPtrTemp++;

            *dstPtrTemp = *srcPtrTemp;
            dstPtrTemp++;
            srcPtrTemp++;

            pixel = (Rpp32s) *srcPtrTemp + (Rpp32s) adjustmentValue;
            pixel = (pixel < (Rpp32s) 255) ? pixel : ((Rpp32s) 255);
            pixel = (pixel > (Rpp32s) 0) ? pixel : ((Rpp32s) 0);
            *dstPtrTemp = (T) pixel;
            dstPtrTemp++;
            srcPtrTemp++;
        }
    }
     
    return RPP_SUCCESS;
}