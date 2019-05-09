// rppi_rgb2hsv
// Parameters to functions are in the order inputs, input optional, outputs, outputs optional

// Uncomment the segment below to get this standalone to work for basic unit testing

#include <stdio.h>
#include <math.h>
#include <algorithm>
#include "rppdefs.h"
 
using namespace std;





RppStatus rppi_rgb2hsv_3C8U_pln_cpu(Rpp8u *pSrc, RppiSize size, Rpp32f *pDst)
{    
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
            pDst[i] = 0;
        }
        else if (cmax == rf)
        {
            pDst[i] = round(60 * fmod(((gf - bf) / delta),6));
        }
        else if (cmax == gf)
        {
            pDst[i] = round(60 * (((bf - rf) / delta) + 2));
        }
        else if (cmax == bf)
        {
            pDst[i] = round(60 * (((rf - gf) / delta) + 4));
        }

        if (cmax == 0)
        {
            pDst[i + (size.width * size.height)] = 0;
        }
        else
        {
            pDst[i + (size.width * size.height)] = delta / cmax;
        }

        pDst[i + (2 * size.width * size.height)] = cmax;

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

void displayf(Rpp32f *pArr, RppiSize size)
{
    int p = 0;
    for(int c = 0; c < size.channel; c++)
    {
        printf("\n\nChannel %d:\n", c+1);
        for (int i = 0; i < (size.width * size.height); i++)
        {
            printf("%0.2f\t\t", *(pArr + p));
            if (((i + 1) % size.width) == 0)
            {
                printf("\n");
            }
            p += 1;
        }
    }
}

int main()
{
    RppiSize size;
    printf("\nNumber of channels is fixed: 3\n");
    size.channel = 3;
    printf("Enter width of image in pixels: ");
    scanf("%d", &size.width);
    printf("Enter height of image in pixels: ");
    scanf("%d", &size.height);
    printf("Channels = %d, Width = %d, Height = %d", size.channel, size.width, size.height);

    Rpp8u *src = (Rpp8u *)malloc(size.channel * size.width * size.height * sizeof(Rpp8u));
    Rpp32f *dst = (Rpp32f *)malloc(size.channel * size.width * size.height * sizeof(Rpp32f));
    
    int *isrc = (int *)malloc(size.channel * size.width * size.height * sizeof(int));
    
    printf("\n\n\n\nEnter elements in array of size %d x %d x %d: \n", size.channel, size.width, size.height);
    input(isrc, size);

    cast(isrc, src, size);

    printf("\nInput:\n\n");
    display(src, size);

    rppi_rgb2hsv_3C8U_pln_cpu(src, size, dst);

    printf("\nOutput of RGB to HSV Modification:\n\n");
    displayf(dst, size);

}
