#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus resize_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           RppiChnFormat chnFormat, unsigned int channel)
{
    if (dstSize.height < 0 || dstSize.width < 0)
    {
        return RPP_ERROR;
    }

    Rpp32f hRatio = (((Rpp32f) dstSize.height) / ((Rpp32f) srcSize.height));
    Rpp32f wRatio = (((Rpp32f) dstSize.width) / ((Rpp32f) srcSize.width));
    Rpp32f srcLocI, srcLocJ;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (unsigned int i = 0; i < dstSize.height; i++)
            {
                for (unsigned int j = 0; j < dstSize.width; j++)
                {
                    srcLocI = ((Rpp32f) i) / hRatio;
                    srcLocJ = ((Rpp32f) j) / wRatio;
                    Rpp32s p = (Rpp32s) RPPFLOOR(srcLocI);
                    Rpp32s q = (Rpp32s) RPPCEIL(srcLocI);
                    Rpp32s r = (Rpp32s) RPPFLOOR(srcLocJ);
                    Rpp32s s = (Rpp32s) RPPCEIL(srcLocJ);

                    Rpp32f h = srcLocI - (Rpp32f) p;
                    Rpp32f w = srcLocJ - (Rpp32f) r;

                    Rpp32f pixel1 = (Rpp32f) srcPtr[(c * srcSize.height * srcSize.width) + (p * srcSize.width) + r];
                    Rpp32f pixel2 = (Rpp32f) srcPtr[(c * srcSize.height * srcSize.width) + (p * srcSize.width) + s];
                    Rpp32f pixel3 = (Rpp32f) srcPtr[(c * srcSize.height * srcSize.width) + (q * srcSize.width) + r];
                    Rpp32f pixel4 = (Rpp32f) srcPtr[(c * srcSize.height * srcSize.width) + (q * srcSize.width) + s];
                    
                    Rpp32f pixel;
                    pixel = ((pixel1) * (1 - w) * (1 - h)) + ((pixel2) * (w) * (1 - h)) + ((pixel3) * (h) * (1 - w)) + ((pixel4) * (w) * (h));
                    pixel = (pixel < (Rpp32f) 255) ? pixel : ((Rpp32f) 255);
                    pixel = (pixel > (Rpp32f) 0) ? pixel : ((Rpp32f) 0);
                    dstPtr[(c * dstSize.height * dstSize.width) + (i * dstSize.width) + j] = (Rpp8u) round(pixel);
                }
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int c = 0; c < channel; c++)
        {
            for (unsigned int i = 0; i < dstSize.height; i++)
            {
                for (unsigned int j = 0; j < dstSize.width; j++)
                {
                    srcLocI = ((Rpp32f) i) / hRatio;
                    srcLocJ = ((Rpp32f) j) / wRatio;
                    Rpp32s p = (Rpp32s) RPPFLOOR(srcLocI);
                    Rpp32s q = (Rpp32s) RPPCEIL(srcLocI);
                    Rpp32s r = (Rpp32s) RPPFLOOR(srcLocJ);
                    Rpp32s s = (Rpp32s) RPPCEIL(srcLocJ);

                    Rpp32f h = srcLocI - (Rpp32f) p;
                    Rpp32f w = srcLocJ - (Rpp32f) r;

                    Rpp32f pixel1 = (Rpp32f) srcPtr[c + (channel * p * srcSize.width) + (channel * r)];
                    Rpp32f pixel2 = (Rpp32f) srcPtr[c + (channel * p * srcSize.width) + (channel * s)];
                    Rpp32f pixel3 = (Rpp32f) srcPtr[c + (channel * q * srcSize.width) + (channel * r)];
                    Rpp32f pixel4 = (Rpp32f) srcPtr[c + (channel * q * srcSize.width) + (channel * s)];
                    
                    Rpp32f pixel;
                    pixel = ((pixel1) * (1 - w) * (1 - h)) + ((pixel2) * (w) * (1 - h)) + ((pixel3) * (h) * (1 - w)) + ((pixel4) * (w) * (h));
                    pixel = (pixel < (Rpp32f) 255) ? pixel : ((Rpp32f) 255);
                    pixel = (pixel > (Rpp32f) 0) ? pixel : ((Rpp32f) 0);
                    dstPtr[c + (channel * i * dstSize.width) + (channel * j)] = (Rpp8u) round(pixel);
                }
            }
        }
    }


















