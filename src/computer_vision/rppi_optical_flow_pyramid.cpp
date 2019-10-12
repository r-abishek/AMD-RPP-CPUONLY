// rppi_optical_flow_pyramid

// Uncomment the segment below to get this standalone to work for basic unit testing

#include "rppdefs.h"
#include "rppi_computer_vision.h"
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include "cpu/rpp_cpu_input_and_display.hpp"
#include <cpu/rpp_cpu_pixel_arrangement_conversions.hpp>
#include "cpu/host_optical_flow_pyramid.hpp"
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
using namespace std::chrono;




RppStatus
rppi_optical_flow_pyramid_u8_pln1_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr, 
                                       Rpp32u* oldPoints, Rpp32u* newPointsEstimates, Rpp32u* newPoints, 
                                       Rpp32u numPoints, Rpp32f threshold, Rpp32u numIterations, Rpp32u kernelSize)
{
    optical_flow_pyramid_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr1), static_cast<Rpp8u*>(srcPtr2), srcSize, static_cast<Rpp8u*>(dstPtr), 
                                     oldPoints, newPointsEstimates, newPoints, 
                                     numPoints, threshold, numIterations, kernelSize, 
                                     RPPI_CHN_PLANAR, 1);

    return RPP_SUCCESS;
}

RppStatus
rppi_optical_flow_pyramid_u8_pln3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr, 
                                       Rpp32u* oldPoints, Rpp32u* newPointsEstimates, Rpp32u* newPoints, 
                                       Rpp32u numPoints, Rpp32f threshold, Rpp32u numIterations, Rpp32u kernelSize)
{
    optical_flow_pyramid_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr1), static_cast<Rpp8u*>(srcPtr2), srcSize, static_cast<Rpp8u*>(dstPtr), 
                                     oldPoints, newPointsEstimates, newPoints, 
                                     numPoints, threshold, numIterations, kernelSize, 
                                     RPPI_CHN_PLANAR, 3);

    return RPP_SUCCESS;
}

RppStatus
rppi_optical_flow_pyramid_u8_pkd3_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppiSize srcSize, RppPtr_t dstPtr, 
                                       Rpp32u* oldPoints, Rpp32u* newPointsEstimates, Rpp32u* newPoints, 
                                       Rpp32u numPoints, Rpp32f threshold, Rpp32u numIterations, Rpp32u kernelSize)
{
    optical_flow_pyramid_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr1), static_cast<Rpp8u*>(srcPtr2), srcSize, static_cast<Rpp8u*>(dstPtr), 
                                     oldPoints, newPointsEstimates, newPoints, 
                                     numPoints, threshold, numIterations, kernelSize, 
                                     RPPI_CHN_PACKED, 3);

    return RPP_SUCCESS;
}





