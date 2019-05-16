// rppi_flip
// Parameters to functions are in the order inputs, input optional, outputs, outputs optional

// Uncomment the segment below to get this standalone to work for basic unit testing

#include <stdio.h>
#include <algorithm>
#include "rppdefs.h"
 
using namespace std;





RppStatus rppi_flip_1C8U_pln_cpu(Rpp8u *pSrc, RppiSize size, Rpp8u *pDst, RppiAxis flipAxis)
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
}
