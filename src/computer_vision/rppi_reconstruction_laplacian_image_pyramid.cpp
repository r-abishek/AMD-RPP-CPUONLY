// rppi_reconstruction_laplacian_image_pyramid

// Uncomment the segment below to get this standalone to work for basic unit testing

#include "rppdefs.h"
#include "rppi_computer_vision.h"
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include "cpu/rpp_cpu_input_and_display.hpp"
#include <cpu/rpp_cpu_pixel_arrangement_conversions.hpp>
#include "cpu/host_reconstruction_laplacian_image_pyramid.hpp"
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
using namespace std::chrono;





RppStatus
rppi_reconstruction_laplacian_image_pyramid_u8_pln1_host(RppPtr_t srcPtr1, RppiSize srcSize1, RppPtr_t srcPtr2, RppiSize srcSize2, RppPtr_t dstPtr, 
                                                        Rpp32f stdDev, Rpp32u kernelSize)
{
    reconstruction_laplacian_image_pyramid_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr1), srcSize1, static_cast<Rpp8u*>(srcPtr2), srcSize2, static_cast<Rpp8u*>(dstPtr), 
                                                       stdDev, kernelSize, 
                                                       RPPI_CHN_PLANAR, 1);
    return RPP_SUCCESS;
}

RppStatus
rppi_reconstruction_laplacian_image_pyramid_u8_pln3_host(RppPtr_t srcPtr1, RppiSize srcSize1, RppPtr_t srcPtr2, RppiSize srcSize2, RppPtr_t dstPtr, 
                                                        Rpp32f stdDev, Rpp32u kernelSize)
{
    reconstruction_laplacian_image_pyramid_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr1), srcSize1, static_cast<Rpp8u*>(srcPtr2), srcSize2, static_cast<Rpp8u*>(dstPtr), 
                                                       stdDev, kernelSize, 
                                                       RPPI_CHN_PLANAR, 3);
    return RPP_SUCCESS;
}

RppStatus
rppi_reconstruction_laplacian_image_pyramid_u8_pkd3_host(RppPtr_t srcPtr1, RppiSize srcSize1, RppPtr_t srcPtr2, RppiSize srcSize2, RppPtr_t dstPtr, 
                                                        Rpp32f stdDev, Rpp32u kernelSize)
{
    reconstruction_laplacian_image_pyramid_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr1), srcSize1, static_cast<Rpp8u*>(srcPtr2), srcSize2, static_cast<Rpp8u*>(dstPtr), 
                                                       stdDev, kernelSize, 
                                                       RPPI_CHN_PACKED, 3);
    return RPP_SUCCESS;
}





