// rppi_planar2packed

// Uncomment the segment below to get this standalone to work for basic unit testing

#include "rppdefs.h"
#include "rppi_image_augumentation_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include "cpu/rpp_cpu_inputAndDisplay.hpp"
#include "cpu/host_packed2planar.hpp"
#include "cpu/host_planar2packed.hpp"
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
using namespace std::chrono;





RppStatus 
rppi_planar2packed_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    planar2packed_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                     1);
    return RPP_SUCCESS;
}

RppStatus
rppi_planar2packed_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    planar2packed_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                     3);
    return RPP_SUCCESS;
}

RppStatus 
rppi_packed2planar_u8_pkd1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    packed2planar_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                     1);
    return RPP_SUCCESS;
}

RppStatus
rppi_packed2planar_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr)
{
    packed2planar_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
                     3);
    return RPP_SUCCESS;
}





int main(int argc, char** argv)
{
    RppiSize srcSize;
    unsigned int channel;
    
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

        srcSize.height = imageIn.rows;
        srcSize.width = imageIn.cols;
        channel = imageIn.channels();
        Rpp8u *srcPtr = imageIn.data;
        Rpp8u *dstPtr = (Rpp8u *)malloc(channel * srcSize.height * srcSize.width * sizeof(Rpp8u));
        Rpp8u *srcPtrTemp = (Rpp8u *)malloc(channel * srcSize.height * srcSize.width * sizeof(Rpp8u));

        rppi_packed2planar_u8_pkd3_host(srcPtr, srcSize, dstPtr);

        for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
        {
            srcPtrTemp[i] = dstPtr[i];
        }
        
        auto start = high_resolution_clock::now();
        rppi_planar2packed_u8_pln3_host(srcPtrTemp, srcSize, dstPtr);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "\nTime taken (milliseconds) = " << duration.count() << endl;

        Mat imageOut(srcSize.height, srcSize.width, CV_8UC3, dstPtr);

        Mat images(imageIn.rows, imageIn.cols*2, imageIn.type());
        imageIn.copyTo(images(cv::Rect(0,0, imageIn.cols, imageIn.rows)));
        imageOut.copyTo(images(cv::Rect(imageIn.cols,0, imageIn.cols, imageIn.rows)));

        namedWindow("Input and Output Images", WINDOW_NORMAL );
        imshow("Input and Output Images", images);

        waitKey(0);

        return 0;
    }
    
    int matrix;
    printf("\nEnter matrix input style: 1 = default 1 channel (1x3x4), 2 = default 3 channel (3x3x4), 3 = customized: ");
    scanf("%d", &matrix);
    
    if (matrix == 1)
    {
        channel = 1;
        srcSize.height = 3;
        srcSize.width = 4;
        Rpp8u srcPtr[12] = {130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108};
        Rpp8u dstPtr[12] = {0};
        printf("\n\nInput:\n");
        displayPlanar(srcPtr, srcSize, channel);

        auto start = high_resolution_clock::now();
        rppi_planar2packed_u8_pln1_host(srcPtr, srcSize, dstPtr);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        cout << "\nTime taken (nanoseconds) = " << duration.count() << endl;

        printf("\n\nOutput of planar2packed:\n");
        displayPacked(dstPtr, srcSize, channel);
    }
    else if (matrix == 2)
    {
        channel = 3;
        srcSize.height = 3;
        srcSize.width = 4;
        Rpp8u srcPtr[36] = {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108, 65, 66, 67, 68, 69, 70, 71, 72, 13, 24, 15, 16};
        Rpp8u dstPtr[36] = {0};
        printf("\n\nInput:\n");
        displayPlanar(srcPtr, srcSize, channel);

        auto start = high_resolution_clock::now();
        rppi_planar2packed_u8_pln3_host(srcPtr, srcSize, dstPtr);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        cout << "\nTime taken (nanoseconds) = " << duration.count() << endl;

        printf("\n\nOutput of planar2packed:\n");
        displayPacked(dstPtr, srcSize, channel);
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

        printf("\n\n\n\nEnter elements in array of size %d x %d x %d in planar format: \n", channel, srcSize.height, srcSize.width);
        inputPlanar(intSrcPtr, srcSize, channel);
        cast(intSrcPtr, srcPtr, srcSize, channel);
        printf("\n\nInput:\n");
        displayPlanar(srcPtr, srcSize, channel);
        if (channel == 1)
        {
            auto start = high_resolution_clock::now();
            rppi_planar2packed_u8_pln1_host(srcPtr, srcSize, dstPtr);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<nanoseconds>(stop - start);
            cout << "\nTime taken (nanoseconds) = " << duration.count() << endl;
        }
        else if (channel == 3)
        {
            auto start = high_resolution_clock::now();
            rppi_planar2packed_u8_pln3_host(srcPtr, srcSize, dstPtr);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<nanoseconds>(stop - start);
            cout << "\nTime taken (nanoseconds) = " << duration.count() << endl;
        }
        printf("\n\nOutput of planar2packed:\n");
        displayPacked(dstPtr, srcSize, channel);
    }
}