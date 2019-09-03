#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus tensor_convert_bit_depth_host(T* srcPtr, U* dstPtr, 
                                        Rpp32u conversionType, 
                                        Rpp32u tensorDimension, Rpp32u *tensorDimensionValues)
{
    Rpp32u *tensorDimensionValuesTemp;
    tensorDimensionValuesTemp = tensorDimensionValues;

    Rpp32u tensorSize = 1;
    for(int i = 0; i < tensorDimension; i++)
    {
        tensorSize *= *tensorDimensionValuesTemp;
        tensorDimensionValuesTemp++;
    }

    T *srcPtrTemp;
    U *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    if (conversionType == 1)
    {
        Rpp32s val = 128;
        for (int i = 0; i < tensorSize; i++)
        {
            *dstPtrTemp = (U) ((Rpp32s) *srcPtrTemp - val);
            srcPtrTemp++;
            dstPtrTemp++;
        }
    }
    else if (conversionType == 2)
    {
        Rpp32f multiplier = 65535/255;
        for (int i = 0; i < tensorSize; i++)
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
        for (int i = 0; i < tensorSize; i++)
        {
            *dstPtrTemp = (U) (((Rpp32f) *srcPtrTemp * multiplier) - val);
            srcPtrTemp++;
            dstPtrTemp++;
        }
    }

    return RPP_SUCCESS;

}