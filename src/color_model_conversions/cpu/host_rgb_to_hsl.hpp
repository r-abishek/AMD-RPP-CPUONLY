#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus rgb_to_hsl_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                    RppiChnFormat chnFormat, unsigned channel)
{
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            Rpp32f rf, gf, bf, cmax, cmin, delta, divisor;
            rf = ((Rpp32f) srcPtr[i]) / 255;
            gf = ((Rpp32f) srcPtr[i + (srcSize.height * srcSize.width)]) / 255;
            bf = ((Rpp32f) srcPtr[i + (2 * srcSize.height * srcSize.width)]) / 255;
            cmax = ((rf > gf) && (rf > bf)) ? rf : ((gf > bf) ? gf : bf);
            cmin = ((rf < gf) && (rf < bf)) ? rf : ((gf < bf) ? gf : bf);
            divisor = cmax + cmin - 1;
            delta = cmax - cmin;

            if (delta == 0)
            {
                dstPtr[i] = 0;
            }
            else if (cmax == rf)
            {
                dstPtr[i] = round(60 * fmod(((gf - bf) / delta),6));
            }
            else if (cmax == gf)
            {
                dstPtr[i] = round(60 * (((bf - rf) / delta) + 2));
            }
            else if (cmax == bf)
            {
                dstPtr[i] = round(60 * (((rf - gf) / delta) + 4));
            }
            
            while (dstPtr[i] > 360)
            {
                dstPtr[i] = dstPtr[i] - 360;
            }
            while (dstPtr[i] < 0)
            {
                dstPtr[i] = 360 + dstPtr[i];
            }

            if (delta == 0)
            {
                dstPtr[i + (srcSize.height * srcSize.width)] = 0;
            }
            else
            {
                dstPtr[i + (srcSize.height * srcSize.width)] = delta / (1 - RPPABS(divisor));
            }

            dstPtr[i + (2 * srcSize.height * srcSize.width)] = (cmax + cmin) / 2;

        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int i = 0; i < (3 * srcSize.height * srcSize.width); i += 3)
        {
            Rpp32f rf, gf, bf, cmax, cmin, delta, divisor;
            rf = ((Rpp32f) srcPtr[i]) / 255;
            gf = ((Rpp32f) srcPtr[i + 1]) / 255;
            bf = ((Rpp32f) srcPtr[i + 2]) / 255;
            cmax = ((rf > gf) && (rf > bf)) ? rf : ((gf > bf) ? gf : bf);
            cmin = ((rf < gf) && (rf < bf)) ? rf : ((gf < bf) ? gf : bf);
            divisor = cmax + cmin - 1;
            delta = cmax - cmin;

            if (delta == 0)
            {
                dstPtr[i] = 0;
            }
            else if (cmax == rf)
            {
                dstPtr[i] = round(60 * fmod(((gf - bf) / delta),6));
            }
            else if (cmax == gf)
            {
                dstPtr[i] = round(60 * (((bf - rf) / delta) + 2));
            }
            else if (cmax == bf)
            {
                dstPtr[i] = round(60 * (((rf - gf) / delta) + 4));
            }
            
            while (dstPtr[i] > 360)
            {
                dstPtr[i] = dstPtr[i] - 360;
            }
            while (dstPtr[i] < 0)
            {
                dstPtr[i] = 360 + dstPtr[i];
            }

            if (delta == 0)
            {
                dstPtr[i + 1] = 0;
            }
            else
            {
                dstPtr[i + 1] = delta / (1 - RPPABS(divisor));
            }

            dstPtr[i + 2] = (cmax + cmin) / 2;

        }
    }

    return RPP_SUCCESS;
}