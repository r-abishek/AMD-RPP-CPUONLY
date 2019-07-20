#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus hsl_to_rgb_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                    RppiChnFormat chnFormat, unsigned channel)
{
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            Rpp32f c, x, m, rf, gf, bf;
            c = (2 * srcPtr[i + (2 * srcSize.height * srcSize.width)]) - 1;
            c = (1 - RPPABS(c)) * srcPtr[i + (srcSize.height * srcSize.width)];
            x = c * (1 - abs((fmod((srcPtr[i] / 60), 2)) - 1));
            m = srcPtr[i + (2 * srcSize.height * srcSize.width)] - c / 2;
            
            if ((0 <= srcPtr[i]) && (srcPtr[i] < 60))
            {
                rf = c;
                gf = x;
                bf = 0;
            }
            else if ((60 <= srcPtr[i]) && (srcPtr[i] < 120))
            {
                rf = x;
                gf = c;
                bf = 0;
            }
            else if ((120 <= srcPtr[i]) && (srcPtr[i] < 180))
            {
                rf = 0;
                gf = c;
                bf = x;
            }
            else if ((180 <= srcPtr[i]) && (srcPtr[i] < 240))
            {
                rf = 0;
                gf = x;
                bf = c;
            }
            else if ((240 <= srcPtr[i]) && (srcPtr[i] < 300))
            {
                rf = x;
                gf = 0;
                bf = c;
            }
            else if ((300 <= srcPtr[i]) && (srcPtr[i] < 360))
            {
                rf = c;
                gf = 0;
                bf = x;
            }

            dstPtr[i] = (Rpp8u) round((rf + m) * 255);
            dstPtr[i + (srcSize.height * srcSize.width)] = (Rpp8u) round((gf + m) * 255);
            dstPtr[i + (2 * srcSize.height * srcSize.width)] = (Rpp8u) round((bf + m) * 255);
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        printf("\nInside\n");
        for (int i = 0; i < (3 * srcSize.height * srcSize.width); i += 3)
        {
            Rpp32f c, x, m, rf, gf, bf;
            c = (2 * srcPtr[i + 2]) - 1;
            c = (1 - RPPABS(c)) * srcPtr[i + 1];
            x = c * (1 - abs((fmod((srcPtr[i] / 60), 2)) - 1));
            m = srcPtr[i + 2] - c / 2;
            
            if ((0 <= srcPtr[i]) && (srcPtr[i] < 60))
            {
                rf = c;
                gf = x;
                bf = 0;
            }
            else if ((60 <= srcPtr[i]) && (srcPtr[i] < 120))
            {
                rf = x;
                gf = c;
                bf = 0;
            }
            else if ((120 <= srcPtr[i]) && (srcPtr[i] < 180))
            {
                rf = 0;
                gf = c;
                bf = x;
            }
            else if ((180 <= srcPtr[i]) && (srcPtr[i] < 240))
            {
                rf = 0;
                gf = x;
                bf = c;
            }
            else if ((240 <= srcPtr[i]) && (srcPtr[i] < 300))
            {
                rf = x;
                gf = 0;
                bf = c;
            }
            else if ((300 <= srcPtr[i]) && (srcPtr[i] < 360))
            {
                rf = c;
                gf = 0;
                bf = x;
            }

            dstPtr[i] = (Rpp8u) round((rf + m) * 255);
            dstPtr[i + 1] = (Rpp8u) round((gf + m) * 255);
            dstPtr[i + 2] = (Rpp8u) round((bf + m) * 255);
        }
    }

    return RPP_SUCCESS;
}