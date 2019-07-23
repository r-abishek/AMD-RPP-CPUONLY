#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus snowy_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                    Rpp32f strength,
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    if (strength < 0 || strength > 1)
    {
        return RPP_ERROR;
    }

    if (channel != 1 && channel != 3)
    {
        return RPP_ERROR;
    }

    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    T *srcPtrRGB = (T *)calloc(3 * srcSize.height * srcSize.width, sizeof(T));
    T *dstPtrRGB = (T *)calloc(3 * srcSize.height * srcSize.width, sizeof(T));
    T *srcPtrRGBTemp, *dstPtrRGBTemp;
    srcPtrRGBTemp = srcPtrRGB;
    dstPtrRGBTemp = dstPtrRGB;

    if (channel == 1)
    {
        if (chnFormat == RPPI_CHN_PLANAR)
        {
            for (int c = 0; c < 3; c++)
            {
                srcPtrTemp = srcPtr;
                for (int i = 0; i < (srcSize.height * srcSize.width); i++)
                {
                    *srcPtrRGBTemp = *srcPtrTemp;
                    srcPtrRGBTemp++;
                    srcPtrTemp++;
                }
            }
        }
        else if (chnFormat == RPPI_CHN_PACKED)
        {
            for (int i = 0; i < (srcSize.height * srcSize.width); i++)
            {
                for (int c = 0; c < 3; c++)
                {
                    *srcPtrRGBTemp = *srcPtrTemp;
                    srcPtrRGBTemp++;
                }
                srcPtrTemp++;
            }
        }
    }
    else if (channel == 3)
    {
        for (int i = 0; i < (3 * srcSize.height * srcSize.width); i++)
        {
            *srcPtrRGBTemp = *srcPtrTemp;
            srcPtrRGBTemp++;
            srcPtrTemp++;
        }
    }

    Rpp32f *srcPtrHSL = (Rpp32f *)calloc(3 * srcSize.height * srcSize.width, sizeof(Rpp32f));
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        rgb_to_hsl_host(srcPtrRGB, srcSize, srcPtrHSL, RPPI_CHN_PLANAR, 3);

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

        hsl_to_rgb_host(srcPtrHSL, srcSize, dstPtrRGB, RPPI_CHN_PLANAR, 3);
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        rgb_to_hsl_host(srcPtrRGB, srcSize, srcPtrHSL, RPPI_CHN_PACKED, 3);

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

        hsl_to_rgb_host(srcPtrHSL, srcSize, dstPtrRGB, RPPI_CHN_PACKED, 3);
    }

    if (channel == 1)
    {
        if (chnFormat == RPPI_CHN_PLANAR)
        {
            for (int i = 0; i < (srcSize.height * srcSize.width); i++)
            {
                *dstPtrTemp = *dstPtrRGBTemp;
                dstPtrRGBTemp++;
                dstPtrTemp++;
            }
        }
        else if (chnFormat == RPPI_CHN_PACKED)
        {
            for (int i = 0; i < (srcSize.height * srcSize.width); i++)
            {
                *dstPtrTemp = *dstPtrRGBTemp;
                dstPtrRGBTemp = dstPtrRGBTemp + 3;;
                dstPtrTemp++;
            }
        }
    }
    else if (channel == 3)
    {
        for (int i = 0; i < (3 * srcSize.height * srcSize.width); i++)
        {
            *dstPtrTemp = *dstPtrRGBTemp;
            dstPtrRGBTemp++;
            dstPtrTemp++;
        }
    }

    return RPP_SUCCESS;
}