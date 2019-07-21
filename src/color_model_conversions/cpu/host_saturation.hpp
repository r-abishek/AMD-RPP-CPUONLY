#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus saturation_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                    Rpp32f saturationFactor,
                    RppiChnFormat chnFormat, Rpp32u channel, RppiFormat imageFormat)
{
    if (imageFormat == RGB)
    {
        Rpp32f *srcPtrHSV = (Rpp32f *)calloc(channel * srcSize.height * srcSize.width, sizeof(Rpp32f));
        compute_rgb_to_hsv_host(srcPtr, srcSize, srcPtrHSV, chnFormat, channel);

        Rpp32f *srcPtrHSVTemp;
        srcPtrHSVTemp = srcPtrHSV;

        if (chnFormat == RPPI_CHN_PLANAR)
        {
            srcPtrHSVTemp = srcPtrHSV + (srcSize.height * srcSize.width);
            for (int i = 0; i < (srcSize.height * srcSize.width); i++)
            {
                *srcPtrHSVTemp *= saturationFactor;
                *srcPtrHSVTemp = (*srcPtrHSVTemp < (Rpp32f) 1) ? *srcPtrHSVTemp : ((Rpp32f) 1);
                *srcPtrHSVTemp = (*srcPtrHSVTemp > (Rpp32f) 0) ? *srcPtrHSVTemp : ((Rpp32f) 0);
                srcPtrHSVTemp++;
            }
        }
        else if (chnFormat == RPPI_CHN_PACKED)
        {
            srcPtrHSVTemp = srcPtrHSV + 1;
            for (int i = 0; i < (srcSize.height * srcSize.width); i++)
            {
                *srcPtrHSVTemp *= saturationFactor;
                *srcPtrHSVTemp = (*srcPtrHSVTemp < (Rpp32f) 1) ? *srcPtrHSVTemp : ((Rpp32f) 1);
                *srcPtrHSVTemp = (*srcPtrHSVTemp > (Rpp32f) 0) ? *srcPtrHSVTemp : ((Rpp32f) 0);
                srcPtrHSVTemp = srcPtrHSVTemp + channel;
            }
        }

        compute_hsv_to_rgb_host(srcPtrHSV, srcSize, dstPtr, chnFormat, channel);
    }
    else if (imageFormat == HSV)
    {
        T *srcPtrTemp, *dstPtrTemp;
        srcPtrTemp = srcPtr;
        dstPtrTemp = dstPtr;

        if (chnFormat == RPPI_CHN_PLANAR)
        {
            for (int i = 0; i < (srcSize.height * srcSize.width); i++)
            {
                *dstPtrTemp = *srcPtrTemp;
                srcPtrTemp++;
                dstPtrTemp++;
            }
            for (int i = 0; i < (srcSize.height * srcSize.width); i++)
            {
                *dstPtrTemp = *srcPtrTemp * saturationFactor;
                *dstPtrTemp = (*dstPtrTemp < (Rpp32f) 1) ? *dstPtrTemp : ((Rpp32f) 1);
                *dstPtrTemp = (*dstPtrTemp > (Rpp32f) 0) ? *dstPtrTemp : ((Rpp32f) 0);
                srcPtrTemp++;
                dstPtrTemp++;
            }
            for (int i = 0; i < (srcSize.height * srcSize.width); i++)
            {
                *dstPtrTemp = *srcPtrTemp;
                srcPtrTemp++;
                dstPtrTemp++;
            }
        }
        else if (chnFormat == RPPI_CHN_PACKED)
        {
            for (int i = 0; i < (srcSize.height * srcSize.width); i++)
            {
                *dstPtrTemp = *srcPtrTemp;
                srcPtrTemp++;
                dstPtrTemp++;

                *dstPtrTemp = *srcPtrTemp * saturationFactor;
                *dstPtrTemp = (*dstPtrTemp < (Rpp32f) 1) ? *dstPtrTemp : ((Rpp32f) 1);
                *dstPtrTemp = (*dstPtrTemp > (Rpp32f) 0) ? *dstPtrTemp : ((Rpp32f) 0);
                srcPtrTemp++;
                dstPtrTemp++;

                *dstPtrTemp = *srcPtrTemp;
                srcPtrTemp++;
                dstPtrTemp++;
            }
        }
    }

    return RPP_SUCCESS;
}