#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus rgb_to_hsl_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                    RppiChnFormat chnFormat, unsigned channel)
{
    T *srcPtrTempR, *srcPtrTempG, *srcPtrTempB;
    U *dstPtrTempH, *dstPtrTempS, *dstPtrTempL;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        srcPtrTempR = srcPtr;
        srcPtrTempG = srcPtr + (srcSize.height * srcSize.width);
        srcPtrTempB = srcPtr + (2 * srcSize.height * srcSize.width);
        dstPtrTempH = dstPtr;
        dstPtrTempS = dstPtr + (srcSize.height * srcSize.width);
        dstPtrTempL = dstPtr + (2 * srcSize.height * srcSize.width);

        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            Rpp32f rf, gf, bf, cmax, cmin, delta, divisor;
            rf = ((Rpp32f) *srcPtrTempR) / 255;
            gf = ((Rpp32f) *srcPtrTempG) / 255;
            bf = ((Rpp32f) *srcPtrTempB) / 255;
            cmax = ((rf > gf) && (rf > bf)) ? rf : ((gf > bf) ? gf : bf);
            cmin = ((rf < gf) && (rf < bf)) ? rf : ((gf < bf) ? gf : bf);
            divisor = cmax + cmin - 1;
            delta = cmax - cmin;

            if (delta == 0)
            {
                *dstPtrTempH = 0;
            }
            else if (cmax == rf)
            {
                *dstPtrTempH = round(60 * fmod(((gf - bf) / delta),6));
            }
            else if (cmax == gf)
            {
                *dstPtrTempH = round(60 * (((bf - rf) / delta) + 2));
            }
            else if (cmax == bf)
            {
                *dstPtrTempH = round(60 * (((rf - gf) / delta) + 4));
            }
            
            while (*dstPtrTempH > 360)
            {
                *dstPtrTempH = *dstPtrTempH - 360;
            }
            while (*dstPtrTempH < 0)
            {
                *dstPtrTempH = 360 + *dstPtrTempH;
            }

            if (delta == 0)
            {
                *dstPtrTempS = 0;
            }
            else
            {
                *dstPtrTempS = delta / (1 - RPPABS(divisor));
            }

            *dstPtrTempL = (cmax + cmin) / 2;

            srcPtrTempR++;
            srcPtrTempG++;
            srcPtrTempB++;
            dstPtrTempH++;
            dstPtrTempS++;
            dstPtrTempL++;

        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        srcPtrTempR = srcPtr;
        srcPtrTempG = srcPtr + 1;
        srcPtrTempB = srcPtr + 2;
        dstPtrTempH = dstPtr;
        dstPtrTempS = dstPtr + 1;
        dstPtrTempL = dstPtr + 2;

        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            Rpp32f rf, gf, bf, cmax, cmin, delta, divisor;
            rf = ((Rpp32f) *srcPtrTempR) / 255;
            gf = ((Rpp32f) *srcPtrTempG) / 255;
            bf = ((Rpp32f) *srcPtrTempB) / 255;
            cmax = ((rf > gf) && (rf > bf)) ? rf : ((gf > bf) ? gf : bf);
            cmin = ((rf < gf) && (rf < bf)) ? rf : ((gf < bf) ? gf : bf);
            divisor = cmax + cmin - 1;
            delta = cmax - cmin;

            if (delta == 0)
            {
                *dstPtrTempH = 0;
            }
            else if (cmax == rf)
            {
                *dstPtrTempH = round(60 * fmod(((gf - bf) / delta),6));
            }
            else if (cmax == gf)
            {
                *dstPtrTempH = round(60 * (((bf - rf) / delta) + 2));
            }
            else if (cmax == bf)
            {
                *dstPtrTempH = round(60 * (((rf - gf) / delta) + 4));
            }
            
            while (*dstPtrTempH > 360)
            {
                *dstPtrTempH = *dstPtrTempH - 360;
            }
            while (*dstPtrTempH < 0)
            {
                *dstPtrTempH = 360 + *dstPtrTempH;
            }

            if (delta == 0)
            {
                *dstPtrTempS = 0;
            }
            else
            {
                *dstPtrTempS = delta / (1 - RPPABS(divisor));
            }

            *dstPtrTempL = (cmax + cmin) / 2;

            srcPtrTempR += 3;
            srcPtrTempG += 3;
            srcPtrTempB += 3;
            dstPtrTempH += 3;
            dstPtrTempS += 3;
            dstPtrTempL += 3;

        }
    }

    return RPP_SUCCESS;
}