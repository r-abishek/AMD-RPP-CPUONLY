#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus random_crop_letterbox_host(T* srcPtr, RppiSize srcSize, T* dstPtr, RppiSize dstSize,
                                     Rpp32u x1, Rpp32u y1, Rpp32u x2, Rpp32u y2, 
                                     RppiChnFormat chnFormat, Rpp32u channel)
{
    if ((RPPINRANGE(x1, 0, srcSize.width - 1) == 0) 
        || (RPPINRANGE(x2, 0, srcSize.width - 1) == 0) 
        || (RPPINRANGE(y1, 0, srcSize.height - 1) == 0) 
        || (RPPINRANGE(y2, 0, srcSize.height - 1) == 0))
    {
        return RPP_ERROR;
    }

    Rpp32u borderWidth = 3;

    RppiSize srcSizeSubImage;
    T* srcPtrSubImage;
    compute_subimage_location_host(srcPtr, &srcPtrSubImage, srcSize, &srcSizeSubImage, x1, y1, x2, y2, chnFormat, channel);

    RppiSize srcSizeSubImagePadded;
    srcSizeSubImagePadded.height = srcSizeSubImage.height + (2 * borderWidth);
    srcSizeSubImagePadded.width = srcSizeSubImage.width + (2 * borderWidth);
    if (dstSize.height < srcSizeSubImagePadded.height || dstSize.width < srcSizeSubImagePadded.width)
    {
        return RPP_ERROR;
    }

    T *srcPtrCrop = (T *)calloc(channel * srcSizeSubImage.height * srcSizeSubImage.width, sizeof(T));
    generate_crop_host(srcPtr, srcSize, srcPtrSubImage, srcSizeSubImage, srcPtrCrop, chnFormat, channel);

    T *srcPtrCropPadded = (T *)calloc(channel * srcSizeSubImagePadded.height * srcSizeSubImagePadded.width, sizeof(T));
    generate_evenly_padded_image_host(srcPtrCrop, srcSizeSubImage, srcPtrCropPadded, srcSizeSubImagePadded, chnFormat, channel);

    T *srcPtrCropPaddedTemp, *dstPtrTemp;
    srcPtrCropPaddedTemp = srcPtrCropPadded;
    dstPtrTemp = dstPtr;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            dstPtrTemp = dstPtr + (c * dstSize.height * dstSize.width);
            for (int i = 0; i < srcSizeSubImagePadded.height; i++)
            {
                for (int j = 0; j < srcSizeSubImagePadded.width; j++)
                {
                    *dstPtrTemp = *srcPtrCropPaddedTemp;
                    dstPtrTemp++;
                    srcPtrCropPaddedTemp++;
                }
                dstPtrTemp = dstPtrTemp + (dstSize.width - srcSizeSubImagePadded.width);
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int i = 0; i < srcSizeSubImagePadded.height; i++)
        {
            for (int j = 0; j < srcSizeSubImagePadded.width; j++)
            {
                for (int c = 0; c < channel; c++)
                {
                    *dstPtrTemp = *srcPtrCropPaddedTemp;
                    dstPtrTemp++;
                    srcPtrCropPaddedTemp++;
                }
            }
            dstPtrTemp = dstPtrTemp + (channel * (dstSize.width - srcSizeSubImagePadded.width));
        }
    }

    return RPP_SUCCESS;
    
}
/*
{
    if ((RPPINRANGE(x1, 0, srcSize.width - 1) == 0) 
        || (RPPINRANGE(x2, 0, srcSize.width - 1) == 0) 
        || (RPPINRANGE(y1, 0, srcSize.height - 1) == 0) 
        || (RPPINRANGE(y2, 0, srcSize.height - 1) == 0))
    {
        return RPP_ERROR;
    }

    RppiSize srcNewSize;
    int xDiff = (int) x2 - (int) x1;
    int yDiff = (int) y2 - (int) y1;
    srcNewSize.width = (Rpp32u) RPPABS(xDiff);
    srcNewSize.height = (Rpp32u) RPPABS(yDiff);
    
    T *srcNewPtr = (T *)calloc(channel * srcNewSize.height * srcNewSize.width, sizeof(T));
    T *srcPtrTemp, *srcNewPtrTemp;
    srcPtrTemp = srcPtr;
    srcNewPtrTemp = srcNewPtr;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            srcPtrTemp += (c * srcSize.height * srcSize.width);
            srcPtrTemp += ((RPPMIN2(y1, y2) * srcSize.width) + RPPMIN2(x1, x2));
            for (int i = RPPMIN2(y1, y2); i < RPPMAX2(y1, y2); i++)
            {
                for (int j = RPPMIN2(x1, x2); j < RPPMAX2(x1, x2); j++)
                {
                    *srcNewPtrTemp = *srcPtrTemp;
                    srcNewPtrTemp++;
                    srcPtrTemp++;
                }
                srcPtrTemp += (srcSize.width - srcNewSize.width);
            }
            srcPtrTemp = srcPtr;
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        Rpp32s elementsInRow = srcSize.width * channel;
        srcPtrTemp += (RPPMIN2(y1, y2) * elementsInRow) + (RPPMIN2(x1, x2) * channel);
        for (int i = RPPMIN2(y1, y2); i < RPPMAX2(y1, y2); i++)
        {
            for (int j = RPPMIN2(x1, x2); j < RPPMAX2(x1, x2); j++)
            {
                for (int c = 0; c < channel; c++)
                {
                    *srcNewPtrTemp = *srcPtrTemp;
                    srcNewPtrTemp++;
                    srcPtrTemp++;
                }
            }
            srcPtrTemp += ((srcSize.width - srcNewSize.width) * channel);
        }
    }

    generate_evenly_padded_image_host(srcNewPtr, srcNewSize, dstPtr, dstSize, chnFormat, channel);

    return RPP_SUCCESS;
    
}
*/