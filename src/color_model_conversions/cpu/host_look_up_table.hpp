#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus look_up_table_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                    U *lutPtr,
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;
    U *lutPtrTemp;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            lutPtrTemp = lutPtr + (c * 256);
            for (int i = 0; i < (srcSize.height * srcSize.width); i++)
            {
                *dstPtrTemp = *(lutPtrTemp + (Rpp32u)(*srcPtrTemp));
                srcPtrTemp++;
                dstPtrTemp++;
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        lutPtrTemp = lutPtr;
        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            for (int c = 0; c < channel; c++)
            {
                *dstPtrTemp = *(lutPtrTemp + c + (channel * (Rpp32u)(*srcPtrTemp)));
                srcPtrTemp++;
                dstPtrTemp++;
            }
        }
    }
    
    return RPP_SUCCESS;
}