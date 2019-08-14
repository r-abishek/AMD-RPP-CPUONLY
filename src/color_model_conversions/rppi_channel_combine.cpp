// rppi_channel_combine

// Uncomment the segment below to get this standalone to work for basic unit testing

#include "rppdefs.h"
#include "rppi_color_model_conversions.h"
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include "cpu/rpp_cpu_input_and_display.hpp"
#include <cpu/rpp_cpu_pixel_arrangement_conversions.hpp>
#include "cpu/host_channel_combine.hpp"
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
using namespace std::chrono;





RppStatus
rppi_channel_combine_u8_pln3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppPtr_t srcPtr3, RppiSize srcSize, RppPtr_t dstPtr)
{
    channel_combine_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr1), static_cast<Rpp8u*>(srcPtr2), static_cast<Rpp8u*>(srcPtr3), 
                                srcSize, static_cast<Rpp8u*>(dstPtr), 
                                RPPI_CHN_PLANAR, 3);
    return RPP_SUCCESS;
}

RppStatus
rppi_channel_combine_u8_pkd3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppPtr_t srcPtr3, RppiSize srcSize, RppPtr_t dstPtr)
{
    channel_combine_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr1), static_cast<Rpp8u*>(srcPtr2), static_cast<Rpp8u*>(srcPtr3), 
                                srcSize, static_cast<Rpp8u*>(dstPtr), 
                                RPPI_CHN_PACKED, 3);
    return RPP_SUCCESS;
}





int main(int argc, char** argv)
{
    RppiSize srcSize, dstSize;
    unsigned int channel = 1, channelOut = 3;

    int input;
    printf("\nEnter input: 1 = image, 2 = pixel values: ");
    scanf("%d", &input);

    int type;
    printf("\nEnter type of arrangement: 1 = planar, 2 = packed: ");
    scanf("%d", &type);

    if (input == 1)
    {
        if ( argc != 4 )
        {
            printf("usage: DisplayImage.out <Image1_Path> <Image2_Path> <Image3_Path>\n");
            return -1;
        }

        Mat imageIn1, imageIn2, imageIn3;

        imageIn1 = imread( argv[1], 0 );
        imageIn2 = imread( argv[2], 0 );
        imageIn3 = imread( argv[3], 0 );

        if ( !imageIn1.data )
        {
            printf("No image1 data \n");
            return -1;
        }
        if ( !imageIn2.data )
        {
            printf("No image2 data \n");
            return -1;
        }
        if ( !imageIn3.data )
        {
            printf("No image3 data \n");
            return -1;
        }

        srcSize.height = imageIn1.rows;
        srcSize.width = imageIn1.cols;
        dstSize.height = srcSize.height;
        dstSize.width = srcSize.width;

        printf("\nInput Height - %d, Input Width - %d, Input Channels - %d\n", srcSize.height, srcSize.width, channel);
        Rpp8u *srcPtr1 = imageIn1.data;
        Rpp8u *srcPtr2 = imageIn2.data;
        Rpp8u *srcPtr3 = imageIn3.data;
        
        printf("\nOutput Height - %d, Output Width - %d, Output Channels - %d\n", dstSize.height, dstSize.width, channelOut);
        Rpp8u *dstPtr = (Rpp8u *)calloc(channelOut * dstSize.height * dstSize.width, sizeof(Rpp8u));
        
        auto start = high_resolution_clock::now();
        auto stop = high_resolution_clock::now();

        Mat imageOut;

        if (type == 1)
        {   
            printf("\nExecuting pln3...\n");
            Rpp8u *dstPtrTemp = (Rpp8u *)calloc(channelOut * srcSize.height * srcSize.width, sizeof(Rpp8u));

            start = high_resolution_clock::now();
            rppi_channel_combine_u8_pln3_host(srcPtr1, srcPtr2, srcPtr3, srcSize, dstPtrTemp);
            stop = high_resolution_clock::now();

            rppi_planar_to_packed_u8_pln3_host(dstPtrTemp, srcSize, dstPtr);

            imageOut = Mat(dstSize.height, dstSize.width, CV_8UC3, dstPtr);
        }
        else if (type == 2)
        {   
            printf("\nExecuting pkd3...\n");
            start = high_resolution_clock::now();
            rppi_channel_combine_u8_pkd3_host(srcPtr1, srcPtr2, srcPtr3, srcSize, dstPtr);
            stop = high_resolution_clock::now();

            imageOut = Mat(dstSize.height, dstSize.width, CV_8UC3, dstPtr);
        }

        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "\nTime taken (milliseconds) = " << duration.count() << endl;

        Mat images(imageIn1.rows, imageIn1.cols*3, imageIn1.type());
        imageIn1.copyTo(images(cv::Rect(0,0, imageIn1.cols, imageIn1.rows)));
        imageIn2.copyTo(images(cv::Rect(imageIn1.cols,0, imageIn1.cols, imageIn1.rows)));
        imageIn3.copyTo(images(cv::Rect(2 * imageIn1.cols,0, imageIn1.cols, imageIn1.rows)));

        namedWindow("Input Images", WINDOW_NORMAL );
        imshow("Input Images", images);

        namedWindow("Output Image", WINDOW_NORMAL );
        imshow("Output Image", imageOut);

        waitKey(0);

        return 0;
    }
     
    int matrix;
    printf("\nEnter matrix input style: 1 = default 1 channel (1x3x4), 2 = default 3 channel (3x3x4), 3 = customized: ");
    scanf("%d", &matrix);

    if (matrix == 1)
    {
        printf("\n\nToo complicated! Not Needed!\n");
    }
    else if (matrix == 2)
    {
        printf("\n\nToo complicated! Not Needed!\n");
    }
    else if (matrix == 3)
    {
        printf("\n\nToo complicated! Not Needed!\n");
    }
}
