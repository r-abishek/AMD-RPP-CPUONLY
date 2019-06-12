#include <math.h>
#include "host_declarations.hpp"


RppStatus host_resize_output_size(RppiSize srcSize, RppiSize *dstSizePtr,
                                  Rpp32f percentage)
{
    if (percentage < 0)
    {
        return RPP_ERROR;
    }
    percentage /= 100;
    Rpp32f resize[6] = {0};
    resize[0] = percentage;
    resize[1] = 0;
    resize[2] = 0;
    resize[3] = 0;
    resize[4] = percentage;
    resize[5] = 0;

    float minX = 0, minY = 0, maxX = 0, maxY = 0;
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
            if (newi > maxX)
            {
                maxX = newi;
            }
            if (newj > maxY)
            {
                maxY = newj;
            }
        }
    }
    dstSizePtr->height = ((Rpp32s)maxX - (Rpp32s)minX) + 1;
    dstSizePtr->width = ((Rpp32s)maxY - (Rpp32s)minY) + 1;

    return RPP_SUCCESS;
}

template <typename T>
RppStatus host_resize(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           Rpp32f percentage,
                           RppiChnFormat chnFormat, unsigned int channel)
{
    if (percentage < 0)
    {
        return RPP_ERROR;
    }
    percentage /= 100;
    Rpp32f resize[6] = {0};
    resize[0] = percentage;
    resize[1] = 0;
    resize[2] = 0;
    resize[3] = 0;
    resize[4] = percentage;
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

        for (int c = 0; c < channel; c++)
        {
            for (int i = 1; i < dstSize.height - 1; i++)
            {
                for (int j = 1; j < dstSize.width - 1; j++)
                {
                    if (dstPtr[(c * dstSize.height * dstSize.width) + (i * dstSize.width) + j] == 0)
                    {
                        Rpp32f pixel;
                        pixel = 0.25 * (dstPtr[(c * dstSize.height * dstSize.width) + ((i - 1) * dstSize.width) + (j - 1)] +
                                        dstPtr[(c * dstSize.height * dstSize.width) + ((i - 1) * dstSize.width) + (j + 1)] +
                                        dstPtr[(c * dstSize.height * dstSize.width) + ((i + 1) * dstSize.width) + (j - 1)] +
                                        dstPtr[(c * dstSize.height * dstSize.width) + ((i + 1) * dstSize.width) + (j + 1)]);
                        dstPtr[(c * dstSize.height * dstSize.width) + (i * dstSize.width) + j] = (Rpp8u) pixel;
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

        for (int c = 0; c < channel; c++)
        {
            for (int i = 1; i < dstSize.height - 1; i++)
            {
                for (int j = 1; j < dstSize.width - 1; j++)
                {
                    if (dstPtr[c + (channel * i * dstSize.width) + (channel * j)] == 0)
                    {
                        Rpp32f pixel;
                        pixel = 0.25 * (dstPtr[c + (channel * (i - 1) * dstSize.width) + (channel * (j - 1))] +
                                        dstPtr[c + (channel * (i - 1) * dstSize.width) + (channel * (j + 1))] +
                                        dstPtr[c + (channel * (i + 1) * dstSize.width) + (channel * (j - 1))] +
                                        dstPtr[c + (channel * (i + 1) * dstSize.width) + (channel * (j + 1))]);
                        dstPtr[c + (channel * i * dstSize.width) + (channel * j)] = (Rpp8u) pixel;
                    }
                }
            }
        }
    }

    return RPP_SUCCESS;
}