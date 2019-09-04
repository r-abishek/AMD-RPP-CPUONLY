#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus tensor_look_up_table_host(T* srcPtr, T* dstPtr, T* lutPtr, 
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

    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    for (int i = 0; i < tensorSize; i++)
    {
        *dstPtrTemp = *(lutPtr + (Rpp32u)(*srcPtrTemp));
        srcPtrTemp++;
        dstPtrTemp++;
    }

    return RPP_SUCCESS;

}