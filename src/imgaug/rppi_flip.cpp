// rppi_flip

// Uncomment the segment below to get this standalone to work for basic unit testing

#include <stdio.h>
#include <stdlib.h>
#include "rppdefs.h"
#include "cpu/host_flip.hpp"





RppStatus 
rppi_flip_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
                       RppiAxis flipAxis)
{
    host_flip<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                     flipAxis,
                     RPPI_CHN_PLANAR, 1);
    return RPP_SUCCESS;
}

RppStatus
rppi_flip_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
                       RppiAxis flipAxis)
{
    host_flip<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                     flipAxis,
                     RPPI_CHN_PLANAR, 3);
    return RPP_SUCCESS;
}

RppStatus
rppi_flip_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
                       RppiAxis flipAxis)
{
    host_flip<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                     flipAxis,
                     RPPI_CHN_PACKED, 3);
    return RPP_SUCCESS;
}

/*RppStatus rppi_flip_1C8U_pln_cpu(Rpp8u *pSrc, RppiSize size, Rpp8u *pDst, RppiAxis flipAxis)
{
    if (flipAxis == RPPI_HORIZONTAL_AXIS)
    {
        int srcLoc = 0, dstLoc = 0;
        for (int i = (size.height - 1); i >= 0; i--)
        {
            for (int j = 0; j < size.width; j++)
            {
                srcLoc = (i * size.width) + j;
                pDst[dstLoc] = pSrc[srcLoc];
                dstLoc += 1;
            }
        }
    }
    else if (flipAxis == RPPI_VERTICAL_AXIS)
    {
        int srcLoc = 0, dstLoc = 0;
        for (int i = (size.width - 1); i >= 0; i--)
        {
            dstLoc = size.width - 1 - i;
            for (int j = 0; j < size.height; j++)
            {
                srcLoc = (j * size.width) + i;
                pDst[dstLoc] = pSrc[srcLoc];
                dstLoc += size.width;
            }
        }
    }
    else if (flipAxis == RPPI_BOTH_AXIS)
    {
        Rpp8u *pInter = (Rpp8u *)malloc(size.width * size.height * sizeof(Rpp8u));
        int srcLoc = 0, interLoc = 0;
        for (int i = (size.height - 1); i >= 0; i--)
        {
            for (int j = 0; j < size.width; j++)
            {
                srcLoc = (i * size.width) + j;
                pInter[interLoc] = pSrc[srcLoc];
                interLoc += 1;
            }
        }
        int dstLoc = 0;
        interLoc = 0;
        for (int i = (size.width - 1); i >= 0; i--)
        {
            dstLoc = size.width - 1 - i;
            for (int j = 0; j < size.height; j++)
            {
                interLoc = (j * size.width) + i;
                pDst[dstLoc] = pInter[interLoc];
                dstLoc += size.width;
            }
        }
    }

    return RPP_SUCCESS;

}

RppStatus rppi_flip_3C8U_pln_cpu(Rpp8u *pSrc, RppiSize size, Rpp8u *pDst, RppiAxis flipAxis)
{
    if (flipAxis == RPPI_HORIZONTAL_AXIS)
    {
        int srcLoc = 0, dstLoc = 0;
        for (int i = (size.height - 1); i >= 0; i--)
        {
            for (int j = 0; j < size.width; j++)
            {
                srcLoc = (i * size.width) + j;
                pDst[dstLoc] = pSrc[srcLoc];
                pDst[dstLoc + (size.width * size.height)] = pSrc[srcLoc + (size.width * size.height)];
                pDst[dstLoc + (2 * size.width * size.height)] = pSrc[srcLoc + (2 * size.width * size.height)];
                dstLoc += 1;
            }
        }
    }
    else if (flipAxis == RPPI_VERTICAL_AXIS)
    {
        int srcLoc = 0, dstLoc = 0;
        for (int i = (size.width - 1); i >= 0; i--)
        {
            dstLoc = size.width - 1 - i;
            for (int j = 0; j < size.height; j++)
            {
                srcLoc = (j * size.width) + i;
                pDst[dstLoc] = pSrc[srcLoc];
                pDst[dstLoc + (size.width * size.height)] = pSrc[srcLoc + (size.width * size.height)];
                pDst[dstLoc + (2 * size.width * size.height)] = pSrc[srcLoc + (2 * size.width * size.height)];
                dstLoc += size.width;
            }
        }
    }
    else if (flipAxis == RPPI_BOTH_AXIS)
    {
        Rpp8u *pInter = (Rpp8u *)malloc(size.channel * size.width * size.height * sizeof(Rpp8u));
        int srcLoc = 0, interLoc = 0;
        for (int i = (size.height - 1); i >= 0; i--)
        {
            for (int j = 0; j < size.width; j++)
            {
                srcLoc = (i * size.width) + j;
                pInter[interLoc] = pSrc[srcLoc];
                pInter[interLoc + (size.width * size.height)] = pSrc[srcLoc + (size.width * size.height)];
                pInter[interLoc + (2 * size.width * size.height)] = pSrc[srcLoc + (2 * size.width * size.height)];
                interLoc += 1;
            }
        }
        int dstLoc = 0;
        interLoc = 0;
        for (int i = (size.width - 1); i >= 0; i--)
        {
            dstLoc = size.width - 1 - i;
            for (int j = 0; j < size.height; j++)
            {
                interLoc = (j * size.width) + i;
                pDst[dstLoc] = pInter[interLoc];
                pDst[dstLoc + (size.width * size.height)] = pInter[interLoc + (size.width * size.height)];
                pDst[dstLoc + (2 * size.width * size.height)] = pInter[interLoc + (2 * size.width * size.height)];
                dstLoc += size.width;
            }
        }
    }

    return RPP_SUCCESS;

}

RppStatus rppi_flip_3C8U_pkd_cpu(Rpp8u *pSrc, RppiSize size, Rpp8u *pDst, RppiAxis flipAxis)
{
    if (flipAxis == RPPI_HORIZONTAL_AXIS)
    {
        int srcLoc = 0, dstLoc = 0;
        for (int i = (size.height - 1); i >= 0; i--)
        {
            for (int j = 0; j < size.width; j++)
            {
                srcLoc = (i * 3 * size.width) + (3 * j);
                pDst[dstLoc] = pSrc[srcLoc];
                pDst[dstLoc + 1] = pSrc[srcLoc + 1];
                pDst[dstLoc + 2] = pSrc[srcLoc + 2];
                srcLoc += 3;
                dstLoc += 3;
            }
        }
    }
    else if (flipAxis == RPPI_VERTICAL_AXIS)
    {
        int srcLoc = 0, dstLoc = 0;
        for (int i = (size.width - 1); i >= 0; i--)
        {
            dstLoc = 3 * (size.width - 1 - i);
            for (int j = 0; j < size.height; j++)
            {
                srcLoc = (j * 3 * size.width) + (i * 3);
                pDst[dstLoc] = pSrc[srcLoc];
                pDst[dstLoc + 1] = pSrc[srcLoc + 1];
                pDst[dstLoc + 2] = pSrc[srcLoc + 2];
                dstLoc += (size.width * 3);
            }
        }
    }
    else if (flipAxis == RPPI_BOTH_AXIS)
    {
        Rpp8u *pInter = (Rpp8u *)malloc(size.channel * size.width * size.height * sizeof(Rpp8u));
        int srcLoc = 0, interLoc = 0;

        for (int i = (size.height - 1); i >= 0; i--)
        {
            for (int j = 0; j < size.width; j++)
            {
                srcLoc = (i * 3 * size.width) + (3 * j);
                pInter[interLoc] = pSrc[srcLoc];
                pInter[interLoc + 1] = pSrc[srcLoc + 1];
                pInter[interLoc + 2] = pSrc[srcLoc + 2];
                srcLoc += 3;
                interLoc += 3;
            }
        }

        int dstLoc = 0;
        interLoc = 0;


        for (int i = (size.width - 1); i >= 0; i--)
        {
            dstLoc = 3 * (size.width - 1 - i);
            for (int j = 0; j < size.height; j++)
            {
                interLoc = (j * 3 * size.width) + (i * 3);
                pDst[dstLoc] = pInter[interLoc];
                pDst[dstLoc + 1] = pInter[interLoc + 1];
                pDst[dstLoc + 2] = pInter[interLoc + 2];
                dstLoc += (size.width * 3);
            }
        }
    }

    return RPP_SUCCESS;

}*/






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

