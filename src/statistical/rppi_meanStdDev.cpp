// rppi_meanStd

// Uncomment the segment below to get this standalone to work for basic unit testing

#include "rppdefs.h"
#include "rppi_statistics_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include "cpu/rpp_cpu_inputAndDisplay.hpp"
#include "cpu/host_meanStdDev.hpp"
 
using namespace std;





RppStatus
rppi_meanStd_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, Rpp32f* mean, Rpp32f* stdDev)
{
    meanStd_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, 
                       mean, stdDev, 
                       RPPI_CHN_PLANAR, 1);

    return RPP_SUCCESS;

}

RppStatus
rppi_meanStd_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, Rpp32f* mean, Rpp32f* stdDev)
{
    meanStd_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, 
                       mean, stdDev, 
                       RPPI_CHN_PLANAR, 3);

    return RPP_SUCCESS;

}

RppStatus
rppi_meanStd_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, Rpp32f* mean, Rpp32f* stdDev)
{
    meanStd_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, 
                       mean, stdDev, 
                       RPPI_CHN_PACKED, 3);

    return RPP_SUCCESS;

}





int main()
{
    RppiSize srcSize;
    unsigned int channel;
    Rpp32f mean, stdDev;
     
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
        Rpp8u maskPtr[12] = {0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0};
        //Rpp8u dstPtr[12] = {0};
        printf("\n\nInput:\n");
        displayPlanar(srcPtr, srcSize, channel);
        rppi_meanStd_u8_pln1_host(srcPtr, srcSize, &mean, &stdDev);
        printf("\n\nOutput of meanStd operation:\n");
        printf("\nMean = %f, StdDev = %f\n", mean, stdDev);
        //displayPlanar(dstPtr, srcSize, channel);
    }
    else if (matrix == 2)
    {
        channel = 3;
        srcSize.height = 3;
        srcSize.width = 4;
        if (type == 1)
        {
            Rpp8u srcPtr[36] = {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108, 65, 66, 67, 68, 69, 70, 71, 72, 13, 24, 15, 16};
            Rpp8u maskPtr[36] = {0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0};
            //Rpp8u dstPtr[36] = {0};
            printf("\n\nInput:\n");
            displayPlanar(srcPtr, srcSize, channel);
            rppi_meanStd_u8_pln3_host(srcPtr, srcSize, &mean, &stdDev);
            printf("\n\nOutput of meanStd operation:\n");
            printf("\nMean = %f, StdDev = %f\n", mean, stdDev);
            //displayPlanar(dstPtr, srcSize, channel);
        }
        else if (type == 2)
        {
            Rpp8u srcPtr[36] = {255, 130, 65, 254, 129, 66, 253, 128, 67, 252, 127, 68, 251, 126, 69, 250, 117, 70, 249, 113, 71, 248, 121, 72, 247, 127, 13, 246, 111, 24, 245, 100, 15, 244, 108, 16};
            Rpp8u maskPtr[36] = {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            //Rpp8u dstPtr[36] = {0};
            printf("\n\nInput:\n");
            displayPacked(srcPtr, srcSize, channel);
            rppi_meanStd_u8_pkd3_host(srcPtr, srcSize, &mean, &stdDev);
            printf("\n\nOutput of meanStd operation:\n");
            printf("\nMean = %f, StdDev = %f\n", mean, stdDev);
            //displayPacked(dstPtr, srcSize, channel);
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
        //Rpp8u *dstPtr = (Rpp8u *)malloc(channel * srcSize.height * srcSize.width * sizeof(Rpp8u));
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
                rppi_meanStd_u8_pln1_host(srcPtr, srcSize, &mean, &stdDev);
            }
            else if (channel == 3)
            {
                rppi_meanStd_u8_pln3_host(srcPtr, srcSize, &mean, &stdDev);
            }
            printf("\n\nOutput of meanStd operation:\n");
            printf("\nMean = %f, StdDev = %f\n", mean, stdDev);
            //displayPlanar(dstPtr, srcSize, channel);
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
                rppi_meanStd_u8_pln1_host(srcPtr, srcSize, &mean, &stdDev);
            }
            else if (channel == 3)
            {
                rppi_meanStd_u8_pkd3_host(srcPtr, srcSize, &mean, &stdDev);
            }
            printf("\n\nOutput of meanStd operation:\n");
            printf("\nMean = %f, StdDev = %f\n", mean, stdDev);
            //displayPacked(dstPtr, srcSize, channel);
        }
    }
}
