// rppi_gaussianBlur
// Parameters to functions are in the order inputs, input optional, outputs, outputs optional

// Uncomment the segment below to get this standalone to work for basic unit testing

#include <stdio.h>
#include <math.h>
#include <algorithm>
#include "rppdefs.h"
 
using namespace std;

void display(Rpp8u *pArr, RppiSize size);
void displayPacked(Rpp8u *pArr, RppiSize size);




RppStatus rppi_gaussianBlur3x3_1C8U_pln_cpu(Rpp8u *pSrc, RppiSize size, Rpp8u *pDst)
{
    float kernel_3x3[9] = {1,2,1,2,4,2,1,2,1};
    for (int i = 0; i < 9; i++)
    {
        kernel_3x3[i] *= 0.0625;
    }
    RppiSize sizeMod;
    sizeMod.channel = size.channel;
    sizeMod.width = size.width + 2;
    sizeMod.height = size.height + 2;

    Rpp8u *pSrcMod = (Rpp8u *)malloc(sizeMod.width * sizeMod.height * sizeof(Rpp8u));

    int srcLoc = 0, srcModLoc = 0, dstLoc = 0;
    for (int i = 0; i < sizeMod.width; i++)
    {
        pSrcMod[srcModLoc] = 0;
        srcModLoc += 1;
    }
    for (int i = 0; i < size.height; i++)
    {
        pSrcMod[srcModLoc] = 0;
        srcModLoc += 1;
        for (int j = 0; j < size.width; j++)
        {
            pSrcMod[srcModLoc] = pSrc[srcLoc];
            srcModLoc += 1;
            srcLoc += 1;
        }
        pSrcMod[srcModLoc] = 0;
        srcModLoc += 1;
    }
    for (int i = 0; i < sizeMod.width; i++)
    {
        pSrcMod[srcModLoc] = 0;
        srcModLoc += 1;
    }
    
    dstLoc = 0;
    srcModLoc = 0;
    int convLocs[9] = {0}, count = 0;
    float pixel = 0.0;

    for (int i = 0; i < size.height; i++)
    {
        for (int j = 0; j < size.width; j++)
        {
            count = 0;
            pixel = 0.0;
            for (int m = 0; m < 3; m++)
            {
                for (int n = 0; n < 3; n++, count++)
                {
                    convLocs[count] = srcModLoc + (m * sizeMod.width) + n;
                }
            }
            for (int k = 0; k < 9; k++)
            {
                pixel += (kernel_3x3[k] * (float)pSrcMod[convLocs[k]]);
            }
            pixel = std::min(pixel, (Rpp32f) 255);
            pixel = std::max(pixel, (Rpp32f) 0);
            pDst[dstLoc] = (Rpp8u) round(pixel);
            dstLoc += 1;
            srcModLoc += 1;
        }
        srcModLoc += 2;
    }
    
    return RPP_SUCCESS;

}

RppStatus rppi_gaussianBlur3x3_3C8U_pln_cpu(Rpp8u *pSrc, RppiSize size, Rpp8u *pDst)
{
    float kernel_3x3[9] = {1,2,1,2,4,2,1,2,1};
    for (int i = 0; i < 9; i++)
    {
        kernel_3x3[i] *= 0.0625;
    }
    RppiSize sizeMod;
    sizeMod.channel = size.channel;
    sizeMod.width = size.width + 2;
    sizeMod.height = size.height + 2;

    Rpp8u *pSrcMod = (Rpp8u *)malloc(sizeMod.channel * sizeMod.width * sizeMod.height * sizeof(Rpp8u));

    int srcLoc = 0, srcModLoc = 0, dstLoc = 0;
    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < sizeMod.width; i++)
        {
            pSrcMod[srcModLoc] = 0;
            srcModLoc += 1;
        }
        for (int i = 0; i < size.height; i++)
        {
            pSrcMod[srcModLoc] = 0;
            srcModLoc += 1;
            for (int j = 0; j < size.width; j++)
            {
                pSrcMod[srcModLoc] = pSrc[srcLoc];
                srcModLoc += 1;
                srcLoc += 1;
            }
            pSrcMod[srcModLoc] = 0;
            srcModLoc += 1;
        }
        for (int i = 0; i < sizeMod.width; i++)
        {
            pSrcMod[srcModLoc] = 0;
            srcModLoc += 1;
        }
    }
    
    dstLoc = 0;
    srcModLoc = 0;
    int convLocs[9] = {0}, count = 0;
    float pixel = 0.0;

    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < size.height; i++)
        {
            for (int j = 0; j < size.width; j++)
            {
                count = 0;
                pixel = 0.0;
                for (int m = 0; m < 3; m++)
                {
                    for (int n = 0; n < 3; n++, count++)
                    {
                        convLocs[count] = srcModLoc + (m * sizeMod.width) + n;
                    }
                }
                for (int k = 0; k < 9; k++)
                {
                    pixel += (kernel_3x3[k] * (float)pSrcMod[convLocs[k]]);
                }
                pixel = std::min(pixel, (Rpp32f) 255);
                pixel = std::max(pixel, (Rpp32f) 0);
                pDst[dstLoc] = (Rpp8u) round(pixel);
                dstLoc += 1;
                srcModLoc += 1;
            }
            srcModLoc += 2;
        }
        srcModLoc += (2 * sizeMod.width);
    }
    return RPP_SUCCESS;
}

