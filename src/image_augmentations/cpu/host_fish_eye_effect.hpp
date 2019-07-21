#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus fish_eye_effect_host(T* srcPtr, RppiSize srcSize, T* dstPtr,
                    RppiChnFormat chnFormat, Rpp32u channel)
{
    T *srcPtrTemp, *dstPtrTemp;
    srcPtrTemp = srcPtr;
    dstPtrTemp = dstPtr;

    Rpp32f newI, newJ, newIsrc, newJsrc, newIsquared, newJsquared, euclideanDistance, newEuclideanDistance, theta;
    int iSrc, jSrc, srcPosition;
    Rpp32u elementsPerChannel = srcSize.height * srcSize.width;
    Rpp32u elements = channel * elementsPerChannel;
    
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        for(int c = 0; c < channel; c++)
        {
            srcPtrTemp = srcPtr + (c * srcSize.height * srcSize.width);
            for(int i = 0; i < srcSize.height; i++)
            {
                newI = (((Rpp32f) (i * 2.0)) / ((Rpp32f)(srcSize.height))) - 1.0;
                newIsquared = newI * newI;
                for(int j = 0; j < srcSize.width; j++)
                {
                    newJ = (((Rpp32f) (j * 2.0)) / ((Rpp32f)(srcSize.width))) - 1.0;
                    newJsquared = newJ * newJ;
                    euclideanDistance = sqrt(newIsquared + newJsquared);
                    if (euclideanDistance >= 0 && euclideanDistance <= 1)
                    {
                        newEuclideanDistance = sqrt(1.0 - (euclideanDistance * euclideanDistance));
                        newEuclideanDistance = (euclideanDistance + (1.0 - newEuclideanDistance)) / 2.0;
                        if (newEuclideanDistance <= 1.0)
                        {
                            theta = atan2(newI, newJ);

                            newIsrc = newEuclideanDistance * sin(theta);
                            newJsrc = newEuclideanDistance * cos(theta);

                            iSrc = (int) (((newIsrc + 1.0) * ((Rpp32f) srcSize.height)) / 2.0);
                            jSrc = (int) (((newJsrc + 1.0) * ((Rpp32f) srcSize.width)) / 2.0);

                            srcPosition = (int)((iSrc * srcSize.width) + jSrc);

                            if ((srcPosition >= 0) && (srcPosition < elementsPerChannel))
                            {
                                *dstPtrTemp = *(srcPtrTemp + srcPosition);
                            }
                        }
                    }
                    dstPtrTemp++;
                }
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        for(int i = 0; i < srcSize.height; i++)
        {
            newI = (((Rpp32f) (i * 2.0)) / ((Rpp32f)(srcSize.height))) - 1.0;
            newIsquared = newI * newI;
            for(int j = 0; j < srcSize.width; j++)
            {
                for(int c = 0; c < channel; c++)
                {
                    srcPtrTemp = srcPtr + c;
                    newJ = (((Rpp32f) (j * 2.0)) / ((Rpp32f)(srcSize.width))) - 1.0;
                    newJsquared = newJ * newJ;
                    euclideanDistance = sqrt(newIsquared + newJsquared);
                    if (euclideanDistance >= 0 && euclideanDistance <= 1)
                    {
                        newEuclideanDistance = sqrt(1.0 - (euclideanDistance * euclideanDistance));
                        newEuclideanDistance = (euclideanDistance + (1.0 - newEuclideanDistance)) / 2.0;
                        if (newEuclideanDistance <= 1.0)
                        {
                            theta = atan2(newI, newJ);

                            newIsrc = newEuclideanDistance * sin(theta);
                            newJsrc = newEuclideanDistance * cos(theta);

                            iSrc = (int) (((newIsrc + 1.0) * ((Rpp32f) srcSize.height)) / 2.0);
                            jSrc = (int) (((newJsrc + 1.0) * ((Rpp32f) srcSize.width)) / 2.0);

                            srcPosition = (int)(channel * ((iSrc * srcSize.width) + jSrc));

                            if ((srcPosition >= 0) && (srcPosition < elements))
                            {
                                *dstPtrTemp = *(srcPtrTemp + srcPosition);
                            }
                        }
                    }
                    dstPtrTemp++;
                }
            }
        }
    }

    return RPP_SUCCESS;
}