#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus resizeCrop_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           Rpp32u x1, Rpp32u y1, Rpp32u x2, Rpp32u y2,
                           RppiChnFormat chnFormat, unsigned int channel)
{
    if (dstSize.height < 0 || dstSize.width < 0)
    {
        return RPP_ERROR;
    }
    if ((RPPINRANGE(x1, 0, srcSize.width - 1) == 0) || (RPPINRANGE(x2, 0, srcSize.width - 1) == 0) || (RPPINRANGE(y1, 0, srcSize.height - 1) == 0) || (RPPINRANGE(y2, 0, srcSize.height - 1) == 0))
    {
        return RPP_ERROR;
    }

    RppiSize srcNewSize;
    int xDiff = (int) x2 - (int) x1;
    int yDiff = (int) y2 - (int) y1;
    srcNewSize.width = (Rpp32u) RPPABS(xDiff);
    srcNewSize.height = (Rpp32u) RPPABS(yDiff);
    
    Rpp8u *srcNewPtr = (Rpp8u *)calloc(channel * srcNewSize.height * srcNewSize.width, sizeof(Rpp8u));

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = RPPMIN2(y1, y2), m = 0; i < RPPMAX2(y1, y2); i++, m++)
            {
                for (int j = RPPMIN2(x1, x2), n = 0; j < RPPMAX2(x1, x2); j++, n++)
                {
                    srcNewPtr[(c * srcNewSize.height * srcNewSize.width) + (m * srcNewSize.width) + n] = srcPtr[(c * srcSize.height * srcSize.width) + (i * srcSize.width) + j];
                }
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = RPPMIN2(y1, y2), m = 0; i < RPPMAX2(y1, y2); i++, m++)
            {
                for (int j = RPPMIN2(x1, x2), n = 0; j < RPPMAX2(x1, x2); j++, n++)
                {
                    srcNewPtr[c + (channel * m * srcNewSize.width) + (channel * n)] = srcPtr[c + (channel * i * srcSize.width) + (channel * j)];
                }
            }
        }
    }
    //printf("\nFound height = %d, Found width = %d", srcNewSize.height, srcNewSize.width);
    //printf("\nDestination height = %d, Destination width = %d", dstSize.height, dstSize.width);

    Rpp32f hRatio = (((Rpp32f) dstSize.height) / ((Rpp32f) srcNewSize.height));
    Rpp32f wRatio = (((Rpp32f) dstSize.width) / ((Rpp32f) srcNewSize.width));
    //printf("\nhRatio = %0.4f, wRatio = %0.4f", hRatio, wRatio);
    Rpp32f srcLocI, srcLocJ;
    /*
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
                    //printf("\nsrcLocI = %0.4f, srcLocJ = %0.4f", srcLocI, srcLocJ);
                    Rpp32s p = (Rpp32s) RPPFLOOR(srcLocI);
                    Rpp32s q = (Rpp32s) RPPCEIL(srcLocI);
                    Rpp32s r = (Rpp32s) RPPFLOOR(srcLocJ);
                    Rpp32s s = (Rpp32s) RPPCEIL(srcLocJ);
                    //printf("\np = %d, q = %d, r = %d, s = %d\n", p, q, r, s);

                    Rpp32f h = srcLocI - (Rpp32f) p;
                    Rpp32f w = srcLocJ - (Rpp32f) r;
                    //printf("\nh = %0.4f, w = %0.4f", h, w);

                    Rpp32f pixel1 = (Rpp32f) srcNewPtr[(c * srcNewSize.height * srcNewSize.width) + (p * srcNewSize.width) + r];
                    Rpp32f pixel2 = (Rpp32f) srcNewPtr[(c * srcNewSize.height * srcNewSize.width) + (p * srcNewSize.width) + s];
                    Rpp32f pixel3 = (Rpp32f) srcNewPtr[(c * srcNewSize.height * srcNewSize.width) + (q * srcNewSize.width) + r];
                    Rpp32f pixel4 = (Rpp32f) srcNewPtr[(c * srcNewSize.height * srcNewSize.width) + (q * srcNewSize.width) + s];
                    
                    Rpp32f pixel;
                    pixel = ((pixel1) * (1 - w) * (1 - h)) + ((pixel2) * (w) * (1 - h)) + ((pixel3) * (h) * (1 - w)) + ((pixel4) * (w) * (h));
                    pixel = (pixel < (Rpp32f) 255) ? pixel : ((Rpp32f) 255);
                    pixel = (pixel > (Rpp32f) 0) ? pixel : ((Rpp32f) 0);
                    dstPtr[(c * dstSize.height * dstSize.width) + (i * dstSize.width) + j] = (Rpp8u) round(pixel);
                    //return RPP_SUCCESS;
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
                    //printf("\nsrcLocI = %0.4f, srcLocJ = %0.4f", srcLocI, srcLocJ);
                    Rpp32s p = (Rpp32s) RPPFLOOR(srcLocI);
                    Rpp32s q = (Rpp32s) RPPCEIL(srcLocI);
                    Rpp32s r = (Rpp32s) RPPFLOOR(srcLocJ);
                    Rpp32s s = (Rpp32s) RPPCEIL(srcLocJ);
                    //printf("\np = %d, q = %d, r = %d, s = %d\n", p, q, r, s);

                    Rpp32f h = srcLocI - (Rpp32f) p;
                    Rpp32f w = srcLocJ - (Rpp32f) r;
                    //printf("\nh = %0.4f, w = %0.4f", h, w);

                    Rpp32f pixel1 = (Rpp32f) srcNewPtr[c + (channel * p * srcNewSize.width) + (channel * r)];
                    Rpp32f pixel2 = (Rpp32f) srcNewPtr[c + (channel * p * srcNewSize.width) + (channel * s)];
                    Rpp32f pixel3 = (Rpp32f) srcNewPtr[c + (channel * q * srcNewSize.width) + (channel * r)];
                    Rpp32f pixel4 = (Rpp32f) srcNewPtr[c + (channel * q * srcNewSize.width) + (channel * s)];
                    
                    Rpp32f pixel;
                    pixel = ((pixel1) * (1 - w) * (1 - h)) + ((pixel2) * (w) * (1 - h)) + ((pixel3) * (h) * (1 - w)) + ((pixel4) * (w) * (h));
                    pixel = (pixel < (Rpp32f) 255) ? pixel : ((Rpp32f) 255);
                    pixel = (pixel > (Rpp32f) 0) ? pixel : ((Rpp32f) 0);
                    dstPtr[c + (channel * i * dstSize.width) + (channel * j)] = (Rpp8u) round(pixel);
                    //return RPP_SUCCESS;
                }
            }
        }
    }
*/










    Rpp32f resize[6] = {0};
    resize[0] = (((Rpp32f) dstSize.height) / ((Rpp32f) srcNewSize.height));
    resize[1] = 0;
    resize[2] = 0;
    resize[3] = 0;
    resize[4] = (((Rpp32f) dstSize.width) / ((Rpp32f) srcNewSize.width));
    resize[5] = 0;

    float minX = 0, minY = 0;
    for (int i = 0; i < srcNewSize.height; i++)
    {
        for (int j = 0; j < srcNewSize.width; j++)
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
            for (int i = 0; i < srcNewSize.height; i++)
            {
                for (int j = 0; j < srcNewSize.width; j++)
                {
                    int k = (Rpp32s)((resize[0] * i) + (resize[1] * j) + (resize[2] * 1));
                    int l = (Rpp32s)((resize[3] * i) + (resize[4] * j) + (resize[5] * 1));
                    k -= (Rpp32s)minX;
                    l -= (Rpp32s)minY;
                    dstPtr[(c * dstSize.height * dstSize.width) + (k * dstSize.width) + l] = srcNewPtr[(c * srcNewSize.height * srcNewSize.width) + (i * srcNewSize.width) + j];
                }
            }
        }

        float w = ((float)dstSize.width - (float)srcNewSize.width) / ((float) (srcNewSize.width - 1));
        float h = ((float)dstSize.height - (float)srcNewSize.height) / ((float) (srcNewSize.height - 1));
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
            for (int i = 0; i < srcNewSize.height - 1; i++)
            {
                for (int j = 0; j < srcNewSize.width - 1; j++)
                {                    
                    int k = (Rpp32s)((resize[0] * i) + (resize[1] * j) + (resize[2] * 1));
                    int l = (Rpp32s)((resize[3] * i) + (resize[4] * j) + (resize[5] * 1));
                    k -= (Rpp32s)minX;
                    l -= (Rpp32s)minY;
                    
                    int pixel1 = (int) srcNewPtr[(c * srcNewSize.height * srcNewSize.width) + (i * srcNewSize.width) + j];
                    int pixel2 = (int) srcNewPtr[(c * srcNewSize.height * srcNewSize.width) + (i * srcNewSize.width) + (j + 1)];
                    int pixel3 = (int) srcNewPtr[(c * srcNewSize.height * srcNewSize.width) + ((i + 1) * srcNewSize.width) + j];
                    int pixel4 = (int) srcNewPtr[(c * srcNewSize.height * srcNewSize.width) + ((i + 1) * srcNewSize.width) + (j + 1)];
                    
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
            for (int i = 0; i < srcNewSize.height; i++)
            {
                for (int j = 0; j < srcNewSize.width; j++)
                {
                    int k = (Rpp32s)((resize[0] * i) + (resize[1] * j) + (resize[2] * 1));
                    int l = (Rpp32s)((resize[3] * i) + (resize[4] * j) + (resize[5] * 1));
                    k -= (Rpp32s)minX;
                    l -= (Rpp32s)minY;
                    dstPtr[c + (channel * k * dstSize.width) + (channel * l)] = srcNewPtr[c + (channel * i * srcNewSize.width) + (channel * j)];
                }
            }
        }

        float w = ((float)dstSize.width - (float)srcNewSize.width) / ((float) (srcNewSize.width - 1));
        float h = ((float)dstSize.height - (float)srcNewSize.height) / ((float) (srcNewSize.height - 1));
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
            for (int i = 0; i < srcNewSize.height - 1; i++)
            {
                for (int j = 0; j < srcNewSize.width - 1; j++)
                {                    
                    int k = (Rpp32s)((resize[0] * i) + (resize[1] * j) + (resize[2] * 1));
                    int l = (Rpp32s)((resize[3] * i) + (resize[4] * j) + (resize[5] * 1));
                    k -= (Rpp32s)minX;
                    l -= (Rpp32s)minY;

                    int pixel1 = (int) srcNewPtr[c + (channel * i * srcNewSize.width) + (channel * j)];
                    int pixel2 = (int) srcNewPtr[c + (channel * i * srcNewSize.width) + (channel * (j + 1))];
                    int pixel3 = (int) srcNewPtr[c + (channel * (i + 1) * srcNewSize.width) + (channel * j)];
                    int pixel4 = (int) srcNewPtr[c + (channel * (i + 1) * srcNewSize.width) + (channel * (j + 1))];
                    
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
   
    return RPP_SUCCESS;
    
}