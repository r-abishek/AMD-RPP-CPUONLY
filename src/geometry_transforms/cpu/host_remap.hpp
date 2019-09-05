#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus remap_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                     Rpp32u* rowRemapTable, Rpp32u* colRemapTable, 
                     RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTemp, *dstPtrTemp;
    Rpp32u *rowRemapTableTemp, *colRemapTableTemp;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        dstPtrTemp = dstPtr;
        for (int c = 0; c < channel; c++)
        {
            rowRemapTableTemp = rowRemapTable;
            colRemapTableTemp = colRemapTable;
            srcPtrTemp = srcPtr + (c * srcSize.height * srcSize.width);
            for (int i = 0; i < srcSize.height; i++)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    *dstPtrTemp = *(srcPtrTemp + (*rowRemapTableTemp * srcSize.width) + *colRemapTableTemp);
                    dstPtrTemp++;
                    rowRemapTableTemp++;
                    colRemapTableTemp++;
                }
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        dstPtrTemp = dstPtr;
        rowRemapTableTemp = rowRemapTable;
        colRemapTableTemp = colRemapTable;
        Rpp32u elementsInRow = srcSize.width * channel;
        for (int i = 0; i < srcSize.height; i++)
        {
            for (int j = 0; j < srcSize.width; j++)
            {
                srcPtrTemp = srcPtr;
                for (int c = 0; c < channel; c++)
                {
                    *dstPtrTemp = *(srcPtrTemp + (*rowRemapTableTemp * elementsInRow) + (*colRemapTableTemp * channel));
                    dstPtrTemp++;
                    srcPtrTemp++;
                }
                rowRemapTableTemp++;
                colRemapTableTemp++;
            }
        }
    }
    
    return RPP_SUCCESS;
}