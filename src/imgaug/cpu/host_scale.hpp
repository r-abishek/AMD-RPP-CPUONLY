#include <cpu/rpp_cpu_common.hpp>

RppStatus scale_output_size_host(RppiSize srcSize, RppiSize *dstSizePtr,
                                  Rpp32f percentage)
{
    if (percentage < 0)
    {
        return RPP_ERROR;
    }
    percentage /= 100;
    dstSizePtr->height = (Rpp32s) (percentage * (Rpp32f) srcSize.height);
    dstSizePtr->width = (Rpp32s) (percentage * (Rpp32f) srcSize.width);

    return RPP_SUCCESS;
}

template <typename T>
RppStatus scale_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           Rpp32f percentage,
                           RppiChnFormat chnFormat, unsigned int channel)
{
    if (percentage < 0)
    {
        return RPP_ERROR;
    }
    percentage /= 100;
    Rpp32f scale[6] = {0};
    scale[0] = percentage;
    scale[1] = 0;
    scale[2] = 0;
    scale[3] = 0;
    scale[4] = percentage;
    scale[5] = 0;

    float minX = 0, minY = 0;
    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            Rpp32f newi = 0, newj = 0;
            newi = (scale[0] * i) + (scale[1] * j) + (scale[2] * 1);
            newj = (scale[3] * i) + (scale[4] * j) + (scale[5] * 1);
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
                    int k = (Rpp32s)((scale[0] * i) + (scale[1] * j) + (scale[2] * 1));
                    int l = (Rpp32s)((scale[3] * i) + (scale[4] * j) + (scale[5] * 1));
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
                    int k = (Rpp32s)((scale[0] * i) + (scale[1] * j) + (scale[2] * 1));
                    int l = (Rpp32s)((scale[3] * i) + (scale[4] * j) + (scale[5] * 1));
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
                    int k = (Rpp32s)((scale[0] * i) + (scale[1] * j) + (scale[2] * 1));
                    int l = (Rpp32s)((scale[3] * i) + (scale[4] * j) + (scale[5] * 1));
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
                    int k = (Rpp32s)((scale[0] * i) + (scale[1] * j) + (scale[2] * 1));
                    int l = (Rpp32s)((scale[3] * i) + (scale[4] * j) + (scale[5] * 1));
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
    
    return RPP_SUCCESS;
}