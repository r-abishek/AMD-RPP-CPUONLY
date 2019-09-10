#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus tensor_transpose_host(T* srcPtr, T* dstPtr, Rpp32u dimension1, Rpp32u dimension2, 
                          Rpp32u tensorDimension, Rpp32u *tensorDimensionValues)
{
    Rpp32u *tensorDimensionValuesTemp;
    tensorDimensionValuesTemp = tensorDimensionValues;

    Rpp32u *tensorDimensionValuesProduct = (Rpp32u*) calloc(tensorDimension, sizeof(Rpp32u));
    Rpp32u *tensorDimensionValuesProductTemp;
    tensorDimensionValuesProductTemp = tensorDimensionValuesProduct;

    Rpp32u tensorSize = 1;
    for(int i = 0; i < tensorDimension; i++)
    {
        tensorSize *= *tensorDimensionValuesTemp;
        *tensorDimensionValuesProductTemp = tensorSize;
        tensorDimensionValuesTemp++;
        tensorDimensionValuesProductTemp++;
    }

    memcpy(dstPtr, srcPtr, tensorSize * sizeof(T));

    Rpp32u* loopCount = (Rpp32u *)calloc(tensorDimension, sizeof(Rpp32u));
    Rpp32u* loopCountTransposed = (Rpp32u *)calloc(tensorDimension, sizeof(Rpp32u));
    
    tensor_transpose_iterate_kernel_host(srcPtr, dstPtr, 
                                         0, tensorDimension, 
                                         tensorDimensionValues, tensorDimensionValuesProduct, 
                                         loopCount, loopCountTransposed, 
                                         dimension1, dimension2);
    
    return RPP_SUCCESS;

}