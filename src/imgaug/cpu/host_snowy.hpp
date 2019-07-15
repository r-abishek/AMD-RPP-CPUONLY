#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus snowy_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                    Rpp32f strength,
                    RppiChnFormat chnFormat, unsigned channel, RppiFormat imageFormat)
{
    if (strength < 0 || strength > 1)
    {
        return RPP_ERROR;
    }

    if (imageFormat == RGB)
    {
        Rpp32f *srcPtrHSL = (Rpp32f *)calloc(channel * srcSize.height * srcSize.width, sizeof(Rpp32f));
        if (chnFormat == RPPI_CHN_PLANAR)
        {
            rgb2hsl_host(srcPtr, srcSize, srcPtrHSL, RPPI_CHN_PLANAR, 3);

            Rpp32f *srcPtrHSLTemp;
            srcPtrHSLTemp = srcPtrHSL + (2 * srcSize.height * srcSize.width);

            for (int i = 0; i < srcSize.height * srcSize.width; i++)
            {
                if (*srcPtrHSLTemp < strength)
                {
                    *srcPtrHSLTemp *= 4;
                }
                if (*srcPtrHSLTemp > 1)
                {
                    *srcPtrHSLTemp = 1;
                }
                srcPtrHSLTemp++;
            }

            hsl2rgb_host(srcPtrHSL, srcSize, dstPtr, RPPI_CHN_PLANAR, 3);
        }
        else if (chnFormat == RPPI_CHN_PACKED)
        {
            rgb2hsl_host(srcPtr, srcSize, srcPtrHSL, RPPI_CHN_PACKED, 3);

            Rpp32f *srcPtrHSLTemp;
            srcPtrHSLTemp = srcPtrHSL + 2;

            for (int i = 0; i < srcSize.height * srcSize.width; i++)
            {
                if (*srcPtrHSLTemp < strength)
                {
                    *srcPtrHSLTemp *= 4;
                }
                if (*srcPtrHSLTemp > 1)
                {
                    *srcPtrHSLTemp = 1;
                }
                srcPtrHSLTemp = srcPtrHSLTemp + channel;
            }

            hsl2rgb_host(srcPtrHSL, srcSize, dstPtr, RPPI_CHN_PACKED, 3);
        }
    }
    /*
    else if (imageFormat == HSV)
    {
        Rpp8u *srcPtrRGB = (Rpp8u *)calloc(channel * srcSize.height * srcSize.width, sizeof(Rpp8u));
        if (chnFormat == RPPI_CHN_PLANAR)
        {
            hsv2rgb_host(srcPtr, srcSize, srcPtrRGB, RPPI_CHN_PLANAR, 3);
            
            //rgb2hsl_host(srcPtrRGB, srcSize, srcPtrHSL, RPPI_CHN_PLANAR, 3);

            Rpp32f *srcPtrHSLTemp;
            srcPtrHSLTemp = srcPtrHSL + (2 * srcSize.height * srcSize.width);

            for (int i = 0; i < srcSize.height * srcSize.width; i++)
            {
                if (*srcPtrHSLTemp < strength)
                {
                    *srcPtrHSLTemp *= 4;
                }
                if (*srcPtrHSLTemp > 1)
                {
                    *srcPtrHSLTemp = 1;
                }
                srcPtrHSLTemp++;
            }

            //hsl2rgb_host(srcPtrHSL, srcSize, srcPtrRGB, RPPI_CHN_PLANAR, 3);

            rgb2hsv_host(srcPtrRGB, srcSize, dstPtr, RPPI_CHN_PLANAR, 3);
        }
        else if (chnFormat == RPPI_CHN_PACKED)
        {
            hsv2rgb_host(srcPtr, srcSize, srcPtrRGB, RPPI_CHN_PACKED, 3);
            
            //rgb2hsl_host(srcPtrRGB, srcSize, srcPtrHSL, RPPI_CHN_PACKED, 3);

            Rpp32f *srcPtrHSLTemp;
            srcPtrHSLTemp = srcPtrHSL + 2;

            for (int i = 0; i < srcSize.height * srcSize.width; i++)
            {
                if (*srcPtrHSLTemp < strength)
                {
                    *srcPtrHSLTemp *= 4;
                }
                if (*srcPtrHSLTemp > 1)
                {
                    *srcPtrHSLTemp = 1;
                }
                srcPtrHSLTemp = srcPtrHSLTemp + channel;
            }

            //hsl2rgb_host(srcPtrHSL, srcSize, srcPtrRGB, RPPI_CHN_PACKED, 3);

            rgb2hsv_host(srcPtrRGB, srcSize, dstPtr, RPPI_CHN_PACKED, 3);
        }
    }
    */
    else
    {
        return RPP_ERROR;
    }
    return RPP_SUCCESS;
}