#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus convert_bit_depth_host(T* srcPtr, RppiSize srcSize, U* dstPtr,
                                 Rpp32u conversionType, 
                                 RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTemp;
    U *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    if (conversionType == 1)
    {
        Rpp32s val = 128;
        for (int i = 0; i < srcSize.height * srcSize.width * channel; i++)
        {
            *dstPtrTemp = (U) ((Rpp32s) *srcPtrTemp - val);
            srcPtrTemp++;
            dstPtrTemp++;
        }
    }
    else if (conversionType == 2)
    {
        Rpp32f multiplier = 65535/255;
        for (int i = 0; i < srcSize.height * srcSize.width * channel; i++)
        {
            *dstPtrTemp = (U) ((Rpp32f) *srcPtrTemp * multiplier);
            srcPtrTemp++;
            dstPtrTemp++;
        }
    }
    else if (conversionType == 3)
    {
        Rpp32f multiplier = 65535/255;
        Rpp32f val = 32768;
        for (int i = 0; i < srcSize.height * srcSize.width * channel; i++)
        {
            *dstPtrTemp = (U) (((Rpp32f) *srcPtrTemp * multiplier) - val);
            srcPtrTemp++;
            dstPtrTemp++;
        }
    }
    //else if (conversionType == 4)
    //{
    //    Rpp64f multiplier = 4294967295/255;
    //    printf("\nmultiplier = %f", multiplier);
    //    Rpp64f val = 2147483648;
    //    printf("\nval = %f", val);
    //    for (int i = 0; i < srcSize.height * srcSize.width * channel; i++)
    //    {
    //        *dstPtrTemp = (U) (((Rpp64f) *srcPtrTemp * multiplier) - val);
    //        srcPtrTemp++;
    //        dstPtrTemp++;
    //    }
    //}
    
    return RPP_SUCCESS;
}
