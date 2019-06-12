#include<math.h>

template <typename T>
RppStatus host_gamma_correction(T* srcPtr, RppiSize srcSize, T* dstPtr,
                                   Rpp32f gamma,
                                   unsigned int channel)
{
    for (int i = 0; i < (channel * srcSize.width * srcSize.height); i++)
    {
        Rpp32f pixel = ((Rpp32f) srcPtr[i]) / 255;
        pixel = pow(pixel, gamma);
        //pixel = (pixel < (Rpp32f) 255) ? pixel : ((Rpp32f) 255);
        //pixel = (pixel > (Rpp32f) 0) ? pixel : ((Rpp32f) 0);
        pixel *= 255;
        dstPtr[i] =(Rpp8u) pixel;
    }

    return RPP_SUCCESS;

}