int main(int argc, char** argv)
{
    RppiSize srcSize;
    unsigned int channel;

    Rpp32u oldPoints[2] = {54, 272};
    //Rpp32u oldPoints[2] = {93, 71};
    
    Rpp32u newPointsEstimates[2] = {58, 268};
    //Rpp32u newPointsEstimates[2] = {53, 271};
    //Rpp32u newPointsEstimates[2] = {93, 71};
    
    Rpp32u newPoints[2] = {0, 0};
    Rpp32u numPoints = 1;
    //Rpp32f threshold = 0.005;
    Rpp32f threshold = 500;
    Rpp32u numIterations = 20;
    Rpp32u kernelSize = 9;
    
    int input;
    printf("\nEnter input: 1 = images, 2 = pixel values: ");
    scanf("%d", &input);

    int type;
    printf("\nEnter type of arrangement: 1 = planar, 2 = packed: ");
    scanf("%d", &type);

    if (input == 1)
    {
        if ( argc != 3 )
        {
            printf("usage: DisplayImage.out <Image_Path>\n");
            return -1;
        }

        do
        {   printf("\nThe image input/inputs can be interpreted as 1 or 3 channel (greyscale or RGB). Please choose - only 1 or 3: ");
            scanf("%d", &channel);
        }while (channel != 1 && channel != 3);

        Mat imageIn1, imageIn2;

        if (channel == 1)
        {
            imageIn1 = imread( argv[1], 0 );
            imageIn2 = imread( argv[2], 0 );
        }
        else if (channel ==3)
        {
            imageIn1 = imread( argv[1], 1 );
            imageIn2 = imread( argv[2], 1 );
        }

        if ( !imageIn1.data || !imageIn2.data)
        {
            printf("No image data \n");
            return -1;
        }

        if ((imageIn1.rows != imageIn2.rows) || (imageIn1.cols != imageIn2.cols) || (imageIn1.channels() != imageIn2.channels()))
        {
            printf("Both images must have the same height, width and channels \n");
            return -1;
        }

        srcSize.height = imageIn1.rows;
        srcSize.width = imageIn1.cols;

        printf("\nInput1 Height - %d, Input Width - %d, Input Channels - %d\n", imageIn1.rows, imageIn1.cols, channel);
        Rpp8u *srcPtr1 = imageIn1.data;
        
        printf("\nInput2 Height - %d, Input Width - %d, Input Channels - %d\n", imageIn2.rows, imageIn2.cols, channel);
        Rpp8u *srcPtr2 = imageIn2.data;

        Rpp32u* newPoints = (Rpp32u*) calloc(numPoints * 2, sizeof(Rpp32u));
        
        auto start = high_resolution_clock::now();
        auto stop = high_resolution_clock::now();

        RppiSize dstSize;
        dstSize.height = srcSize.height;
        dstSize.width = srcSize.width;
        Rpp8u* dstPtr = (Rpp8u*) calloc(dstSize.height * dstSize.width, sizeof(Rpp8u));
        Mat imageOut;

        if (type == 1)
        {   
            if (channel == 1)
            {
                printf("\nExecuting pln1...\n");
                start = high_resolution_clock::now();
                rppi_optical_flow_pyramid_u8_pln1_host(srcPtr1, srcPtr2, srcSize, dstPtr, oldPoints, newPointsEstimates, newPoints, 
                                                       numPoints, threshold, numIterations, kernelSize);
                stop = high_resolution_clock::now();

                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC1, dstPtr);
                
            }
            else if (channel == 3)
            {
                printf("\nExecuting pln3...\n");
                Rpp8u *srcPtr1Temp = (Rpp8u *)calloc(channel * srcSize.height * srcSize.width, sizeof(Rpp8u));
                Rpp8u *srcPtr2Temp = (Rpp8u *)calloc(channel * srcSize.height * srcSize.width, sizeof(Rpp8u));
                rppi_packed_to_planar_u8_pkd3_host(srcPtr1, srcSize, srcPtr1Temp);
                rppi_packed_to_planar_u8_pkd3_host(srcPtr2, srcSize, srcPtr2Temp);

                start = high_resolution_clock::now();
                rppi_optical_flow_pyramid_u8_pln3_host(srcPtr1Temp, srcPtr2Temp, srcSize, dstPtr, oldPoints, newPointsEstimates, newPoints, 
                                                       numPoints, threshold, numIterations, kernelSize);
                stop = high_resolution_clock::now();

                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC1, dstPtr);
            }
        }
        else if (type == 2)
        {   
            if (channel == 1)
            {
                printf("\nExecuting pln1 for pkd1...\n");
                start = high_resolution_clock::now();
                rppi_optical_flow_pyramid_u8_pln1_host(srcPtr1, srcPtr2, srcSize, dstPtr, oldPoints, newPointsEstimates, newPoints, 
                                                       numPoints, threshold, numIterations, kernelSize);
                stop = high_resolution_clock::now();

                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC1, dstPtr);
            }
            else if (channel ==3)
            {
                printf("\nExecuting pkd3...\n");
                start = high_resolution_clock::now();
                rppi_optical_flow_pyramid_u8_pkd3_host(srcPtr1, srcPtr2, srcSize, dstPtr, oldPoints, newPointsEstimates, newPoints, 
                                                       numPoints, threshold, numIterations, kernelSize);
                stop = high_resolution_clock::now();

                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC1, dstPtr);
            }
        }

        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "\nTime taken (milliseconds) = " << duration.count() << endl;

        Rpp32u *oldPointsTemp, *newPointsTemp;
        oldPointsTemp = oldPoints;
        newPointsTemp = newPoints;
        printf("\nOld Points vs New Points:");
        for (int i = 0; i < numPoints; i++)
        {
            printf("\n(%d,%d) -> (%d,%d)", *oldPointsTemp, *(oldPointsTemp + 1), *newPointsTemp, *(newPointsTemp + 1));
            oldPointsTemp += 2;
            newPointsTemp += 2;
        }
        printf("\n");

        Mat images(imageIn1.rows, imageIn1.cols*2, imageIn1.type());
        imageIn1.copyTo(images(cv::Rect(0,0, imageIn1.cols, imageIn1.rows)));
        imageIn2.copyTo(images(cv::Rect(imageIn1.cols,0, imageIn1.cols, imageIn1.rows)));
        namedWindow("Input Images", WINDOW_NORMAL );
        imshow("Input Images", images);
        namedWindow("Output Image", WINDOW_NORMAL );
        imshow("Output Image", imageOut);
        waitKey(0);

        return 0;
    }

    printf("\nThis option is not available for optical flow pyramid!");
