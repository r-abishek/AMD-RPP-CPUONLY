#include <cpu/rpp_cpu_common.hpp>
#include <limits>

template <typename T>
RppStatus histogram_host(T* srcPtr, RppiSize srcSize, Rpp32u* outputHistogram, T* maskPtr, Rpp32u bins, 
                         RppiChnFormat chnFormat, unsigned int channel)
{
    T *srcPtrTemp, *maskPtrTemp;
    srcPtrTemp = srcPtr;
    maskPtrTemp = maskPtr;
    Rpp32u *outputHistogramTemp;
    outputHistogramTemp = outputHistogram;
    int flag = 0;

    Rpp32u elementsInBin = ((Rpp32u)(std::numeric_limits<unsigned char>::max()) + 1) / bins;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < (srcSize.height * srcSize.width); i++)
            {
                if (*maskPtrTemp != 0 && *maskPtrTemp != 1)
                {
                    return RPP_ERROR;
                }
                if (*maskPtrTemp == 0)
                {
                    srcPtrTemp++;
                    maskPtrTemp++;
                }
                else if (*maskPtrTemp == 1)
                {
                    flag = 0;
                    for (int binCheck = 0; binCheck < bins - 1; binCheck++)
                    {
                        if (*srcPtrTemp >= binCheck * elementsInBin && *srcPtrTemp <= ((binCheck + 1) * elementsInBin) - 1)
                        {
                            *(outputHistogramTemp + binCheck) += 1;
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 0)
                    {
                        *(outputHistogramTemp + bins - 1) += 1;
                    }
                    srcPtrTemp++;
                    maskPtrTemp++;
                }
            }
            outputHistogramTemp += bins;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            for (int c = 0; c < channel; c++)
            {
                if (*maskPtrTemp != 0 && *maskPtrTemp != 1)
                {
                    return RPP_ERROR;
                }
                if (*maskPtrTemp == 0)
                {
                    srcPtrTemp++;
                    maskPtrTemp++;
                }
                else if (*maskPtrTemp == 1)
                {
                    flag = 0;
                    for (int binCheck = 0; binCheck < bins - 1; binCheck++)
                    {
                        if (*srcPtrTemp >= binCheck * elementsInBin && *srcPtrTemp <= ((binCheck + 1) * elementsInBin) - 1)
                        {
                            *(outputHistogramTemp + (c * bins) + binCheck) += 1;
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 0)
                    {
                        *(outputHistogramTemp + (c * bins) + bins - 1) += 1;
                    }
                    srcPtrTemp++;
                    maskPtrTemp++;
                }
            }
        }
    }

    return RPP_SUCCESS;

}