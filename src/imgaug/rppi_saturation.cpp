// rppi_saturation
// Parameters to functions are in the order inputs, input optional, outputs, outputs optional

// Uncomment the segment below to get this standalone to work for basic unit testing

#include <stdio.h>
#include <math.h>
#include <algorithm>
#include "rppdefs.h"
 
using namespace std;





RppStatus rppi_saturation_3C8U_pln_cpu(Rpp8u *pSrc, RppiSize size, Rpp8u *pDst, Rpp32f saturationFactor = 1)
{
    Rpp32f *pHSV = (Rpp32f *)malloc(size.channel * size.width * size.height * sizeof(Rpp32f));
    for (int i = 0; i < (size.width * size.height); i++)
    {
        float rf, gf, bf, cmax, cmin, delta;
        rf = ((float) pSrc[i]) / 255;
        gf = ((float) pSrc[i + (size.width * size.height)]) / 255;
        bf = ((float) pSrc[i + (2 * size.width * size.height)]) / 255;
        cmax = std::max(std::max(rf, gf), bf);
        cmin = std::min(std::min(rf, gf), bf);
        delta = cmax - cmin;

        if (delta == 0)
        {
            pHSV[i] = 0;
        }
        else if (cmax == rf)
        {
            pHSV[i] = round(60 * fmod(((gf - bf) / delta),6));
        }
        else if (cmax == gf)
        {
            pHSV[i] = round(60 * (((bf - rf) / delta) + 2));
        }
        else if (cmax == bf)
        {
            pHSV[i] = round(60 * (((rf - gf) / delta) + 4));
        }
        
        while (pHSV[i] > 360)
        {
            pHSV[i] = pHSV[i] - 360;
        }
        while (pHSV[i] < 0)
        {
            pHSV[i] = 360 + pHSV[i];
        }

        if (cmax == 0)
        {
            pHSV[i + (size.width * size.height)] = 0;
        }
        else
        {
            pHSV[i + (size.width * size.height)] = delta / cmax;
        }

        pHSV[i + (2 * size.width * size.height)] = cmax;

    }
    
    for (int i = 0; i < (size.width * size.height); i++)
    {
        pHSV[i + (size.width * size.height)] *= saturationFactor;
        pHSV[i + (size.width * size.height)] = std::min(pHSV[i + (size.width * size.height)], (float) 1);
        pHSV[i + (size.width * size.height)] = std::max(pHSV[i + (size.width * size.height)], (float) 0);
    }

    for (int i = 0; i < (size.width * size.height); i++)
    {
        float c, x, m, rf, gf, bf;
        c = pHSV[i + (2 * size.width * size.height)] * pHSV[i + (size.width * size.height)];
        x = c * (1 - abs((fmod((pHSV[i] / 60), 2)) - 1));
        m = pHSV[i + (2 * size.width * size.height)] - c;
        
        if ((0 <= pHSV[i]) && (pHSV[i] < 60))
        {
            rf = c;
            gf = x;
            bf = 0;
        }
        else if ((60 <= pHSV[i]) && (pHSV[i] < 120))
        {
            rf = x;
            gf = c;
            bf = 0;
        }
        else if ((120 <= pHSV[i]) && (pHSV[i] < 180))
        {
            rf = 0;
            gf = c;
            bf = x;
        }
        else if ((180 <= pHSV[i]) && (pHSV[i] < 240))
        {
            rf = 0;
            gf = x;
            bf = c;
        }
        else if ((240 <= pHSV[i]) && (pHSV[i] < 300))
        {
            rf = x;
            gf = 0;
            bf = c;
        }
        else if ((300 <= pHSV[i]) && (pHSV[i] < 360))
        {
            rf = c;
            gf = 0;
            bf = x;
        }

        pDst[i] = (Rpp8u) round((rf + m) * 255);
        pDst[i + (size.width * size.height)] = (Rpp8u) round((gf + m) * 255);
        pDst[i + (2 * size.width * size.height)] = (Rpp8u) round((bf + m) * 255);
    }

    return RPP_SUCCESS;

}

