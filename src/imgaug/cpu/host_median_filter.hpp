#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus median_filter_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    unsigned int kernelSize,
                    RppiChnFormat chnFormat, unsigned int channel)
{
    if (kernelSize % 2 == 0)
    {
        return RPP_ERROR;
    }
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        dstPtr[i] = srcPtr[i];
    }
    Rpp8u *kernel = (Rpp8u *)malloc(kernelSize * kernelSize * sizeof(Rpp8u));
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height - kernelSize + 2; i++)
            {
                for (int j = 0; j < srcSize.width - kernelSize + 2; j++)
                {
                    for (int m = 0; m < kernelSize; m++)
                    {
                        for (int n = 0; n < kernelSize; n++)
                        {
                            kernel[(m * kernelSize) + n] = srcPtr[(c * srcSize.height * srcSize.width) + ((i + m) * srcSize.width) + (j + n)];
                        }
                    }
                    std::sort(std::begin(kernel), std::end(kernel));
                    dstPtr[(c * srcSize.height * srcSize.width) + ((i + (kernelSize / 2)) * srcSize.width) + (j + (kernelSize / 2))] = kernel[((kernelSize / 2) * (kernelSize)) + (kernelSize / 2) + 1];
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
                    for (int m = 0; m < kernelSize; m++)
                    {
                        for (int n = 0; n < kernelSize; n++)
                        {
                            kernel[(m * kernelSize) + n] = srcPtr[c + (channel * (i + m) * dstSize.width) + (channel * (j + n))];
                        }
                    }
                    std::sort(std::begin(kernel), std::end(kernel));
                    dstPtr[c + (channel * (i + (kernelSize / 2)) * dstSize.width) + (channel * (j + (kernelSize / 2)))] = kernel[((kernelSize / 2) * (kernelSize)) + (kernelSize / 2) + 1];
                }
            }
        }
    }

    return RPP_SUCCESS;
}