/*
    Rpp32f resize[6] = {0};
    resize[0] = (((Rpp32f) dstSize.height) / ((Rpp32f) srcSize.height));
    resize[1] = 0;
    resize[2] = 0;
    resize[3] = 0;
    resize[4] = (((Rpp32f) dstSize.width) / ((Rpp32f) srcSize.width));
    resize[5] = 0;

    float minX = 0, minY = 0;
    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            Rpp32f newi = 0, newj = 0;
            newi = (resize[0] * i) + (resize[1] * j) + (resize[2] * 1);
            newj = (resize[3] * i) + (resize[4] * j) + (resize[5] * 1);
            if (newi < minX)
            {
                minX = newi;
            }
            if (newj < minY)
            {
                minY = newj;
            }
        }
    }

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    int k = (Rpp32s)((resize[0] * i) + (resize[1] * j) + (resize[2] * 1));
                    int l = (Rpp32s)((resize[3] * i) + (resize[4] * j) + (resize[5] * 1));
                    k -= (Rpp32s)minX;
                    l -= (Rpp32s)minY;
                    dstPtr[(c * dstSize.height * dstSize.width) + (k * dstSize.width) + l] = srcPtr[(c * srcSize.height * srcSize.width) + (i * srcSize.width) + j];
                }
            }
        }

        float w = ((float)dstSize.width - (float)srcSize.width) / ((float) (srcSize.width - 1));
        float h = ((float)dstSize.height - (float)srcSize.height) / ((float) (srcSize.height - 1));
        if (w <= 0)
        {
            w = -1;
        }
        if (h <= 0)
        {
            h = -1;
        }

        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height - 1; i++)
            {
                for (int j = 0; j < srcSize.width - 1; j++)
                {                    
                    int k = (Rpp32s)((resize[0] * i) + (resize[1] * j) + (resize[2] * 1));
                    int l = (Rpp32s)((resize[3] * i) + (resize[4] * j) + (resize[5] * 1));
                    k -= (Rpp32s)minX;
                    l -= (Rpp32s)minY;
                    
                    int pixel1 = (int) srcPtr[(c * srcSize.height * srcSize.width) + (i * srcSize.width) + j];
                    int pixel2 = (int) srcPtr[(c * srcSize.height * srcSize.width) + (i * srcSize.width) + (j + 1)];
                    int pixel3 = (int) srcPtr[(c * srcSize.height * srcSize.width) + ((i + 1) * srcSize.width) + j];
                    int pixel4 = (int) srcPtr[(c * srcSize.height * srcSize.width) + ((i + 1) * srcSize.width) + (j + 1)];
                    
                    for (float m = 0; m < h + 2; m++)
                    {
                        for (float n = 0; n < w + 2; n++)
                        {
                            Rpp32f pixel;
                            pixel = ((pixel1) * (1 - n) * (1 - m)) + ((pixel2) * (n) * (1 - m)) + ((pixel3) * (m) * (1 - n)) + ((pixel4) * (m) * (n));
                            pixel = (pixel < (Rpp32f) 255) ? pixel : ((Rpp32f) 255);
                            pixel = (pixel > (Rpp32f) 0) ? pixel : ((Rpp32f) 0);
                            dstPtr[(c * dstSize.height * dstSize.width) + ((int)(k + m) * dstSize.width) + (int)(l + n)] = (Rpp8u) round(pixel);
                        }
                    }
                }
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    int k = (Rpp32s)((resize[0] * i) + (resize[1] * j) + (resize[2] * 1));
                    int l = (Rpp32s)((resize[3] * i) + (resize[4] * j) + (resize[5] * 1));
                    k -= (Rpp32s)minX;
                    l -= (Rpp32s)minY;
                    dstPtr[c + (channel * k * dstSize.width) + (channel * l)] = srcPtr[c + (channel * i * srcSize.width) + (channel * j)];
                }
            }
        }

        float w = ((float)dstSize.width - (float)srcSize.width) / ((float) (srcSize.width - 1));
        float h = ((float)dstSize.height - (float)srcSize.height) / ((float) (srcSize.height - 1));
        if (w <= 0)
        {
            w = -1;
        }
        if (h <= 0)
        {
            h = -1;
        }

        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height - 1; i++)
            {
                for (int j = 0; j < srcSize.width - 1; j++)
                {                    
                    int k = (Rpp32s)((resize[0] * i) + (resize[1] * j) + (resize[2] * 1));
                    int l = (Rpp32s)((resize[3] * i) + (resize[4] * j) + (resize[5] * 1));
                    k -= (Rpp32s)minX;
                    l -= (Rpp32s)minY;

                    int pixel1 = (int) srcPtr[c + (channel * i * srcSize.width) + (channel * j)];
                    int pixel2 = (int) srcPtr[c + (channel * i * srcSize.width) + (channel * (j + 1))];
                    int pixel3 = (int) srcPtr[c + (channel * (i + 1) * srcSize.width) + (channel * j)];
                    int pixel4 = (int) srcPtr[c + (channel * (i + 1) * srcSize.width) + (channel * (j + 1))];
                    
                    for (float m = 0; m <= h + 1; m++)
                    {
                        for (float n = 0; n <= w + 1; n++)
                        {
                            Rpp32f pixel;
                            pixel = ((pixel1) * (1 - n) * (1 - m)) + ((pixel2) * (n) * (1 - m)) + ((pixel3) * (m) * (1 - n)) + ((pixel4) * (m) * (n));
                            pixel = (pixel < (Rpp32f) 255) ? pixel : ((Rpp32f) 255);
                            pixel = (pixel > (Rpp32f) 0) ? pixel : ((Rpp32f) 0);
                            dstPtr[c + (channel * (int)(k + m) * dstSize.width) + (channel * (int)(l + n))] = (Rpp8u) round(pixel);
                        }
                    }
                }
            }
        }
    }
    */
    return RPP_SUCCESS;
}