// rppi_hsv2rgb

// Uncomment the segment below to get this standalone to work for basic unit testing

#include <stdio.h>
#include <stdlib.h>
#include "rppdefs.h"
#include "cpu/host_hsv2rgb.hpp"
#include "rppi_image_augumentation_functions.h"
 




RppStatus
rppi_hsv2rgb_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{

    host_hsv2rgb<Rpp32f, Rpp8u>(static_cast<Rpp32f*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                         RPPI_CHN_PLANAR, 3);
    return RPP_SUCCESS;

}

RppStatus
rppi_hsv2rgb_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{

    host_hsv2rgb<Rpp32f, Rpp8u>(static_cast<Rpp32f*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                         RPPI_CHN_PACKED, 3);
    return RPP_SUCCESS;

}





// Uncomment the segment below to get this standalone to work for basic unit testing

void inputPlanar(int *intSrcPtr, RppiSize srcSize, unsigned int channel)
{
    int p = 0;
    for(int c = 0; c < channel; c++)
    {
        printf("\n\nEnter %d elements for channel %d:\n", (srcSize.width * srcSize.height), c+1);
        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            scanf("%d", intSrcPtr + p);
            p += 1;
        }
    }
}

void inputPacked(int *intSrcPtr, RppiSize srcSize, unsigned int channel)
{
    int p = 0;
    printf("\n\nEnter %d elements for the image:\n", (channel * srcSize.height * srcSize.width));
    for (int i = 0; i < (srcSize.height * srcSize.width); i++)
    {
        for (int c = 0; c < channel; c++)
        {
            printf("Channel %d - ", c+1);
            scanf("%d", intSrcPtr + p);
            p += 1;
        }
        printf("\n");
    }
}

void inputPlanarF(float *floatSrcPtr, RppiSize srcSize, unsigned int channel)
{
    int p = 0;
    for(int c = 0; c < channel; c++)
    {
        printf("\n\nEnter %d elements for channel %d:\n", (srcSize.width * srcSize.height), c+1);
        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            scanf("%f", floatSrcPtr + p);
            p += 1;
        }
    }
}

void inputPackedF(float *floatSrcPtr, RppiSize srcSize, unsigned int channel)
{
    int p = 0;
    printf("\n\nEnter %d elements for the image:\n", (channel * srcSize.height * srcSize.width));
    for (int i = 0; i < (srcSize.height * srcSize.width); i++)
    {
        for (int c = 0; c < channel; c++)
        {
            printf("Channel %d - ", c+1);
            scanf("%f", floatSrcPtr + p);
            p += 1;
        }
        printf("\n");
    }
}

void displayPlanar(Rpp8u *pArr, RppiSize size, unsigned int channel)
{
    int p = 0;
    for(int c = 0; c < channel; c++)
    {
        printf("\n\nChannel %d:\n", c+1);
        for (int i = 0; i < (size.height * size.width); i++)
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

void displayPacked(Rpp8u *pArr, RppiSize size, unsigned int channel)
{
    int p = 0;
    for (int i = 0; i < size.height; i++)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int j = 0; j < size.width; j++)
            {
                printf("%d\t\t", *(pArr + p + c + (j * channel)));
            }
            printf("\n");
        }
        printf("\n");
        p += (channel * size.width);
    }
}

void displayPlanarF(Rpp32f *pArr, RppiSize size, unsigned int channel)
{
    int p = 0;
    for(int c = 0; c < channel; c++)
    {
        printf("\n\nChannel %d:\n", c+1);
        for (int i = 0; i < (size.height * size.width); i++)
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

void displayPackedF(Rpp32f *pArr, RppiSize size, unsigned int channel)
{
    int p = 0;
    for (int i = 0; i < size.height; i++)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int j = 0; j < size.width; j++)
            {
                printf("%0.2f\t\t", *(pArr + p + c + (j * channel)));
            }
            printf("\n");
        }
        printf("\n");
        p += (channel * size.width);
    }
}