/*
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
        rppi_optical_flow_pyramid_u8_pln1_host(srcPtr, srcSize, dstPtr, gaussianKernelSize, stdDev, kernelSize, kValue, threshold, nonmaxKernelSize);
        printf("\n\nOutput of optical_flow_pyramid:\n");
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
            rppi_optical_flow_pyramid_u8_pln3_host(srcPtr, srcSize, dstPtr, gaussianKernelSize, stdDev, kernelSize, kValue, threshold, nonmaxKernelSize);
            printf("\n\nOutput of optical_flow_pyramid:\n");
            displayPlanar(dstPtr, srcSize, channel);
        }
        else if (type == 2)
        {
            Rpp8u srcPtr[36] = {255, 130, 65, 254, 129, 66, 253, 128, 67, 252, 127, 68, 251, 126, 69, 250, 117, 70, 249, 113, 71, 248, 121, 72, 247, 127, 13, 246, 111, 24, 245, 100, 15, 244, 108, 16};
            Rpp8u dstPtr[36] = {0};
            printf("\n\nInput:\n");
            displayPacked(srcPtr, srcSize, channel);
            rppi_optical_flow_pyramid_u8_pkd3_host(srcPtr, srcSize, dstPtr, gaussianKernelSize, stdDev, kernelSize, kValue, threshold, nonmaxKernelSize);
            printf("\n\nOutput of optical_flow_pyramid:\n");
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
        Rpp8u *srcPtr = (Rpp8u *)calloc(channel * srcSize.height * srcSize.width, sizeof(Rpp8u));
        Rpp8u *dstPtr = (Rpp8u *)calloc(channel * srcSize.height * srcSize.width, sizeof(Rpp8u));
        int *intSrcPtr = (int *)calloc(channel * srcSize.height * srcSize.width, sizeof(int));
        if (type == 1)
        {
            printf("\n\n\n\nEnter elements in array of size %d x %d x %d in planar format: \n", channel, srcSize.height, srcSize.width);
            inputPlanar(intSrcPtr, srcSize, channel);
            cast(intSrcPtr, srcPtr, srcSize, channel);
            printf("\n\nInput:\n");
            displayPlanar(srcPtr, srcSize, channel);
            if (channel == 1)
            {
                rppi_optical_flow_pyramid_u8_pln1_host(srcPtr, srcSize, dstPtr, gaussianKernelSize, stdDev, kernelSize, kValue, threshold, nonmaxKernelSize);
            }
            else if (channel == 3)
            {
                rppi_optical_flow_pyramid_u8_pln3_host(srcPtr, srcSize, dstPtr, gaussianKernelSize, stdDev, kernelSize, kValue, threshold, nonmaxKernelSize);
            }
            printf("\n\nOutput of optical_flow_pyramid:\n");
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
                rppi_optical_flow_pyramid_u8_pln1_host(srcPtr, srcSize, dstPtr, gaussianKernelSize, stdDev, kernelSize, kValue, threshold, nonmaxKernelSize);
            }
            else if (channel == 3)
            {
                rppi_optical_flow_pyramid_u8_pkd3_host(srcPtr, srcSize, dstPtr, gaussianKernelSize, stdDev, kernelSize, kValue, threshold, nonmaxKernelSize);
            }
            printf("\n\nOutput of optical_flow_pyramid:\n");
            displayPacked(dstPtr, srcSize, channel);
        }
    }
*/
}
