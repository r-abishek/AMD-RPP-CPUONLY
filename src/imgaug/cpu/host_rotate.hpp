#include <cpu/rpp_cpu_common.hpp>

RppStatus host_rotate_output_size(RppiSize srcSize, RppiSize *dstSizePtr,
                                  Rpp32f angleDeg)
{
    Rpp32f angleRad = RAD(angleDeg);
    Rpp32f rotate[6] = {0};
    rotate[0] = cos(angleRad);
    rotate[1] = sin(angleRad);
    rotate[2] = 0;
    rotate[3] = -sin(angleRad);
    rotate[4] = cos(angleRad);
    rotate[5] = 0;

    float minX = 0, minY = 0, maxX = 0, maxY = 0;
    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            Rpp32f newi = 0, newj = 0;
            newi = (rotate[0] * i) + (rotate[1] * j) + (rotate[2] * 1);
            newj = (rotate[3] * i) + (rotate[4] * j) + (rotate[5] * 1);
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
RppStatus host_rotate(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           Rpp32f angleDeg,
                           RppiChnFormat chnFormat, unsigned int channel)
{
    Rpp32f angleRad = RAD(angleDeg);
    Rpp32f rotate[6] = {0};
    rotate[0] = cos(angleRad);
    rotate[1] = sin(angleRad);
    rotate[2] = 0;
    rotate[3] = -sin(angleRad);
    rotate[4] = cos(angleRad);
    rotate[5] = 0;

    float minX = 0, minY = 0;
    for (int i = 0; i < srcSize.height; i++)
    {
        for (int j = 0; j < srcSize.width; j++)
        {
            Rpp32f newi = 0, newj = 0;
            newi = (rotate[0] * i) + (rotate[1] * j) + (rotate[2] * 1);
            newj = (rotate[3] * i) + (rotate[4] * j) + (rotate[5] * 1);
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
                    int k = (Rpp32s)((rotate[0] * i) + (rotate[1] * j) + (rotate[2] * 1));
                    int l = (Rpp32s)((rotate[3] * i) + (rotate[4] * j) + (rotate[5] * 1));
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
                    int k = (Rpp32s)((rotate[0] * i) + (rotate[1] * j) + (rotate[2] * 1));
                    int l = (Rpp32s)((rotate[3] * i) + (rotate[4] * j) + (rotate[5] * 1));
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