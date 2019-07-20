#include <cpu/rpp_cpu_common.hpp>
#include <limits>

template <typename T>
RppStatus histogram_subimage_perChannel_host(T* srcPtr, RppiSize srcSize, Rpp32u* outputHistogram, Rpp32u bins, 
                                  Rpp32u x1, Rpp32u y1, Rpp32u x2, Rpp32u y2, 
                                  RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrSubImage;
    RppiSize srcSizeSubImage;
    compute_subimage_location_host(srcPtr, &srcPtrSubImage, srcSize, &srcSizeSubImage, x1, y1, x2, y2, chnFormat, channel);

    Rpp32u increment = srcSize.width - srcSizeSubImage.width;

    T *srcPtrTemp;
    srcPtrTemp = srcPtrSubImage;
    Rpp32u *histogram = (Rpp32u *) calloc(bins, sizeof(Rpp32u));
    Rpp32u *outputHistogramTemp;
    outputHistogramTemp = outputHistogram;
    for (int c = 0; c < channel; c++)
    {
        memset (histogram,0,bins * sizeof(Rpp32u));

        Rpp32u *histogramTemp;
        histogramTemp = histogram;

        histogram_kernel_perChannel_host(srcPtrTemp, srcSizeSubImage, histogram, bins, increment, chnFormat, channel);

        for (int i = 0; i < bins; i++)
        {
            *outputHistogramTemp = *histogramTemp;
            outputHistogramTemp++;
            histogramTemp++;
        }
        if (chnFormat == RPPI_CHN_PLANAR)
        {
            srcPtrTemp += (srcSize.height * srcSize.width);
        }
        else if (chnFormat == RPPI_CHN_PACKED)
        {
            srcPtrTemp += channel;
        }
    }

    return RPP_SUCCESS;

}