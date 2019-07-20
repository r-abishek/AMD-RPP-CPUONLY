#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus exposure_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                    Rpp32f exposureFactor,
                    RppiChnFormat chnFormat, unsigned channel, RppiFormat imageFormat)
{
    Rpp32f pixel;
    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    if (imageFormat == RGB)
    {
        for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
        {
            pixel = *srcPtrTemp * (pow(2, exposureFactor));
            pixel = RPPPIXELCHECK(pixel);
            *dstPtrTemp = (T) round(pixel);
            dstPtrTemp++;
            srcPtrTemp++;
        }
    }
    else if (imageFormat == HSV)
    {
        exposureFactor = RPPABS(exposureFactor);
        if (chnFormat == RPPI_CHN_PLANAR)
        {
            for (int i = 0; i < ((channel - 1) * (srcSize.height * srcSize.width)); i++)
            {
                *dstPtrTemp = *srcPtrTemp;
                srcPtrTemp++;
                dstPtrTemp++;
            }
            for (int i = 0; i < (srcSize.height * srcSize.width); i++)
            {
                pixel = *srcPtrTemp * exposureFactor;
                pixel = (pixel < (Rpp32f) 1) ? pixel : ((Rpp32f) 1);
                pixel = (pixel > (Rpp32f) 0) ? pixel : ((Rpp32f) 0);
                *dstPtrTemp = pixel;
                dstPtrTemp++;
                srcPtrTemp++;
            }
        }
        else if (chnFormat == RPPI_CHN_PACKED)
        {
            int count = 0;
            for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
            {
                if (count == 2)
                {
                    pixel = *srcPtrTemp * exposureFactor;
                    pixel = (pixel < (Rpp32f) 1) ? pixel : ((Rpp32f) 1);
                    pixel = (pixel > (Rpp32f) 0) ? pixel : ((Rpp32f) 0);
                    *dstPtrTemp = pixel;
                    dstPtrTemp++;
                    srcPtrTemp++;
                    count = 0;
                }
                else
                {
                    *dstPtrTemp = *srcPtrTemp;
                    dstPtrTemp++;
                    srcPtrTemp++;
                    count++;
                }
            }
        }
    }

    return RPP_SUCCESS;
}