RppStatus rppi_gaussianBlur3x3_3C8U_pkd_cpu(Rpp8u *pSrc, RppiSize size, Rpp8u *pDst)
{
    float kernel_3x3[9] = {1,2,1,2,4,2,1,2,1};
    for (int i = 0; i < 9; i++)
    {
        kernel_3x3[i] *= 0.0625;
    }
    RppiSize sizeMod;
    sizeMod.channel = size.channel;
    sizeMod.width = size.width + 2;
    sizeMod.height = size.height + 2;

    Rpp8u *pSrcMod = (Rpp8u *)malloc(sizeMod.channel * sizeMod.width * sizeMod.height * sizeof(Rpp8u));

    int srcLoc = 0, srcModLoc = 0, dstLoc = 0;
    for (int c = 0; c < 3; c++)
    {
        srcModLoc = c;
        srcLoc = c;
        for (int i = 0; i < sizeMod.width; i++)
        {
            pSrcMod[srcModLoc] = 0;
            srcModLoc += 3;
        }
        for (int i = 0; i < size.height; i++)
        {
            pSrcMod[srcModLoc] = 0;
            srcModLoc += 3;
            for (int j = 0; j < size.width; j++)
            {
                pSrcMod[srcModLoc] = pSrc[srcLoc];
                srcModLoc += 3;
                srcLoc += 3;
            }
            pSrcMod[srcModLoc] = 0;
            srcModLoc += 3;
        }
        for (int i = 0; i < sizeMod.width; i++)
        {
            pSrcMod[srcModLoc] = 0;
            srcModLoc += 3;
        }
    }

    dstLoc = 0;
    srcModLoc = 0;
    int convLocs[9] = {0}, count = 0;
    float pixel = 0.0;

    for (int c = 0; c < 3; c++)
    {
        srcModLoc = c;
        dstLoc = c;
        for (int i = 0; i < size.height; i++)
        {
            for (int j = 0; j < size.width; j++)
            {
                count = 0;
                pixel = 0.0;
                for (int m = 0; m < 3; m++)
                {
                    for (int n = 0; n < 3; n++, count++)
                    {
                        convLocs[count] = srcModLoc + (m * sizeMod.width * 3) + (n * 3);
                    }
                }
                for (int k = 0; k < 9; k++)
                {
                    pixel += (kernel_3x3[k] * (float)pSrcMod[convLocs[k]]);
                }
                pixel = std::min(pixel, (Rpp32f) 255);
                pixel = std::max(pixel, (Rpp32f) 0);
                pDst[dstLoc] = (Rpp8u) round(pixel);
                dstLoc += 3;
                srcModLoc += 3;
            }
            srcModLoc += 6;
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
    
    printf("\n\n\n\nEnter elements in array of size %d x %d x %d: \n", size.channel, size.width, size.height);
    input(isrc, size);

    cast(isrc, src, size);

    if (size.channel == 1)
    {
        printf("\nInput:\n");
        display(src, size);
        rppi_gaussianBlur3x3_1C8U_pln_cpu(src, size, dst);
        printf("\nOutput of Gaussian Blur 3x3:\n");
        display(dst, size);
    }
    else if ((size.channel == 3) && (type == 1))
    {
        printf("\nInput:\n");
        display(src, size);
        rppi_gaussianBlur3x3_3C8U_pln_cpu(src, size, dst);
        printf("\nOutput of Gaussian Blur 3x3:\n");
        display(dst, size);
    }
    else if ((size.channel == 3) && (type == 2))
    {
        printf("\nInput:\n");
        displayPacked(src, size);
        rppi_gaussianBlur3x3_3C8U_pkd_cpu(src, size, dst);
        printf("\nOutput of Gaussian Blur 3x3:\n");
        displayPacked(dst, size);
    }
}
