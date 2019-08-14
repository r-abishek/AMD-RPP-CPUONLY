#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus sobel_filter_host(T* srcPtr, RppiSize srcSize, T* dstPtr, 
                            Rpp32u sobelType, 
                            RppiChnFormat chnFormat, Rpp32u channel)
// Separable Kernel
/*
{
    RppiSize srcSizeMod, rppiKernelSize;
    Rpp32f *kernel = (Rpp32f *)calloc(3, sizeof(Rpp32f));
    T *srcPtrMod1 = (T *)calloc((srcSizeMod.height + 2) * srcSizeMod.width * channel, sizeof(T));
    T *srcPtrMod2 = (T *)calloc(srcSizeMod.height * (srcSizeMod.width + 2) * channel, sizeof(T));

    if (sobelType == 2)
    {
        T *dstPtrIntermediateX = (T *)calloc(channel * srcSize.height * srcSize.width, sizeof(T));
        T *dstPtrIntermediateY = (T *)calloc(channel * srcSize.height * srcSize.width, sizeof(T));

        srcSizeMod.height = srcSize.height + 2;
        srcSizeMod.width = srcSize.width;
        rppiKernelSize.height = 3;
        rppiKernelSize.width = 1;

        generate_sobel_kernel_host(kernel, 1);
        generate_opposite_edge_padded_image_host(srcPtr, srcSize, srcPtrMod1, srcSizeMod, 1, chnFormat, channel);
        convolve_image_host(srcPtrMod1, srcSizeMod, dstPtrIntermediateX, srcSize, kernel, rppiKernelSize, chnFormat, channel);
        
        srcSizeMod.height = srcSize.height;
        srcSizeMod.width = srcSize.width + 2;
        rppiKernelSize.height = 1;
        rppiKernelSize.width = 3;

        generate_sobel_kernel_host(kernel, 2);
        generate_opposite_edge_padded_image_host(dstPtrIntermediateX, srcSize, srcPtrMod2, srcSizeMod, 2, chnFormat, channel);
        convolve_image_host(srcPtrMod2, srcSizeMod, dstPtrIntermediateX, srcSize, kernel, rppiKernelSize, chnFormat, channel);

        srcSizeMod.height = srcSize.height + 2;
        srcSizeMod.width = srcSize.width;
        rppiKernelSize.height = 3;
        rppiKernelSize.width = 1;

        generate_sobel_kernel_host(kernel, 2);
        generate_opposite_edge_padded_image_host(srcPtr, srcSize, srcPtrMod1, srcSizeMod, 1, chnFormat, channel);
        convolve_image_host(srcPtrMod1, srcSizeMod, dstPtrIntermediateY, srcSize, kernel, rppiKernelSize, chnFormat, channel);
        
        srcSizeMod.height = srcSize.height;
        srcSizeMod.width = srcSize.width + 2;
        rppiKernelSize.height = 1;
        rppiKernelSize.width = 3;

        generate_sobel_kernel_host(kernel, 1);
        generate_opposite_edge_padded_image_host(dstPtrIntermediateY, srcSize, srcPtrMod2, srcSizeMod, 2, chnFormat, channel);
        convolve_image_host(srcPtrMod2, srcSizeMod, dstPtrIntermediateY, srcSize, kernel, rppiKernelSize, chnFormat, channel);

        compute_magnitude_host(dstPtrIntermediateX, dstPtrIntermediateY, srcSize, dstPtr, chnFormat, channel);
    }
    else
    {
        srcSizeMod.height = srcSize.height + 2;
        srcSizeMod.width = srcSize.width;
        rppiKernelSize.height = 3;
        rppiKernelSize.width = 1;

        if (sobelType == 0)
        {
            generate_sobel_kernel_host(kernel, 1);
        }
        else if (sobelType == 1)
        {
            generate_sobel_kernel_host(kernel, 2);
        }
        generate_opposite_edge_padded_image_host(srcPtr, srcSize, srcPtrMod1, srcSizeMod, 1, chnFormat, channel);
        convolve_image_host(srcPtrMod1, srcSizeMod, dstPtr, srcSize, kernel, rppiKernelSize, chnFormat, channel);
        
        srcSizeMod.height = srcSize.height;
        srcSizeMod.width = srcSize.width + 2;
        rppiKernelSize.height = 1;
        rppiKernelSize.width = 3;

        if (sobelType == 0)
        {
            generate_sobel_kernel_host(kernel, 2);
        }
        else if (sobelType == 1)
        {
            generate_sobel_kernel_host(kernel, 1);
        }
        generate_opposite_edge_padded_image_host(dstPtr, srcSize, srcPtrMod2, srcSizeMod, 2, chnFormat, channel);
        convolve_image_host(srcPtrMod2, srcSizeMod, dstPtr, srcSize, kernel, rppiKernelSize, chnFormat, channel);
    }
    return RPP_SUCCESS;
}
*/

