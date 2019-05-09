// rppi_contrast
// Parameters to functions are in the order inputs, input optional, outputs, outputs optional

// Uncomment the segment below to get this standalone to work for basic unit testing

#include <stdio.h>
#include <algorithm>
#include "rppdefs.h"
 
using namespace std;





RppStatus rppi_contrast_1C8U_pln_cpu(Rpp8u *pSrc, RppiSize size, Rpp8u *pDst, Rpp32f newMax = 255, Rpp32f newMin = 0)
{
    Rpp32f Min = (Rpp32f) *std::min_element(pSrc,pSrc + (size.width * size.height));
    Rpp32f Max = (Rpp32f) *std::max_element(pSrc,pSrc + (size.width * size.height));
    
    for (int i = 0; i < (size.width * size.height); i++)
    {
        Rpp32f pixel = (Rpp32f) pSrc[i];
        pixel = ((pixel - Min) * ((newMax - newMin) / (Max - Min))) + newMin;
        pixel = std::min(pixel, newMax);
        pixel = std::max(pixel, newMin);
        pDst[i] = (Rpp8u) pixel;
    }
    
    return RPP_SUCCESS;

}

RppStatus rppi_contrast_3C8U_pln_cpu(Rpp8u *pSrc, RppiSize size, Rpp8u *pDst, Rpp32f newMax = 255, Rpp32f newMin = 0)
{
    
    for(int c = 0; c < size.channel; c++)
    {
        Rpp32f Min = (Rpp32f) *std::min_element(pSrc + (c * size.width * size.height), pSrc + ((c + 1) * size.width * size.height));
        Rpp32f Max = (Rpp32f) *std::max_element(pSrc + (c * size.width * size.height), pSrc + ((c + 1) * size.width * size.height));
        
        for (int i = 0; i < (size.width * size.height); i++)
        {
            Rpp32f pixel = (Rpp32f) pSrc[i + (c * size.width * size.height)];
            pixel = ((pixel - Min) * ((newMax - newMin) / (Max - Min))) + newMin;
            pixel = std::min(pixel, newMax);
            pixel = std::max(pixel, newMin);
            pDst[i + (c * size.width * size.height)] = (Rpp8u) pixel;
        }
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

    Rpp32f newMax = 240, newMin = 5;
    
    printf("\n\n\n\nEnter elements in array of size %d x %d x %d: \n", size.channel, size.width, size.height);
    input(isrc, size);

    cast(isrc, src, size);

    printf("\nInput:\n\n");
    display(src, size);

    if (size.channel == 1)
    {
        rppi_contrast_1C8U_pln_cpu(src, size, dst, newMax, newMin);
    }
    else if (size.channel == 3)
    {
        rppi_contrast_3C8U_pln_cpu(src, size, dst, newMax, newMin);
    }

    printf("\nOutput of Contrast Modification:\n\n");
    display(dst, size);
}
