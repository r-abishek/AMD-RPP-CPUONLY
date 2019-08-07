#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus hue_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                    Rpp32f hueShift,
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
            for (int i = 0; i < (srcSize.height * srcSize.width); i++)
            {
                *srcPtrHSVTemp = *srcPtrHSVTemp + hueShift;
                while (*srcPtrHSVTemp > 360)
                {
                    *srcPtrHSVTemp = *srcPtrHSVTemp - 360;
                }
                while (*srcPtrHSVTemp < 0)
                {
                    *srcPtrHSVTemp = 360 + *srcPtrHSVTemp;
                }
                srcPtrHSVTemp++;
            }
        }
        else if (chnFormat == RPPI_CHN_PACKED)
        {
            for (int i = 0; i < (srcSize.height * srcSize.width); i++)
            {
                *srcPtrHSVTemp = *srcPtrHSVTemp + hueShift;
                while (*srcPtrHSVTemp > 360)
                {
                    *srcPtrHSVTemp = *srcPtrHSVTemp - 360;
                }
                while (*srcPtrHSVTemp < 0)
                {
                    *srcPtrHSVTemp = 360 + *srcPtrHSVTemp;
                }
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
                *dstPtrTemp = *srcPtrTemp + hueShift;
                while (*dstPtrTemp > 360)
                {
                    *dstPtrTemp = *dstPtrTemp - 360;
                }
                while (*dstPtrTemp < 0)
                {
                    *dstPtrTemp = 360 + *dstPtrTemp;
                }
                srcPtrTemp++;
                dstPtrTemp++;
            }
            memcpy(dstPtrTemp, srcPtrTemp, 2 * srcSize.height * srcSize.width * sizeof(T));
        }
        else if (chnFormat == RPPI_CHN_PACKED)
        {
            for (int i = 0; i < (srcSize.height * srcSize.width); i++)
            {
                *dstPtrTemp = *srcPtrTemp + hueShift;
                while (*dstPtrTemp > 360)
                {
                    *dstPtrTemp = *dstPtrTemp - 360;
                }
                while (*dstPtrTemp < 0)
                {
                    *dstPtrTemp = 360 + *dstPtrTemp;
                }
                srcPtrTemp++;
                dstPtrTemp++;
                for (int c = 0; c < (channel - 1); c++)
                {
                    *dstPtrTemp = *srcPtrTemp;
                    srcPtrTemp++;
                    dstPtrTemp++;
                }
            }
        }
    }

    return RPP_SUCCESS;
}