RppStatus rppi_saturation_3C8U_pkd_cpu(Rpp8u *pSrc, RppiSize size, Rpp8u *pDst, Rpp32f saturationFactor = 1)
{
    Rpp32f *pHSV = (Rpp32f *)malloc(size.channel * size.width * size.height * sizeof(Rpp32f));
    for (int i = 0; i < (3 * size.width * size.height); i+=3)
    {
        float rf, gf, bf, cmax, cmin, delta;
        rf = ((float) pSrc[i]) / 255;
        gf = ((float) pSrc[i + 1]) / 255;
        bf = ((float) pSrc[i + 2]) / 255;
        cmax = std::max(std::max(rf, gf), bf);
        cmin = std::min(std::min(rf, gf), bf);
        delta = cmax - cmin;

        if (delta == 0)
        {
            pHSV[i] = 0;
        }
        else if (cmax == rf)
        {
            pHSV[i] = round(60 * fmod(((gf - bf) / delta),6));
        }
        else if (cmax == gf)
        {
            pHSV[i] = round(60 * (((bf - rf) / delta) + 2));
        }
        else if (cmax == bf)
        {
            pHSV[i] = round(60 * (((rf - gf) / delta) + 4));
        }
        
        while (pHSV[i] > 360)
        {
            pHSV[i] = pHSV[i] - 360;
        }
        while (pHSV[i] < 0)
        {
            pHSV[i] = 360 + pHSV[i];
        }

        if (cmax == 0)
        {
            pHSV[i + 1] = 0;
        }
        else
        {
            pHSV[i + 1] = delta / cmax;
        }

        pHSV[i + 2] = cmax;

    }
    
    for (int i = 0; i < (3 * size.width * size.height); i+=3)
    {
        pHSV[i + 1] *= saturationFactor;
        pHSV[i + 1] = std::min(pHSV[i + 1], (float) 1);
        pHSV[i + 1] = std::max(pHSV[i + 1], (float) 0);
    }

    for (int i = 0; i < (3 * size.width * size.height); i+=3)
    {
        float c, x, m, rf, gf, bf;
        c = pHSV[i + 2] * pHSV[i + 1];
        x = c * (1 - abs((fmod((pHSV[i] / 60), 2)) - 1));
        m = pHSV[i + 2] - c;
        
        if ((0 <= pHSV[i]) && (pHSV[i] < 60))
        {
            rf = c;
            gf = x;
            bf = 0;
        }
        else if ((60 <= pHSV[i]) && (pHSV[i] < 120))
        {
            rf = x;
            gf = c;
            bf = 0;
        }
        else if ((120 <= pHSV[i]) && (pHSV[i] < 180))
        {
            rf = 0;
            gf = c;
            bf = x;
        }
        else if ((180 <= pHSV[i]) && (pHSV[i] < 240))
        {
            rf = 0;
            gf = x;
            bf = c;
        }
        else if ((240 <= pHSV[i]) && (pHSV[i] < 300))
        {
            rf = x;
            gf = 0;
            bf = c;
        }
        else if ((300 <= pHSV[i]) && (pHSV[i] < 360))
        {
            rf = c;
            gf = 0;
            bf = x;
        }

        pDst[i] = (Rpp8u) round((rf + m) * 255);
        pDst[i + 1] = (Rpp8u) round((gf + m) * 255);
        pDst[i + 2] = (Rpp8u) round((bf + m) * 255);
    }

    return RPP_SUCCESS;

}





// Uncomment the segment below to get this standalone to work for basic unit testing

void input(int *isrc, RppiSize size)
{
    int p = 0;
    for(int c = 0; c < size.channel; c++)
    {
        printf("\n\nEnter %d elements for channel %d:\n", (size.width * size.height), c+1);
        for (int i = 0; i < (size.width * size.height); i++)
        {
            scanf("%d", isrc + p);
            p += 1;
        }
    }
}

void cast(int *isrc, Rpp8u *pSrc, RppiSize size)
{
    for (int i = 0; i < (size.channel * size.width * size.height); i++)
    {
        isrc[i] = std::min(isrc[i], 255);
        isrc[i] = std::max(isrc[i], 0);
        pSrc[i] = (Rpp8u) isrc[i];

    }
}

void display(Rpp8u *pArr, RppiSize size)
{
    int p = 0;
    for(int c = 0; c < size.channel; c++)
    {
        printf("\n\nChannel %d:\n", c+1);
        for (int i = 0; i < (size.width * size.height); i++)
        {
            printf("%d\t\t", *(pArr + p));
            if (((i + 1) % size.width) == 0)
            {
                printf("\n");
            }
            p += 1;
        }
    }
}

void displayPacked(Rpp8u *pArr, RppiSize size)
{
    int p = 0;
    for (int i = 0; i < (size.width * size.height); i++)
    {
        printf("%d,%d,%d\t\t", *(pArr + p), *(pArr + p + 1), *(pArr + p + 2));
        if (((i + 1) % size.width) == 0)
        {
            printf("\n");
        }
        p += 3;
    }
}

int main()
{
    RppiSize size;
    int type;
    printf("\nEnter 1 = planar, 2 = packed: ");
    scanf("%d", &type);
    printf("\nNumber of channels is fixed: 3\n");
    size.channel = 3;
    printf("Enter width of image in pixels: ");
    scanf("%d", &size.width);
    printf("Enter height of image in pixels: ");
    scanf("%d", &size.height);
    printf("Channels = %d, Width = %d, Height = %d", size.channel, size.width, size.height);

    Rpp8u *src = (Rpp8u *)malloc(size.channel * size.width * size.height * sizeof(Rpp8u));
    Rpp8u *dst = (Rpp8u *)malloc(size.channel * size.width * size.height * sizeof(Rpp8u));
    
    int *isrc = (int *)malloc(size.channel * size.width * size.height * sizeof(int));
    
    float saturationFactor = 0.3;
    
    printf("\n\n\n\nEnter elements in array of size %d x %d x %d: \n", size.channel, size.width, size.height);
    input(isrc, size);

    cast(isrc, src, size);

    if (type == 1)
    {
        printf("\nInput:\n\n");
        display(src, size);
        rppi_saturation_3C8U_pln_cpu(src, size, dst, saturationFactor);
        printf("\nOutput of Saturation Modification:\n\n");
        display(dst, size);
    }
    else if (type == 2)
    {
        printf("\nInput:\n\n");
        displayPacked(src, size);
        rppi_saturation_3C8U_pkd_cpu(src, size, dst, saturationFactor);
        printf("\nOutput of Hue Modification:\n\n");
        displayPacked(dst, size);
    }
}