int main()
{
    RppiSize srcSize;
    unsigned int channel;
     
    int matrix;
    printf("\nEnter matrix input style: 1 = default 1 channel (1x3x4), 2 = default 3 channel (3x3x4), 3 = customized: ");
    scanf("%d", &matrix);

    int type;
    printf("\nEnter type of arrangement: 1 = planar, 2 = packed: ");
    scanf("%d", &type);
    
    if (matrix == 1)
    {
        printf("\nThe function is only 3 channel compatible!\n");
    }
    else if (matrix == 2)
    {
        channel = 3;
        srcSize.height = 3;
        srcSize.width = 4;
        if (type == 1)
        {
            Rpp32f srcPtr[36] = {21.00, 20.00, 20.00, 19.00, 19.00, 16.00, 14.00, 17.00, 29.00, 24.00, 22.00, 24.00, 0.75, 0.74, 0.74, 0.73, 0.73, 0.72, 0.71, 0.71, 0.95, 0.90, 0.94, 0.93, 1.00, 1.00, 0.99, 0.99, 0.98, 0.98, 0.98, 0.97, 0.97, 0.96, 0.96, 0.96};
            Rpp8u dstPtr[36] = {0};
            printf("\n\nInput:\n");
            displayPlanarF(srcPtr, srcSize, channel);
            rppi_hsv2rgb_u8_pln3_host(srcPtr, srcSize, dstPtr);
            printf("\n\nOutput of hsv2rgb:\n");
            displayPlanar(dstPtr, srcSize, channel);
        }
        else if (type == 2)
        {
            Rpp32f srcPtr[36] = {21.0, 0.75, 1.0, 20.0, 0.74, 1.0, 20.0, 0.74, 0.99, 19.0, 0.73, 0.99, 19.0, 0.73, 0.98, 16.0, 0.72, 0.98, 14.0, 0.71, 0.98, 17.0, 0.71, 0.97, 29.0, 0.95, 0.97, 24.0, 0.9, 0.96, 22.0, 0.94, 0.96, 24.0, 0.93, 0.96};
            Rpp8u dstPtr[36] = {0};
            printf("\n\nInput:\n");
            displayPackedF(srcPtr, srcSize, channel);
            rppi_hsv2rgb_u8_pkd3_host(srcPtr, srcSize, dstPtr);
            printf("\n\nOutput of hsv2rgb:\n");
            displayPacked(dstPtr, srcSize, channel);
        }
    }
    else if (matrix == 3)
    {
        printf("\nEnter number of channels: ");
        scanf("%d", &channel);
        printf("Enter height of image in pixels: ");
        scanf("%d", &srcSize.height);
        printf("Enter width of image in pixels: ");
        scanf("%d", &srcSize.width);
        printf("Channels = %d, Height = %d, Width = %d", channel, srcSize.height, srcSize.width);
        Rpp32f *srcPtr = (Rpp32f *)malloc(channel * srcSize.height * srcSize.width * sizeof(Rpp32f));
        Rpp8u *dstPtr = (Rpp8u *)malloc(channel * srcSize.height * srcSize.width * sizeof(Rpp8u));
        if (type == 1)
        {
            printf("\n\n\n\nEnter elements in array of size %d x %d x %d in planar format: \n", channel, srcSize.height, srcSize.width);
            inputPlanarF(srcPtr, srcSize, channel);
            printf("\n\nInput:\n");
            displayPlanarF(srcPtr, srcSize, channel);
            if (channel == 1)
            {
                printf("\nThe function is only 3 channel compatible!\n");
            }
            else if (channel == 3)
            {
                rppi_hsv2rgb_u8_pln3_host(srcPtr, srcSize, dstPtr);
            }
            printf("\n\nOutput of hsv2rgb:\n");
            displayPlanar(dstPtr, srcSize, channel);
        }
        else if (type == 2)
        {
            printf("\n\n\n\nEnter elements in array of size %d x %d x %d in packed format: \n", channel, srcSize.height, srcSize.width);
            inputPackedF(srcPtr, srcSize, channel);
            printf("\n\nInput:\n");
            displayPackedF(srcPtr, srcSize, channel);
            if (channel == 1)
            {
                printf("\nThe function is only 3 channel compatible!\n");
            }
            else if (channel == 3)
            {
                rppi_hsv2rgb_u8_pkd3_host(srcPtr, srcSize, dstPtr);
            }
            printf("\n\nOutput of hsv2rgb:\n");
            displayPacked(dstPtr, srcSize, channel);
        }
    }
}