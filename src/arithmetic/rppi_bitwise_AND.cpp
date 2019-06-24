// rppi_bitwise_AND

// Uncomment the segment below to get this standalone to work for basic unit testing

#include "rppdefs.h"
#include "rppi_arithmetic_and_logical_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include "cpu/rpp_cpu_inputAndDisplay.hpp"
#include "cpu/host_bitwise_AND.hpp"
 
using namespace std;





RppStatus
rppi_bitwise_AND_u8_pln1_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr)
{
    bitwise_AND_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr1), static_cast<Rpp8u*>(srcPtr2), srcSize, static_cast<Rpp8u*>(dstPtr),
                                    1);

    return RPP_SUCCESS;

}

RppStatus
rppi_bitwise_AND_u8_pln3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr)
{
    bitwise_AND_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr1), static_cast<Rpp8u*>(srcPtr2), srcSize, static_cast<Rpp8u*>(dstPtr),
                                    3);

    return RPP_SUCCESS;

}

RppStatus
rppi_bitwise_AND_u8_pkd3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr)
{
    bitwise_AND_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr1), static_cast<Rpp8u*>(srcPtr2), srcSize, static_cast<Rpp8u*>(dstPtr),
                                    3);

    return RPP_SUCCESS;

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
        channel = 1;
        srcSize.height = 3;
        srcSize.width = 4;
        Rpp8u srcPtr1[12] = {130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108};
        Rpp8u srcPtr2[12] = {65, 66, 67, 68, 69, 70, 249, 248, 247, 246, 245, 244};
        Rpp8u dstPtr[12] = {0};
        printf("\n\nInput 1:\n");
        displayPlanar(srcPtr1, srcSize, channel);
        printf("\n\nInput 2:\n");
        displayPlanar(srcPtr2, srcSize, channel);
        rppi_bitwise_AND_u8_pln1_host(srcPtr1, srcPtr2, srcSize, dstPtr);
        printf("\n\nOutput of bitwise_AND operation:\n");
        displayPlanar(dstPtr, srcSize, channel);
    }
    else if (matrix == 2)
    {
        channel = 3;
        srcSize.height = 3;
        srcSize.width = 4;
        if (type == 1)
        {
            Rpp8u srcPtr1[36] = {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108, 65, 66, 67, 68, 69, 70, 71, 72, 13, 24, 15, 16};
            Rpp8u srcPtr2[36] = {16, 15, 24, 13, 72, 71, 70, 69, 68, 67, 66, 65, 108, 100, 111, 127, 121, 113, 117, 126, 127, 128, 129, 130, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};
            Rpp8u dstPtr[36] = {0};
            printf("\n\nInput 1:\n");
            displayPlanar(srcPtr1, srcSize, channel);
            printf("\n\nInput 2:\n");
            displayPlanar(srcPtr2, srcSize, channel);
            rppi_bitwise_AND_u8_pln3_host(srcPtr1, srcPtr2, srcSize, dstPtr);
            printf("\n\nOutput of bitwise_AND operation:\n");
            displayPlanar(dstPtr, srcSize, channel);
        }
        else if (type == 2)
        {
            Rpp8u srcPtr1[36] = {255, 130, 65, 254, 129, 66, 253, 128, 67, 252, 127, 68, 251, 126, 69, 250, 117, 70, 249, 113, 71, 248, 121, 72, 247, 127, 13, 246, 111, 24, 245, 100, 15, 244, 108, 16};
            Rpp8u srcPtr2[36] = {16, 108, 244, 15, 100, 245, 24, 111, 246, 13, 127, 247, 72, 121, 248, 71, 113, 249, 70, 117, 250, 69, 126, 251, 68, 127, 252, 67, 128, 253, 66, 129, 254, 65, 130, 255};
            Rpp8u dstPtr[36] = {0};
            printf("\n\nInput 1:\n");
            displayPacked(srcPtr1, srcSize, channel);
            printf("\n\nInput 2:\n");
            displayPacked(srcPtr2, srcSize, channel);
            rppi_bitwise_AND_u8_pkd3_host(srcPtr1, srcPtr2, srcSize, dstPtr);
            printf("\n\nOutput of bitwise_AND operation:\n");
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
        Rpp8u *srcPtr1 = (Rpp8u *)malloc(channel * srcSize.height * srcSize.width * sizeof(Rpp8u));
        Rpp8u *srcPtr2 = (Rpp8u *)malloc(channel * srcSize.height * srcSize.width * sizeof(Rpp8u));
        Rpp8u *dstPtr = (Rpp8u *)malloc(channel * srcSize.height * srcSize.width * sizeof(Rpp8u));
        int *intSrcPtr1 = (int *)malloc(channel * srcSize.height * srcSize.width * sizeof(int));
        int *intSrcPtr2 = (int *)malloc(channel * srcSize.height * srcSize.width * sizeof(int));
        if (type == 1)
        {
            printf("\n\n\n\nEnter elements in array 1 of size %d x %d x %d in planar format: \n", channel, srcSize.height, srcSize.width);
            inputPlanar(intSrcPtr1, srcSize, channel);
            cast(intSrcPtr1, srcPtr1, srcSize, channel);
            printf("\n\nInput 1:\n");
            displayPlanar(srcPtr1, srcSize, channel);
            printf("\n\n\n\nEnter elements in array 2 of size %d x %d x %d in planar format: \n", channel, srcSize.height, srcSize.width);
            inputPlanar(intSrcPtr2, srcSize, channel);
            cast(intSrcPtr2, srcPtr2, srcSize, channel);
            printf("\n\nInput 2:\n");
            displayPlanar(srcPtr2, srcSize, channel);
            if (channel == 1)
            {
                rppi_bitwise_AND_u8_pln1_host(srcPtr1, srcPtr2, srcSize, dstPtr);
            }
            else if (channel == 3)
            {
                rppi_bitwise_AND_u8_pln3_host(srcPtr1, srcPtr2, srcSize, dstPtr);
            }
            printf("\n\nOutput of bitwise_AND operation:\n");
            displayPlanar(dstPtr, srcSize, channel);
        }
        else if (type == 2)
        {
            printf("\n\n\n\nEnter elements in array 1 of size %d x %d x %d in packed format: \n", channel, srcSize.height, srcSize.width);
            inputPacked(intSrcPtr1, srcSize, channel);
            cast(intSrcPtr1, srcPtr1, srcSize, channel);
            printf("\n\nInput 1:\n");
            displayPacked(srcPtr1, srcSize, channel);
            printf("\n\n\n\nEnter elements in array 2 of size %d x %d x %d in packed format: \n", channel, srcSize.height, srcSize.width);
            inputPacked(intSrcPtr2, srcSize, channel);
            cast(intSrcPtr2, srcPtr2, srcSize, channel);
            printf("\n\nInput 2:\n");
            displayPacked(srcPtr2, srcSize, channel);
            if (channel == 1)
            {
                rppi_bitwise_AND_u8_pln1_host(srcPtr1, srcPtr2, srcSize, dstPtr);
            }
            else if (channel == 3)
            {
                rppi_bitwise_AND_u8_pkd3_host(srcPtr1, srcPtr2, srcSize, dstPtr);
            }
            printf("\n\nOutput of bitwise_AND operation:\n");
            displayPacked(dstPtr, srcSize, channel);
        }
    }
}