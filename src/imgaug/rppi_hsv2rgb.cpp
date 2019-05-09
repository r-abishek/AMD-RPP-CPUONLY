// rppi_hsv2rgb
// Parameters to functions are in the order inputs, input optional, outputs, outputs optional

// Uncomment the segment below to get this standalone to work for basic unit testing

#include <stdio.h>
#include <math.h>
#include <algorithm>
#include "rppdefs.h"
 
using namespace std;





RppStatus rppi_hsv2rgb_3C8U_pln_cpu(Rpp32f *pSrc, RppiSize size, Rpp8u *pDst)
{    
    for (int i = 0; i < (size.width * size.height); i++)
    {
        float c, x, m, rf, gf, bf;
        c = pSrc[i + (2 * size.width * size.height)] * pSrc[i + (size.width * size.height)];
        x = c * (1 - abs((fmod((pSrc[i] / 60), 2)) - 1));
        m = pSrc[i + (2 * size.width * size.height)] - c;
        
        if ((0 <= pSrc[i]) && (pSrc[i] < 60))
        {
            rf = c;
            gf = x;
            bf = 0;
        }
        else if ((60 <= pSrc[i]) && (pSrc[i] < 120))
        {
            rf = x;
            gf = c;
            bf = 0;
        }
        else if ((120 <= pSrc[i]) && (pSrc[i] < 180))
        {
            rf = 0;
            gf = c;
            bf = x;
        }
        else if ((180 <= pSrc[i]) && (pSrc[i] < 240))
        {
            rf = 0;
            gf = x;
            bf = c;
        }
        else if ((240 <= pSrc[i]) && (pSrc[i] < 300))
        {
            rf = x;
            gf = 0;
            bf = c;
        }
        else if ((300 <= pSrc[i]) && (pSrc[i] < 360))
        {
            rf = c;
            gf = 0;
            bf = x;
        }

        pDst[i] = (Rpp8u) ((rf + m) * 255);
        pDst[i + (size.width * size.height)] = (Rpp8u) ((gf + m) * 255);
        pDst[i + (2 * size.width * size.height)] = (Rpp8u) ((bf + m) * 255);
    }

    return RPP_SUCCESS;

}





// Uncomment the segment below to get this standalone to work for basic unit testing

void input(float *fsrc, RppiSize size)
{
    int p = 0;
    for(int c = 0; c < size.channel; c++)
    {
        printf("\n\nEnter %d elements for channel %d:\n", (size.width * size.height), c+1);
        for (int i = 0; i < (size.width * size.height); i++)
        {
            scanf("%f", fsrc + p);
            p += 1;
        }
    }
}

void cast(float *fsrc, Rpp32f *pSrc, RppiSize size)
{
    for (int i = 0; i < (size.channel * size.width * size.height); i++)
    {
        pSrc[i] = (Rpp32f) fsrc[i];
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

    Rpp32f *src = (Rpp32f *)malloc(size.channel * size.width * size.height * sizeof(Rpp32f));
    Rpp8u *dst = (Rpp8u *)malloc(size.channel * size.width * size.height * sizeof(Rpp8u));
    
    float *fsrc = (float *)malloc(size.channel * size.width * size.height * sizeof(float));
    
    printf("\n\n\n\nEnter elements in array of size %d x %d x %d: \n", size.channel, size.width, size.height);
    input(fsrc, size);

    cast(fsrc, src, size);

    printf("\nInput:");
    displayf(src, size);

    rppi_hsv2rgb_3C8U_pln_cpu(src, size, dst);

    printf("\n\nOutput of HSV to RGB Modification:");
    display(dst, size);
}
