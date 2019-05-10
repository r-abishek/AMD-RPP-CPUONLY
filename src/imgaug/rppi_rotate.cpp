// rppi_rotate
// Parameters to functions are in the order inputs, input optional, outputs, outputs optional

// Uncomment the segment below to get this standalone to work for basic unit testing

#include <stdio.h>
#include <math.h>
#include <algorithm>
#include "rppdefs.h"
 
using namespace std;

#define PI 3.14159265
#define RAD(deg) (deg * PI / 180)




RppStatus rppi_rotate_1C8U_pln_cpu(Rpp8u *pToRotate, RppiSize sizeSrc, Rpp8u *pDst, RppiSize sizeDst, Rpp32f angleRad = 0)
{
    // Step 1 - Rotate
    int m, n;
    Rpp8u *pSrc = (Rpp8u *)malloc(sizeDst.channel * sizeDst.width * sizeDst.height * sizeof(Rpp8u));
    for (int i = 0; i < sizeSrc.height; i++)
    {
        for (int j = 0; j < sizeSrc.width; j++)
        {
            m = (int) ((i * cos(angleRad)) - (j * sin(angleRad)) + (sizeSrc.width * sin(angleRad)));
            n = (int) ((i * sin(angleRad)) + (j * cos(angleRad)));
            pSrc[(m * sizeDst.width) + n] = pToRotate[(i * sizeSrc.width) + j];
        }
    }
    RppiSize size;
    size.channel = sizeDst.channel;
    size.width = sizeDst.width;
    size.height = sizeDst.height;

    // Step 2 - Blur
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

RppStatus rppi_rotate_3C8U_pln_cpu(Rpp8u *pToRotate, RppiSize sizeSrc, Rpp8u *pDst, RppiSize sizeDst, Rpp32f angleRad = 0)
{
    // Step 1 - Rotate
    int m, n;
    Rpp8u *pSrc = (Rpp8u *)malloc(sizeDst.channel * sizeDst.width * sizeDst.height * sizeof(Rpp8u));
    for (int i = 0; i < sizeSrc.height; i++)
    {
        for (int j = 0; j < sizeSrc.width; j++)
        {
            m = (int) ((i * cos(angleRad)) - (j * sin(angleRad)) + (sizeSrc.width * sin(angleRad)));
            n = (int) ((i * sin(angleRad)) + (j * cos(angleRad)));
            pSrc[(m * sizeDst.width) + n] = pToRotate[(i * sizeSrc.width) + j];
            pSrc[(1 * sizeDst.width * sizeDst.height) + (m * sizeDst.width) + n] = pToRotate[(1 * sizeSrc.width * sizeSrc.height) + (i * sizeSrc.width) + j];
            pSrc[(2 * sizeDst.width * sizeDst.height) + (m * sizeDst.width) + n] = pToRotate[(2 * sizeSrc.width * sizeSrc.height) + (i * sizeSrc.width) + j];
        }
    }
    RppiSize size;
    size.channel = sizeDst.channel;
    size.width = sizeDst.width;
    size.height = sizeDst.height;

    // Step 2 - Blur
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
    RppiSize sizeSrc;
    printf("\nEnter number of channels: ");
    scanf("%d", &sizeSrc.channel);
    printf("Enter width of image in pixels: ");
    scanf("%d", &sizeSrc.width);
    printf("Enter height of image in pixels: ");
    scanf("%d", &sizeSrc.height);
    printf("Channels = %d, Width = %d, Height = %d", sizeSrc.channel, sizeSrc.width, sizeSrc.height);

    Rpp8u *src = (Rpp8u *)malloc(sizeSrc.channel * sizeSrc.width * sizeSrc.height * sizeof(Rpp8u));
    
    int *isrc = (int *)malloc(sizeSrc.channel * sizeSrc.width * sizeSrc.height * sizeof(int));

    Rpp32f angleDeg = 45;
    angleDeg = std::min(angleDeg, (Rpp32f) 90);
    angleDeg = std::max(angleDeg, (Rpp32f) 0);
    Rpp32f angleRad = RAD(angleDeg);

    RppiSize sizeDst;
    sizeDst.channel = sizeSrc.channel;
    sizeDst.width = (Rpp32u) ((sizeSrc.width * cos(angleRad)) + (sizeSrc.height * cos(RAD(90) - angleRad)));
    sizeDst.height = (Rpp32u) ((sizeSrc.width * sin(angleRad)) + (sizeSrc.height * sin(RAD(90) - angleRad)));

    Rpp8u *dst = (Rpp8u *)malloc(sizeDst.channel * sizeDst.width * sizeDst.height * sizeof(Rpp8u));
    
    printf("\n\n\n\nEnter elements in array of size %d x %d x %d: \n", sizeSrc.channel, sizeSrc.width, sizeSrc.height);
    input(isrc, sizeSrc);

    cast(isrc, src, sizeSrc);

    printf("\nInput:\n\n");
    display(src, sizeSrc);

    if (sizeSrc.channel == 1)
    {
        rppi_rotate_1C8U_pln_cpu(src, sizeSrc, dst, sizeDst, angleRad);
        //rppi_gaussianBlur3x3_1C8U_pln_cpu(dst, sizeDst, dst);
    }
    else if (sizeSrc.channel == 3)
    {
        rppi_rotate_3C8U_pln_cpu(src, sizeSrc, dst, sizeDst, angleRad);
    }

    printf("\nOutput of Rotate Operation:\n\n");
    display(dst, sizeDst);
}