int main(int argc, char** argv)
{
    RppiSize srcSize1, srcSize2, dstSize;
    unsigned int channel;
    Rpp32f stdDev = 20;
    unsigned int kernelSize = 3;

    //printf("\nEnter kernelSize: ");
    //scanf("%d", &kernelSize);

    int input;
    printf("\nEnter input: 1 = image, 2 = pixel values: ");
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

        srcSize1.height = imageIn1.rows;
        srcSize1.width = imageIn1.cols;
        srcSize2.height = imageIn2.rows;
        srcSize2.width = imageIn2.cols;

        dstSize.height = srcSize1.height;
        dstSize.width = srcSize1.width;

        //(RPPISEVEN(srcSize.height) == 1) ? (dstSize.height = srcSize.height / 2) : (dstSize.height = (srcSize.height - 1) / 2);
        //(RPPISEVEN(srcSize.width) == 1) ? (dstSize.width = srcSize.width / 2) : (dstSize.width = (srcSize.width - 1) / 2);

        printf("\nInput 1 Height - %d, Input Width - %d, Input Channels - %d\n", srcSize1.height, srcSize1.width, channel);
        Rpp8u *srcPtr1 = imageIn1.data;

        printf("\nInput 2 Height - %d, Input Width - %d, Input Channels - %d\n", srcSize2.height, srcSize2.width, channel);
        Rpp8u *srcPtr2 = imageIn2.data;
        
        printf("\nOutput Height - %d, Output Width - %d, Output Channels - %d\n", dstSize.height, dstSize.width, channel);
        Rpp8u *dstPtr = (Rpp8u *)calloc(channel * dstSize.height * dstSize.width, sizeof(Rpp8u));
        
        auto start = high_resolution_clock::now();
        auto stop = high_resolution_clock::now();

        Mat imageOut;

        if (type == 1)
        {   
            if (channel == 1)
            {
                printf("\nExecuting pln1...\n");
                start = high_resolution_clock::now();
                rppi_reconstruction_laplacian_image_pyramid_u8_pln1_host(srcPtr1, srcSize1, srcPtr2, srcSize2, dstPtr, stdDev, kernelSize);
                stop = high_resolution_clock::now();

                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC1, dstPtr);
                
            }
            else if (channel == 3)
            {
                printf("\nExecuting pln3...\n");
                Rpp8u *srcPtr1Temp = (Rpp8u *)calloc(channel * srcSize1.height * srcSize1.width, sizeof(Rpp8u));
                Rpp8u *srcPtr2Temp = (Rpp8u *)calloc(channel * srcSize2.height * srcSize2.width, sizeof(Rpp8u));
                Rpp8u *dstPtrTemp = (Rpp8u *)calloc(channel * dstSize.height * dstSize.width, sizeof(Rpp8u));
                rppi_packed_to_planar_u8_pkd3_host(srcPtr1, srcSize1, srcPtr1Temp);
                rppi_packed_to_planar_u8_pkd3_host(srcPtr2, srcSize2, srcPtr2Temp);

                start = high_resolution_clock::now();
                rppi_reconstruction_laplacian_image_pyramid_u8_pln3_host(srcPtr1Temp, srcSize1, srcPtr2Temp, srcSize2, dstPtrTemp, stdDev, kernelSize);
                stop = high_resolution_clock::now();

                rppi_planar_to_packed_u8_pln3_host(dstPtrTemp, dstSize, dstPtr);

                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC3, dstPtr);
            }
        }
        else if (type == 2)
        {   
            if (channel == 1)
            {
                printf("\nExecuting pln1 for pkd1...\n");
                start = high_resolution_clock::now();
                rppi_reconstruction_laplacian_image_pyramid_u8_pln1_host(srcPtr1, srcSize1, srcPtr2, srcSize2, dstPtr, stdDev, kernelSize);
                stop = high_resolution_clock::now();

                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC1, dstPtr);
            }
            else if (channel ==3)
            {
                printf("\nExecuting pkd3...\n");
                start = high_resolution_clock::now();
                rppi_reconstruction_laplacian_image_pyramid_u8_pkd3_host(srcPtr1, srcSize1, srcPtr2, srcSize2, dstPtr, stdDev, kernelSize);
                stop = high_resolution_clock::now();

                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC3, dstPtr);
            }
        }

        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "\nTime taken (milliseconds) = " << duration.count() << endl;

        Mat images(RPPMAX3(imageIn1.rows, imageIn2.rows, imageOut.rows), (imageIn1.cols + imageIn2.cols + imageOut.cols), imageIn1.type());
        imageIn1.copyTo(images(cv::Rect(0,0, imageIn1.cols, imageIn1.rows)));
        imageIn2.copyTo(images(cv::Rect(imageIn1.cols, 0, imageIn2.cols, imageIn2.rows)));
        imageOut.copyTo(images(cv::Rect(imageIn1.cols + imageIn2.cols,0, imageOut.cols, imageOut.rows)));

        namedWindow("2 Input Images and Output Image", WINDOW_NORMAL );
        imshow("2 Input Images and Output Image", images);

        waitKey(0);

        return 0;
    }
