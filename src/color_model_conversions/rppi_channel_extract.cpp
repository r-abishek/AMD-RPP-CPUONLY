// rppi_channel_extract

// Uncomment the segment below to get this standalone to work for basic unit testing

#include "rppdefs.h"
#include "rppi_color_model_conversions.h"
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include "cpu/rpp_cpu_input_and_display.hpp"
#include <cpu/rpp_cpu_pixel_arrangement_conversions.hpp>
#include "cpu/host_channel_extract.hpp"
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
using namespace std::chrono;





RppStatus
rppi_channel_extract_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, 
                                  Rpp32u extractChannelNumber)
{
    channel_extract_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                     extractChannelNumber, 
                     RPPI_CHN_PLANAR, 3);
    return RPP_SUCCESS;
}

RppStatus
rppi_channel_extract_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, 
                                  Rpp32u extractChannelNumber)
{
    channel_extract_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                     extractChannelNumber, 
                     RPPI_CHN_PACKED, 3);
    return RPP_SUCCESS;
}





int main(int argc, char** argv)
{
    RppiSize srcSize, dstSize;
    unsigned int channel = 3, extractChannelNumber, channelOut = 1;

    do
    {
        printf("\nEnter channel number to extract: ");
        scanf("%d", &extractChannelNumber);
    } while (extractChannelNumber != 0 && extractChannelNumber != 1 && extractChannelNumber != 2);

    int input;
    printf("\nEnter input: 1 = image, 2 = pixel values: ");
    scanf("%d", &input);

    int type;
    printf("\nEnter type of arrangement: 1 = planar, 2 = packed: ");
    scanf("%d", &type);

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

        srcSize.height = imageIn.rows;
        srcSize.width = imageIn.cols;
        dstSize.height = srcSize.height;
        dstSize.width = srcSize.width;

        printf("\nInput Height - %d, Input Width - %d, Input Channels - %d\n", srcSize.height, srcSize.width, channel);
        Rpp8u *srcPtr = imageIn.data;
        
        printf("\nOutput Height - %d, Output Width - %d, Output Channels - %d\n", dstSize.height, dstSize.width, channelOut);
        Rpp8u *dstPtr = (Rpp8u *)calloc(channelOut * dstSize.height * dstSize.width, sizeof(Rpp8u));
        
        auto start = high_resolution_clock::now();
        auto stop = high_resolution_clock::now();

        Mat imageOut;

        if (type == 1)
        {   
            printf("\nExecuting pln3...\n");
            Rpp8u *srcPtrTemp = (Rpp8u *)calloc(channel * srcSize.height * srcSize.width, sizeof(Rpp8u));
            rppi_packed_to_planar_u8_pkd3_host(srcPtr, srcSize, srcPtrTemp);

            start = high_resolution_clock::now();
            rppi_channel_extract_u8_pln3_host(srcPtrTemp, srcSize, dstPtr, extractChannelNumber);
            stop = high_resolution_clock::now();

            imageOut = Mat(dstSize.height, dstSize.width, CV_8UC1, dstPtr);
        }
        else if (type == 2)
        {   
            printf("\nExecuting pkd3...\n");
            start = high_resolution_clock::now();
            rppi_channel_extract_u8_pkd3_host(srcPtr, srcSize, dstPtr, extractChannelNumber);
            stop = high_resolution_clock::now();

            imageOut = Mat(dstSize.height, dstSize.width, CV_8UC1, dstPtr);
        }

        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "\nTime taken (milliseconds) = " << duration.count() << endl;

        //Mat images(imageIn.rows, imageIn.cols*2, imageIn.type());
        //imageIn.copyTo(images(cv::Rect(0,0, imageIn.cols, imageIn.rows)));
        //imageOut.copyTo(images(cv::Rect(imageIn.cols,0, imageIn.cols, imageIn.rows)));

        //namedWindow("Input and Output Images", WINDOW_NORMAL );
        //imshow("Input and Output Images", images);

        namedWindow("Input Image", WINDOW_NORMAL );
        imshow("Input Image", imageIn);

        namedWindow("Output Image", WINDOW_NORMAL );
        imshow("Output Image", imageOut);

        //imwrite( "googleBlueChannel.jpg", imageOut );

        waitKey(0);

        return 0;
    }
     
    int matrix;
    printf("\nEnter matrix input style: 1 = default 1 channel (1x3x4), 2 = default 3 channel (3x3x4), 3 = customized: ");
    scanf("%d", &matrix);

    if (matrix == 1)
    {
        printf("\n\nSingle channel input for channel extract doesn't make sense!\n");
    }
    else if (matrix == 2)
    {
        srcSize.height = 3;
        srcSize.width = 4;
        if (type == 1)
        {
            Rpp8u srcPtr[36] = {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108, 65, 66, 67, 68, 69, 70, 71, 72, 13, 24, 15, 16};
            Rpp8u dstPtr[12] = {0};
            printf("\n\nInput:\n");
            displayPlanar(srcPtr, srcSize, channel);
            rppi_channel_extract_u8_pln3_host(srcPtr, srcSize, dstPtr, extractChannelNumber);
            printf("\n\nOutput of channel_extract:\n");
            displayPlanar(dstPtr, srcSize, channelOut);
        }
        else if (type == 2)
        {
            Rpp8u srcPtr[36] = {255, 130, 65, 254, 129, 66, 253, 128, 67, 252, 127, 68, 251, 126, 69, 250, 117, 70, 249, 113, 71, 248, 121, 72, 247, 127, 13, 246, 111, 24, 245, 100, 15, 244, 108, 16};
            Rpp8u dstPtr[12] = {0};
            printf("\n\nInput:\n");
            displayPacked(srcPtr, srcSize, channel);
            rppi_channel_extract_u8_pkd3_host(srcPtr, srcSize, dstPtr, extractChannelNumber);
            printf("\n\nOutput of channel_extract:\n");
            displayPacked(dstPtr, srcSize, channelOut);
        } 
    }
    else if (matrix == 3)
    {
        printf("Enter height of image in pixels: ");
        scanf("%d", &srcSize.height);
        printf("Enter width of image in pixels: ");
        scanf("%d", &srcSize.width);
        printf("Channels = %d, Height = %d, Width = %d", channel, srcSize.height, srcSize.width);
        Rpp8u *srcPtr = (Rpp8u *)calloc(channel * srcSize.height * srcSize.width, sizeof(Rpp8u));
        Rpp8u *dstPtr = (Rpp8u *)calloc(channelOut * srcSize.height * srcSize.width, sizeof(Rpp8u));
        int *intSrcPtr = (int *)calloc(channel * srcSize.height * srcSize.width, sizeof(int));
        if (type == 1)
        {
            printf("\n\n\n\nEnter elements in array of size %d x %d x %d in planar format: \n", channel, srcSize.height, srcSize.width);
            inputPlanar(intSrcPtr, srcSize, channel);
            cast(intSrcPtr, srcPtr, srcSize, channel);
            printf("\n\nInput:\n");
            displayPlanar(srcPtr, srcSize, channel);
            rppi_channel_extract_u8_pln3_host(srcPtr, srcSize, dstPtr, extractChannelNumber);
            printf("\n\nOutput of channel_extract:\n");
            displayPlanar(dstPtr, srcSize, channelOut);
        }
        else if (type == 2)
        {
            printf("\n\n\n\nEnter elements in array of size %d x %d x %d in packed format: \n", channel, srcSize.height, srcSize.width);
            inputPacked(intSrcPtr, srcSize, channel);
            cast(intSrcPtr, srcPtr, srcSize, channel);
            printf("\n\nInput:\n");
            displayPacked(srcPtr, srcSize, channel);
            rppi_channel_extract_u8_pkd3_host(srcPtr, srcSize, dstPtr, extractChannelNumber);
            printf("\n\nOutput of channel_extract:\n");
            displayPacked(dstPtr, srcSize, channelOut);
        }
    }
}
