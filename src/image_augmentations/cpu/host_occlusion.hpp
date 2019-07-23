#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus occlusion_host(T* srcPtr1, T* srcPtr2, RppiSize srcSize1, RppiSize srcSize2, T* dstPtr, 
                            Rpp32u src1x1, Rpp32u src1y1, Rpp32u src1x2, Rpp32u src1y2, 
                            Rpp32u src2x1, Rpp32u src2y1, Rpp32u src2x2, Rpp32u src2y2, 
                            RppiChnFormat chnFormat, Rpp32u channel)
{
    RppiSize srcSize1SubImage, srcSize2SubImage, dstSizeSubImage;
    T *srcPtr1SubImage, *srcPtr2SubImage, *dstPtrSubImage;

    compute_subimage_location_host(srcPtr2, &srcPtr2SubImage, srcSize2, &srcSize2SubImage, src2x1, src2y1, src2x2, src2y2, chnFormat, channel);

    T *srcPtrResize = (T*) calloc(channel * srcSize2SubImage.height * srcSize2SubImage.width, sizeof(T));

    generate_crop_host(srcPtr2, srcSize2, srcPtr2SubImage, srcSize2SubImage, srcPtrResize, chnFormat, channel);

    compute_subimage_location_host(srcPtr1, &srcPtr1SubImage, srcSize1, &srcSize1SubImage, src1x1, src1y1, src1x2, src1y2, chnFormat, channel);

    T *dstPtrResize = (T*) calloc(channel * srcSize1SubImage.height * srcSize1SubImage.width, sizeof(T));
    
    resize_kernel_host(srcPtrResize, srcSize2SubImage, dstPtrResize, srcSize1SubImage, chnFormat, channel);

    compute_subimage_location_host(dstPtr, &dstPtrSubImage, srcSize1, &dstSizeSubImage, src1x1, src1y1, src1x2, src1y2, chnFormat, channel);

    T *srcPtr1Temp, *dstPtrTemp;
    srcPtr1Temp = srcPtr1;
    dstPtrTemp = dstPtr;

    for (int i = 0; i < (channel * srcSize1.height * srcSize1.width); i++)
    {
        *dstPtrTemp = *srcPtr1Temp;
        srcPtr1Temp++;
        dstPtrTemp++;
    }

    T *dstPtrResizeTemp, *dstPtrSubImageTemp;
    dstPtrResizeTemp = dstPtrResize;
    dstPtrSubImageTemp = dstPtrSubImage;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        Rpp32u remainingElementsInRow = srcSize1.width - dstSizeSubImage.width;
        for (int c = 0;  c < channel; c++)
        {
            dstPtrSubImageTemp = dstPtrSubImage + (c * srcSize1.height * srcSize1.width);
            for (int i = 0; i < srcSize1SubImage.height; i++)
            {
                for (int j = 0; j < srcSize1SubImage.width; j++)
                {
                    *dstPtrSubImageTemp = *dstPtrResizeTemp;
                    dstPtrResizeTemp++;
                    dstPtrSubImageTemp++;
                }
                dstPtrSubImageTemp += remainingElementsInRow;
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        Rpp32u remainingElementsInRow = channel * (srcSize1.width - dstSizeSubImage.width);
        for (int i = 0; i < srcSize1SubImage.height; i++)
        {
            for (int j = 0; j < srcSize1SubImage.width; j++)
            {
                for (int c = 0;  c < channel; c++)
                {
                    *dstPtrSubImageTemp = *dstPtrResizeTemp;
                    dstPtrResizeTemp++;
                    dstPtrSubImageTemp++;
                }
            }
            dstPtrSubImageTemp += remainingElementsInRow;
        }
    }
  
    return RPP_SUCCESS;
}
