#include <cpu/rpp_cpu_common.hpp>
#include <string.h>

template <typename T>
RppStatus equalize_histogram_perChannel_host(T* srcPtr, RppiSize srcSize, T* dstPtr, 
                         RppiChnFormat chnFormat, unsigned int channel)
{
    T *srcPtrChannelBegin, *dstPtrChannelBegin;
    srcPtrChannelBegin = srcPtr;
    dstPtrChannelBegin = dstPtr;
    Rpp32u bins = (Rpp32u)(std::numeric_limits<unsigned char>::max()) + 1;
    Rpp32u *histogram = (Rpp32u *) calloc(bins, sizeof(Rpp32u));
    T *lookUpTable = (T *) calloc (bins, sizeof(T));
    Rpp32f multiplier = 255.0 / ((Rpp32f)(srcSize.height * srcSize.width));

    for (int c = 0; c < channel; c++)
    {
        //memset (histogram,0,bins * sizeof(Rpp32u));
        //memset (lookUpTable,0,bins * sizeof(T));

        Rpp32u sum = 0;
        Rpp32u *histogramTemp;
        T *lookUpTableTemp;
        histogramTemp = histogram;
        lookUpTableTemp = lookUpTable;

        for (int i = 0; i < bins; i++)
        {
            *histogramTemp = (Rpp32u) 0;
        }
        for (int i = 0; i < bins; i++)
        {
            *lookUpTableTemp = (T) 0;
        }

        histogramTemp = histogram;
        lookUpTableTemp = lookUpTable;

        histogram_kernel_perChannel_host(srcPtrChannelBegin, srcSize, histogram, bins, 0, chnFormat, channel);
        
        for (int i = 0; i < bins; i++)
        {
            sum += *histogramTemp;
            *lookUpTableTemp = (T)round(((Rpp32f)sum) * multiplier);
            histogramTemp++;
            lookUpTableTemp++;
        }

        T *srcPtrTemp, *dstPtrTemp;
        srcPtrTemp = srcPtrChannelBegin;
        dstPtrTemp = dstPtrChannelBegin;
        
        if (chnFormat == RPPI_CHN_PLANAR)
        {
            for (int i = 0; i < (srcSize.height * srcSize.width); i++)
            {
                *dstPtrTemp = *(lookUpTable + *srcPtrTemp);
                srcPtrTemp++;
                dstPtrTemp++;
            }
            srcPtrChannelBegin += (srcSize.height * srcSize.width);
            dstPtrChannelBegin += (srcSize.height * srcSize.width);
        }
        else if (chnFormat == RPPI_CHN_PACKED)
        {
            for (int i = 0; i < (srcSize.height * srcSize.width); i++)
            {
                *dstPtrTemp = *(lookUpTable + *srcPtrTemp);
                srcPtrTemp += 3;
                dstPtrTemp += 3;
            }
            srcPtrChannelBegin++;
            dstPtrChannelBegin++;
        }
    }

    return RPP_SUCCESS;

}