// Separable Kernel - First Attempt
/*    
    RppiSize srcSizeMod;
    srcSizeMod.height = srcSize.height + 2;
    srcSizeMod.width = srcSize.width;

    T *srcPtrMod1 = (T *)calloc(srcSizeMod.height * srcSizeMod.width * channel, sizeof(T));
    generate_opposite_edge_padded_image_host(srcPtr, srcSize, srcPtrMod1, srcSizeMod, 1, chnFormat, channel);

    RppiSize rppiKernelSize;
    rppiKernelSize.height = 3;
    rppiKernelSize.width = 1;

    Rpp32f *kernel = (Rpp32f *)calloc(3, sizeof(Rpp32f));

    //T *dstPtrIntermediate = (T *)calloc(channel * srcSize.height * srcSize.width, sizeof(T));

    if (sobelType == 0)
    {
        generate_sobel_kernel_host(kernel, 1);
    }
    else if (sobelType == 1)
    {
        generate_sobel_kernel_host(kernel, 2);
    }
    convolve_image_host(srcPtrMod1, srcSizeMod, dstPtr, srcSize, kernel, rppiKernelSize, chnFormat, channel);

    srcSizeMod.height = srcSize.height;
    srcSizeMod.width = srcSize.width + 2;

    T *srcPtrMod2 = (T *)calloc(srcSizeMod.height * srcSizeMod.width * channel, sizeof(T));
    generate_opposite_edge_padded_image_host(dstPtr, srcSize, srcPtrMod2, srcSizeMod, 2, chnFormat, channel);

    rppiKernelSize.height = 1;
    rppiKernelSize.width = 3;

    if (sobelType == 0)
    {
        generate_sobel_kernel_host(kernel, 2);
    }
    else if (sobelType == 1)
    {
        generate_sobel_kernel_host(kernel, 1);
    }
    convolve_image_host(srcPtrMod2, srcSizeMod, dstPtr, srcSize, kernel, rppiKernelSize, chnFormat, channel);

    else if (sobelType == 2)
    {
        
    }

    return RPP_SUCCESS;
}
*/

// No Kernel Separation
///*
{
    RppiSize srcSizeMod;
    srcSizeMod.width = srcSize.width + 2;
    srcSizeMod.height = srcSize.height + 2;

    T *srcPtrMod = (T *)calloc(srcSizeMod.height * srcSizeMod.width * channel, sizeof(T));
    generate_evenly_padded_image_host(srcPtr, srcSize, srcPtrMod, srcSizeMod, chnFormat, channel);

    RppiSize rppiKernelSize;
    rppiKernelSize.height = 3;
    rppiKernelSize.width = 3;

    if (sobelType == 0)
    {
        Rpp32f *kernelX = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));
        generate_sobel_kernel_host(kernelX, 1);
        convolve_image_host(srcPtrMod, srcSizeMod, dstPtr, srcSize, kernelX, rppiKernelSize, chnFormat, channel);
    }
    else if (sobelType == 1)
    {
        Rpp32f *kernelY = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));
        generate_sobel_kernel_host(kernelY, 2);
        convolve_image_host(srcPtrMod, srcSizeMod, dstPtr, srcSize, kernelY, rppiKernelSize, chnFormat, channel);
    }
    else if (sobelType == 2)
    {
        Rpp32f *kernelX = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));
        generate_sobel_kernel_host(kernelX, 1);
        Rpp32s *dstPtrIntermediateX = (Rpp32s *)calloc(srcSize.height * srcSize.width * channel, sizeof(Rpp32s));
        convolve_image_host(srcPtrMod, srcSizeMod, dstPtrIntermediateX, srcSize, kernelX, rppiKernelSize, chnFormat, channel);

        Rpp32f *kernelY = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));
        generate_sobel_kernel_host(kernelY, 2);
        Rpp32s *dstPtrIntermediateY = (Rpp32s *)calloc(srcSize.height * srcSize.width * channel, sizeof(Rpp32s));
        convolve_image_host(srcPtrMod, srcSizeMod, dstPtrIntermediateY, srcSize, kernelY, rppiKernelSize, chnFormat, channel);

        compute_magnitude_host(dstPtrIntermediateX, dstPtrIntermediateY, srcSize, dstPtr, chnFormat, channel);
    }

    return RPP_SUCCESS;
}
//*/