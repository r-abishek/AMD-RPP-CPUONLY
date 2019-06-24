// rppi_bilateral_filter

// Uncomment the segment below to get this standalone to work for basic unit testing

#include "rppdefs.h"
#include "rppi_arithmetic_and_logical_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include "cpu/rpp_cpu_inputAndDisplay.hpp"
#include "cpu/host_bilateral_filter.hpp"
 
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;





RppStatus
rppi_bilateral_filter_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
                             Rpp32s diameter, Rpp64f sigmaI, Rpp64f sigmaS)
{
    bilateral_filter_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                                    diameter, sigmaI, sigmaS,
                                    RPPI_CHN_PLANAR, 1);

    return RPP_SUCCESS;

}

RppStatus
rppi_bilateral_filter_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
                             Rpp32s diameter, Rpp64f sigmaI, Rpp64f sigmaS)
{
    bilateral_filter_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                                    diameter, sigmaI, sigmaS,
                                    RPPI_CHN_PLANAR, 3);

    return RPP_SUCCESS;

}

RppStatus
rppi_bilateral_filter_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
                             Rpp32s diameter, Rpp64f sigmaI, Rpp64f sigmaS)
{
    bilateral_filter_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                                    diameter, sigmaI, sigmaS,
                                    RPPI_CHN_PACKED, 3);

    return RPP_SUCCESS;

}





int main(int argc, char** argv)
{
    RppiSize srcSize;
    unsigned int channel;
    Rpp32s diameter = 5;
    Rpp64f sigmaI = 80.0, sigmaS = 60.0;
     
    int input;
    printf("\nEnter input: 1 = image, 2 = pixel values: ");
    scanf("%d", &input);

    if (input == 1)
    {
        if ( argc != 2 )
        {
            printf("usage: DisplayImage.out <Image_Path>\n");
            return -1;
        }

        Mat imageIn;
        imageIn = imread( argv[1], 1 );

        if ( !imageIn.data )
        {
            printf("No image data \n");
            return -1;
        }

        Mat imageOutOpenCV;
        bilateralFilter(imageIn, imageOutOpenCV, diameter, sigmaI, sigmaS);

        srcSize.height = imageIn.rows;
        srcSize.width = imageIn.cols;
        channel = imageIn.channels();
        Rpp8u *srcPtr = imageIn.data;
        Rpp8u *dstPtr = (Rpp8u *)malloc(channel * srcSize.height * srcSize.width * sizeof(Rpp8u));
        rppi_bilateral_filter_u8_pkd3_host(srcPtr, srcSize, dstPtr, diameter, sigmaI, sigmaS);
        Mat imageOut(srcSize.height, srcSize.width, CV_8UC3, dstPtr);

        Mat images(imageIn.rows, imageIn.cols*3, imageIn.type());
        imageIn.copyTo(images(cv::Rect(0,0, imageIn.cols, imageIn.rows)));
        imageOutOpenCV.copyTo(images(cv::Rect(imageIn.cols,0, imageIn.cols, imageIn.rows)));
        imageOut.copyTo(images(cv::Rect(imageIn.cols*2,0, imageIn.cols, imageIn.rows)));

        namedWindow("Input, OpenCV function Output, Created function Output", WINDOW_NORMAL );
        imshow("Input, OpenCV function Output, Created function Output", images);

        waitKey(0);

        return 0;
    }

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
        rppi_bilateral_filter_u8_pln1_host(srcPtr, srcSize, dstPtr, diameter, sigmaI, sigmaS);
        printf("\n\nOutput of bilateral filtering:\n");
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
            rppi_bilateral_filter_u8_pln3_host(srcPtr, srcSize, dstPtr, diameter, sigmaI, sigmaS);
            printf("\n\nOutput of bilateral filtering:\n");
            displayPlanar(dstPtr, srcSize, channel);
        }
        else if (type == 2)
        {
            Rpp8u srcPtr[36] = {255, 130, 65, 254, 129, 66, 253, 128, 67, 252, 127, 68, 251, 126, 69, 250, 117, 70, 249, 113, 71, 248, 121, 72, 247, 127, 13, 246, 111, 24, 245, 100, 15, 244, 108, 16};
            Rpp8u dstPtr[36] = {0};
            printf("\n\nInput:\n");
            displayPacked(srcPtr, srcSize, channel);
            rppi_bilateral_filter_u8_pkd3_host(srcPtr, srcSize, dstPtr, diameter, sigmaI, sigmaS);
            printf("\n\nOutput of bilateral filtering:\n");
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
                rppi_bilateral_filter_u8_pln1_host(srcPtr, srcSize, dstPtr, diameter, sigmaI, sigmaS);
            }
            else if (channel == 3)
            {
                rppi_bilateral_filter_u8_pln3_host(srcPtr, srcSize, dstPtr, diameter, sigmaI, sigmaS);
            }
            printf("\n\nOutput of bilateral filtering:\n");
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
                rppi_bilateral_filter_u8_pln1_host(srcPtr, srcSize, dstPtr, diameter, sigmaI, sigmaS);
            }
            else if (channel == 3)
            {
                rppi_bilateral_filter_u8_pkd3_host(srcPtr, srcSize, dstPtr, diameter, sigmaI, sigmaS);
            }
            printf("\n\nOutput of bilateral filtering:\n");
            displayPacked(dstPtr, srcSize, channel);
        }
    }
}