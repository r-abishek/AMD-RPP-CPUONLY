// rppi_brightness
// Parameters to functions are in the order inputs, input optional, outputs, outputs optional

// Uncomment the segment below to get this standalone to work for basic unit testing
/*
#include <stdio.h>
#include <algorithm>
#include "rppdefs.h"
 
using namespace std;
*/
RppStatus rppi_brightness_1C8U_pln_cpu(Rpp8u *pSrc, RppiSize size, Rpp8u *pDst, Rpp32f alpha = 1, Rpp32f beta = 0)
{

    for (int i = 0; i < (size.width * size.height); i++)
    {
        Rpp32f pixel = ((Rpp32f) pSrc[i]) * alpha + beta;
        pixel = std::min(pixel, (Rpp32f) 255);
        pixel = std::max(pixel, (Rpp32f) 0);
        pDst[i] = (Rpp8u) pixel;
    }

    return RPP_SUCCESS;

}

RppStatus rppi_brightness_3C8U_pln_cpu(Rpp8u *pSrc, RppiSize size, Rpp8u *pDst, Rpp32f alpha = 1, Rpp32f beta = 0)
{

    for (int i = 0; i < (size.channel * size.width * size.height); i++)
    {
        Rpp32f pixel = ((Rpp32f) pSrc[i]) * alpha + beta;
        pixel = std::min(pixel, (Rpp32f) 255);
        pixel = std::max(pixel, (Rpp32f) 0);
        pDst[i] = (Rpp8u) pixel;
    }

    return RPP_SUCCESS;

}

// Uncomment the segment below to get this standalone to work for basic unit testing
/*
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

int main()
{
    RppiSize size;
    printf("\nEnter number of channels: ");
    scanf("%d", &size.channel);
    printf("Enter width of image in pixels: ");
    scanf("%d", &size.width);
    printf("Enter height of image in pixels: ");
    scanf("%d", &size.height);
    printf("Channels = %d, Width = %d, Height = %d", size.channel, size.width, size.height);

    Rpp8u *src = (Rpp8u *)malloc(size.channel * size.width * size.height * sizeof(Rpp8u));
    Rpp8u *dst = (Rpp8u *)malloc(size.channel * size.width * size.height * sizeof(Rpp8u));
    
    int *isrc = (int *)malloc(size.channel * size.width * size.height * sizeof(int));

    Rpp32f alpha = 2, beta = 5;
    
    printf("\n\n\n\nEnter elements in array of size %d x %d x %d: \n", size.channel, size.width, size.height);
    input(isrc, size);

    cast(isrc, src, size);

    printf("\nInput:\n\n");
    display(src, size);

    if (size.channel == 1)
    {
        rppi_brightness_1C8U_pln_cpu(src, size, dst, alpha, beta);
    }
    else if (size.channel == 3)
    {
        rppi_brightness_3C8U_pln_cpu(src, size, dst, alpha, beta);
    }

    printf("\nOutput of Brightness Modification:\n\n");
    display(dst, size);
}
*/