void cast(int *intSrcPtr, Rpp8u *srcPtr, RppiSize srcSize, unsigned int channel)
{
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        intSrcPtr[i] = (intSrcPtr[i] < 255) ? intSrcPtr[i] : 255;
        intSrcPtr[i] = (intSrcPtr[i] > 0) ? intSrcPtr[i] : 0;
        srcPtr[i] = (Rpp8u) intSrcPtr[i];

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
/*
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

    RppiAxis flipAxis = RPPI_BOTH_AXIS;

    printf("\n\n\n\nEnter elements in array of size %d x %d x %d: \n", size.channel, size.width, size.height);
    input(isrc, size);

    cast(isrc, src, size);

    if (size.channel == 1)
    {
        printf("\nInput:\n\n");
        display(src, size);
        rppi_flip_1C8U_pln_cpu(src, size, dst, flipAxis);
        printf("\nOutput of Brightness Modification:\n\n");
        display(dst, size);
    }
    else if ((size.channel == 3) && (type == 1))
    {
        printf("\nInput:\n\n");
        display(src, size);
        rppi_flip_3C8U_pln_cpu(src, size, dst, flipAxis);
        printf("\nOutput of Brightness Modification:\n\n");
        display(dst, size);
    }
    else if ((size.channel == 3) && (type == 2))
    {
        printf("\nInput:\n\n");
        displayPacked(src, size);
        rppi_flip_3C8U_pkd_cpu(src, size, dst, flipAxis);
        printf("\nOutput of Brightness Modification:\n\n");
        displayPacked(dst, size);
    }
}*/

int main()
{
    RppiSize srcSize;
    unsigned int channel;
    RppiAxis flipAxis = RPPI_BOTH_AXIS;
     
    int matrix;
    printf("\nEnter matrix input style: 1 = default 1 channel (1x3x4), 2 = default 3 channel (3x3x4), 3 = customized: ");
    scanf("%d", &matrix);

    int type;
    printf("\nEnter type of arrangement: 1 = planar, 2 = packed: ");
    scanf("%d", &type);
    
    if (matrix == 1)
    {
        channel = 1;
        srcSize.height = 3;
        srcSize.width = 4;
        Rpp8u srcPtr[12] = {130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108};
        Rpp8u dstPtr[12] = {0};
        printf("\n\nInput:\n");
        displayPlanar(srcPtr, srcSize, channel);
        rppi_flip_u8_pln1_host(srcPtr, srcSize, dstPtr, flipAxis);
        printf("\n\nOutput of Flip:\n");
        displayPlanar(dstPtr, srcSize, channel);
    }
    else if (matrix == 2)
    {
        channel = 3;
        srcSize.height = 3;
        srcSize.width = 4;
        if (type == 1)
        {
            Rpp8u srcPtr[36] = {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108, 65, 66, 67, 68, 69, 70, 71, 72, 13, 24, 15, 16};
            Rpp8u dstPtr[36] = {0};
            printf("\n\nInput:\n");
            displayPlanar(srcPtr, srcSize, channel);
            rppi_flip_u8_pln3_host(srcPtr, srcSize, dstPtr, flipAxis);
            printf("\n\nOutput of Flip:\n");
            displayPlanar(dstPtr, srcSize, channel);
        }
        else if (type == 2)
        {
            Rpp8u srcPtr[36] = {255, 130, 65, 254, 129, 66, 253, 128, 67, 252, 127, 68, 251, 126, 69, 250, 117, 70, 249, 113, 71, 248, 121, 72, 247, 127, 13, 246, 111, 24, 245, 100, 15, 244, 108, 16};
            Rpp8u dstPtr[36] = {0};
            printf("\n\nInput:\n");
            displayPacked(srcPtr, srcSize, channel);
            rppi_flip_u8_pkd3_host(srcPtr, srcSize, dstPtr, flipAxis);
            printf("\n\nOutput of Flip:\n");
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
        Rpp8u *srcPtr = (Rpp8u *)malloc(channel * srcSize.height * srcSize.width * sizeof(Rpp8u));
        Rpp8u *dstPtr = (Rpp8u *)malloc(channel * srcSize.height * srcSize.width * sizeof(Rpp8u));
        int *intSrcPtr = (int *)malloc(channel * srcSize.height * srcSize.width * sizeof(int));
        if (type == 1)
        {
            printf("\n\n\n\nEnter elements in array of size %d x %d x %d in planar format: \n", channel, srcSize.height, srcSize.width);
            inputPlanar(intSrcPtr, srcSize, channel);
            cast(intSrcPtr, srcPtr, srcSize, channel);
            printf("\n\nInput:\n");
            displayPlanar(srcPtr, srcSize, channel);
            if (channel == 1)
            {
                rppi_flip_u8_pln1_host(srcPtr, srcSize, dstPtr, flipAxis);
            }
            else if (channel == 3)
            {
                rppi_flip_u8_pln3_host(srcPtr, srcSize, dstPtr, flipAxis);
            }
            printf("\n\nOutput of Flip:\n");
            displayPlanar(dstPtr, srcSize, channel);
        }
        else if (type == 2)
        {
            printf("\n\n\n\nEnter elements in array of size %d x %d x %d in packed format: \n", channel, srcSize.height, srcSize.width);
            inputPacked(intSrcPtr, srcSize, channel);
            cast(intSrcPtr, srcPtr, srcSize, channel);
            printf("\n\nInput:\n");
            displayPacked(srcPtr, srcSize, channel);
            if (channel == 1)
            {
                rppi_flip_u8_pln1_host(srcPtr, srcSize, dstPtr, flipAxis);
            }
            else if (channel == 3)
            {
                rppi_flip_u8_pkd3_host(srcPtr, srcSize, dstPtr, flipAxis);
            }
            printf("\n\nOutput of Flip:\n");
            displayPacked(dstPtr, srcSize, channel);
        }
    }
}
