#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus channel_combine_host(T* srcPtr1, T* srcPtr2, T* srcPtr3, RppiSize srcSize, T* dstPtr,
                               RppiChnFormat chnFormat, Rpp32u channel)
{

    T *srcPtr1Temp, *srcPtr2Temp, *srcPtr3Temp, *dstPtrTemp;
    srcPtr1Temp = srcPtr1;
    srcPtr2Temp = srcPtr2;
    srcPtr3Temp = srcPtr3;
    dstPtrTemp = dstPtr;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        Rpp32u increment = srcSize.height * srcSize.width;
        memcpy(dstPtrTemp, srcPtr1Temp, srcSize.height * srcSize.width * sizeof(T));
        dstPtrTemp += increment;
        memcpy(dstPtrTemp, srcPtr2Temp, srcSize.height * srcSize.width * sizeof(T));
        dstPtrTemp += increment;
        memcpy(dstPtrTemp, srcPtr3Temp, srcSize.height * srcSize.width * sizeof(T));
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for (int i = 0; i < srcSize.height * srcSize.width; i++)
        {
            *dstPtrTemp = *srcPtr1Temp;
            dstPtrTemp++;
            srcPtr1Temp++;
            *dstPtrTemp = *srcPtr2Temp;
            dstPtrTemp++;
            srcPtr2Temp++;
            *dstPtrTemp = *srcPtr3Temp;
            dstPtrTemp++;
            srcPtr3Temp++;
        }
    }

    return RPP_SUCCESS;
}
