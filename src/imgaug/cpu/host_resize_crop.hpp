#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus resize_crop_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                           Rpp32u x1, Rpp32u y1, Rpp32u x2, Rpp32u y2,
                           RppiChnFormat chnFormat, unsigned int channel)
{
    RppiSize srcSizeSubImage;
    T *srcPtrSubImage;

    compute_subimage_location_host(srcPtr, &srcPtrSubImage, srcSize, &srcSizeSubImage, x1, y1, x2, y2, chnFormat, channel);

    T *srcPtrResize = (T*) calloc(channel * srcSizeSubImage.height * srcSizeSubImage.width, sizeof(T));

    generate_crop_host(srcPtr, srcSize, srcPtrSubImage, srcSizeSubImage, srcPtrResize, chnFormat, channel);

    resize_kernel_host(srcPtrResize, srcSizeSubImage, dstPtr, dstSize, chnFormat, channel);

    return RPP_SUCCESS;
    
}