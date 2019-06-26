#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus median_filter_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    unsigned int kernelSize,
                    RppiChnFormat chnFormat, unsigned int channel)
{
    if (kernelSize % 2 == 0 || kernelSize > RPPMIN2(srcSize.width, srcSize.height))
    {
        return RPP_ERROR;
    }
    printf("\nkernelSize = %d", kernelSize);
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        dstPtr[i] = srcPtr[i];
    }
    Rpp8u *kernel = (Rpp8u *)calloc(kernelSize * kernelSize, sizeof(Rpp8u));
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int i = 0; i < srcSize.height - kernelSize + 2; i++)
            {
                for (int j = 0; j < srcSize.width - kernelSize + 2; j++)
                {
                    printf("\nKernel\n\n");
                    for (int m = 0; m < kernelSize; m++)
                    {
                        for (int n = 0; n < kernelSize; n++)
                        {
                            kernel[(m * kernelSize) + n] = srcPtr[(c * srcSize.height * srcSize.width) + ((i + m) * srcSize.width) + (j + n)];
                            printf("\n%d = %d", (m * kernelSize) + n, kernel[(m * kernelSize) + n]);
                        }
                    }
                    std::sort(kernel, kernel + (kernelSize * kernelSize));
                    for (int m = 0; m < kernelSize*kernelSize; m++)
                    {
                        printf("  %d ", kernel[m]);
                    }
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
                    printf("\nKernel\n\n");
                    for (int m = 0; m < kernelSize; m++)
                    {
                        for (int n = 0; n < kernelSize; n++)
                        {
                            kernel[(m * kernelSize) + n] = srcPtr[c + (channel * (i + m) * srcSize.width) + (channel * (j + n))];
                            printf("\n%d = %d", (m * kernelSize) + n, kernel[(m * kernelSize) + n]);
                        }
                    }
                    std::sort(kernel, kernel + (kernelSize * kernelSize));
                    for (int m = 0; m < kernelSize*kernelSize; m++)
                    {
                        printf("  %d ", kernel[m]);
                    }
                    dstPtr[c + (channel * (i + (kernelSize / 2)) * srcSize.width) + (channel * (j + (kernelSize / 2)))] = kernel[((kernelSize / 2) * (kernelSize)) + (kernelSize / 2) + 1];
                }
            }
        }
    }

    return RPP_SUCCESS;
}
