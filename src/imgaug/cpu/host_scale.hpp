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
    Rpp32f srcLocI, srcLocJ;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (unsigned int i = 0; i < dstSize.height; i++)
            {
                for (unsigned int j = 0; j < dstSize.width; j++)
                {
                    srcLocI = ((Rpp32f) i) / percentage;
                    srcLocJ = ((Rpp32f) j) / percentage;
                    Rpp32s p = (Rpp32s) floor(srcLocI);
                    Rpp32s q = (Rpp32s) ceil(srcLocI);
                    Rpp32s r = (Rpp32s) floor(srcLocJ);
                    Rpp32s s = (Rpp32s) ceil(srcLocJ);

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
                    srcLocI = ((Rpp32f) i) / percentage;
                    srcLocJ = ((Rpp32f) j) / percentage;
                    Rpp32s p = (Rpp32s) floor(srcLocI);
                    Rpp32s q = (Rpp32s) ceil(srcLocI);
                    Rpp32s r = (Rpp32s) floor(srcLocJ);
                    Rpp32s s = (Rpp32s) ceil(srcLocJ);

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
    
    return RPP_SUCCESS;
}