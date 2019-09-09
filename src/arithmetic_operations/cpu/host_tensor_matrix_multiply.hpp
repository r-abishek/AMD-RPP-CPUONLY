#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus tensor_matrix_multiply_host(T* srcPtr1, T* srcPtr2, T* dstPtr, 
                          Rpp32u *tensorDimensionValues1, Rpp32u *tensorDimensionValues2)
{
    if (*(tensorDimensionValues1 + 1) != *tensorDimensionValues2)
    {
        return RPP_ERROR;
    }
    
    T *srcPtr1Temp, *srcPtr2Temp, *dstPtrRow, *dstPtrCol;

    Rpp32u outputCols = *(tensorDimensionValues2 + 1);
    Rpp32u pixel;

    srcPtr1Temp = srcPtr1;
    for (int i = 0; i < *tensorDimensionValues1; i++)
    {
        dstPtrRow = dstPtr + (i * outputCols);
        srcPtr2Temp = srcPtr2;
        for (int k = 0; k < *tensorDimensionValues2; k++)
        {
            dstPtrCol = dstPtrRow;
            for (int j = 0; j < outputCols; j++)
            {
                pixel = (Rpp32u) *dstPtrCol + ((Rpp32u) *srcPtr1Temp * (Rpp32u) *srcPtr2Temp);
                pixel = RPPPIXELCHECK(pixel);
                *dstPtrCol = (T) pixel;
                dstPtrCol++;
                srcPtr2Temp++;
            }
            srcPtr1Temp++;
        }
    }
    
    return RPP_SUCCESS;

}