/*     
    int matrix;
    printf("\nEnter matrix input style: 1 = default 1 channel (1x3x4), 2 = default 3 channel (3x3x4), 3 = customized: ");
    scanf("%d", &matrix);

    if (matrix == 1)
    {
        channel = 1;
        srcSize.height = 3;
        srcSize.width = 4;
        dstSize.height = (srcSize.height + 1) / 2;
        dstSize.width = (srcSize.width + 1) / 2;
        Rpp8u srcPtr[12] = {130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108};
        Rpp8u dstPtr[4] = {0};
        printf("\n\nInput:\n");
        displayPlanar(srcPtr, srcSize, channel);
        rppi_reconstruction_laplacian_image_pyramid_u8_pln1_host(srcPtr, srcSize, dstPtr, stdDev, kernelSize);
        printf("\n\nOutput of reconstruction_laplacian_image_pyramid:\n");
        displayPlanar(dstPtr, dstSize, channel);
    }
    else if (matrix == 2)
    {
        channel = 3;
        srcSize.height = 3;
        srcSize.width = 4;
        dstSize.height = (srcSize.height + 1) / 2;
        dstSize.width = (srcSize.width + 1) / 2;
        if (type == 1)
        {
            Rpp8u srcPtr[36] = {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108, 65, 66, 67, 68, 69, 70, 71, 72, 13, 24, 15, 16};
            Rpp8u dstPtr[12] = {0};
            printf("\n\nInput:\n");
            displayPlanar(srcPtr, srcSize, channel);
            rppi_reconstruction_laplacian_image_pyramid_u8_pln3_host(srcPtr, srcSize, dstPtr, stdDev, kernelSize);
            printf("\n\nOutput of reconstruction_laplacian_image_pyramid:\n");
            displayPlanar(dstPtr, dstSize, channel);
        }
        else if (type == 2)
        {
            Rpp8u srcPtr[36] = {255, 130, 65, 254, 129, 66, 253, 128, 67, 252, 127, 68, 251, 126, 69, 250, 117, 70, 249, 113, 71, 248, 121, 72, 247, 127, 13, 246, 111, 24, 245, 100, 15, 244, 108, 16};
            Rpp8u dstPtr[12] = {0};
            printf("\n\nInput:\n");
            displayPacked(srcPtr, srcSize, channel);
            rppi_reconstruction_laplacian_image_pyramid_u8_pkd3_host(srcPtr, srcSize, dstPtr, stdDev, kernelSize);
            printf("\n\nOutput of reconstruction_laplacian_image_pyramid:\n");
            displayPacked(dstPtr, dstSize, channel);
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

        dstSize.height = (srcSize.height + 1) / 2;
        dstSize.width = (srcSize.width + 1) / 2;

        //(RPPISEVEN(dstSize.height) == 1) ? (dstSize.height = srcSize.height / 2) : (dstSize.height = (srcSize.height - 1) / 2);
        //(RPPISEVEN(dstSize.width) == 1) ? (dstSize.width = srcSize.width / 2) : (dstSize.width = (srcSize.width - 1) / 2);

        Rpp8u *dstPtr = (Rpp8u *)calloc(channel * dstSize.height * dstSize.width, sizeof(Rpp8u));
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
                rppi_reconstruction_laplacian_image_pyramid_u8_pln1_host(srcPtr, srcSize, dstPtr, stdDev, kernelSize);
            }
            else if (channel == 3)
            {
                rppi_reconstruction_laplacian_image_pyramid_u8_pln3_host(srcPtr, srcSize, dstPtr, stdDev, kernelSize);
            }
            printf("\n\nOutput of reconstruction_laplacian_image_pyramid:\n");
            displayPlanar(dstPtr, dstSize, channel);
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
                rppi_reconstruction_laplacian_image_pyramid_u8_pln1_host(srcPtr, srcSize, dstPtr, stdDev, kernelSize);
            }
            else if (channel == 3)
            {
                rppi_reconstruction_laplacian_image_pyramid_u8_pkd3_host(srcPtr, srcSize, dstPtr, stdDev, kernelSize);
            }
            printf("\n\nOutput of reconstruction_laplacian_image_pyramid:\n");
            displayPacked(dstPtr, dstSize, channel);
        }
    }
*/
    return 0;
}
