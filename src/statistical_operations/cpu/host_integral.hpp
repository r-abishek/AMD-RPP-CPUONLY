#include <cpu/rpp_cpu_common.hpp>

template <typename T, typename U>
RppStatus integral_host(T* srcPtr, RppiSize srcSize, U* dstPtr, 
                        RppiChnFormat chnFormat, unsigned int channel)
{
    T *srcPtrTemp;
    U *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for (int c = 0; c < channel; c++)
        {
            U sum = 0;
            for (int i = 0; i < srcSize.height * srcSize.width; i++)
            {
                sum += (U) (*srcPtrTemp);
                *dstPtrTemp = sum;
                srcPtrTemp = srcPtrTemp + 1;
                dstPtrTemp = dstPtrTemp + 1;
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        U *sum = (U*)calloc(channel, sizeof(U));
        U *sumTemp;
        for (int i = 0; i < srcSize.height * srcSize.width; i++)
        {
            sumTemp = sum;
            for (int c = 0; c < channel; c++)
            {
                *sumTemp += (U) (*srcPtrTemp);
                *dstPtrTemp = *sumTemp;
                sumTemp = sumTemp + 1;
                srcPtrTemp = srcPtrTemp + 1;
                dstPtrTemp = dstPtrTemp + 1;
            }
        }
    }
    
    return RPP_SUCCESS;

}