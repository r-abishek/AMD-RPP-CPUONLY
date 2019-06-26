// rppi_scale

// Uncomment the segment below to get this standalone to work for basic unit testing

#include "rppdefs.h"
#include "rppi_image_augumentation_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include "cpu/rpp_cpu_inputAndDisplay.hpp"
#include <cpu/rpp_cpu_pixelArrangementConversions.hpp>
#include "cpu/host_scale.hpp"
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
using namespace std::chrono;





RppStatus
rppi_scale_output_size_host(RppiSize srcSize, RppiSize *dstSizePtr,
                             Rpp32f percentage)
{
    scale_output_size_host(srcSize, dstSizePtr,
                            percentage);

    return RPP_SUCCESS;

}

RppStatus
rppi_scale_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize,
                         Rpp32f percentage)
{
    scale_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr), dstSize,
                            percentage,
                            RPPI_CHN_PLANAR, 1);

    return RPP_SUCCESS;

}

RppStatus
rppi_scale_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize,
                         Rpp32f percentage)
{
    scale_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr), dstSize,
                            percentage,
                            RPPI_CHN_PLANAR, 3);

    return RPP_SUCCESS;

}

RppStatus
rppi_scale_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize,
                         Rpp32f percentage)
{
    scale_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr), dstSize,
                            percentage,
                            RPPI_CHN_PACKED, 3);

    return RPP_SUCCESS;

}





int main(int argc, char** argv)
{
    RppiSize srcSize, dstSize;
    unsigned int channel;

    Rpp32f percentage;
    printf("\nEnter percentage to scale to: ");
    scanf("%f", &percentage);

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

        do
        {   printf("\nThe image input/inputs can be interpreted as 1 or 3 channel (greyscale or RGB). Please choose - only 1 or 3: ");
            scanf("%d", &channel);
        }while (channel != 1 && channel != 3);

        Mat imageIn;

        if (channel == 1)
        {
            imageIn = imread( argv[1], 0 );
        }
        else if (channel ==3)
        {
            imageIn = imread( argv[1], 1 );
        }

        if ( !imageIn.data )
        {
            printf("No image data \n");
            return -1;
        }

        srcSize.height = imageIn.rows;
        srcSize.width = imageIn.cols;
        
        printf("\nInput Height - %d, Input Width - %d, Input Channels - %d\n", srcSize.height, srcSize.width, channel);
        Rpp8u *srcPtr = imageIn.data;
        
        rppi_scale_output_size_host(srcSize, &dstSize, percentage);
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
                rppi_scale_u8_pln1_host(srcPtr, srcSize, dstPtr, dstSize, percentage);
                stop = high_resolution_clock::now();

                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC1, dstPtr);
                
            }
            else if (channel == 3)
            {
                printf("\nExecuting pln3...\n");
                Rpp8u *srcPtrTemp = (Rpp8u *)calloc(channel * srcSize.height * srcSize.width, sizeof(Rpp8u));
                Rpp8u *dstPtrTemp = (Rpp8u *)calloc(channel * dstSize.height * dstSize.width, sizeof(Rpp8u));
                rppi_packed2planar_u8_pkd3_host(srcPtr, srcSize, srcPtrTemp);

                start = high_resolution_clock::now();
                rppi_scale_u8_pln3_host(srcPtrTemp, srcSize, dstPtrTemp, dstSize, percentage);
                stop = high_resolution_clock::now();

                rppi_planar2packed_u8_pln3_host(dstPtrTemp, dstSize, dstPtr);

                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC3, dstPtr);
            }
        }
        else if (type == 2)
        {   
            if (channel == 1)
            {
                printf("\nExecuting pln1 for pkd1...\n");
                start = high_resolution_clock::now();
                rppi_scale_u8_pln1_host(srcPtr, srcSize, dstPtr, dstSize, percentage);
                stop = high_resolution_clock::now();

                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC1, dstPtr);
            }
            else if (channel ==3)
            {
                printf("\nExecuting pkd3...\n");
                start = high_resolution_clock::now();
                rppi_scale_u8_pkd3_host(srcPtr, srcSize, dstPtr, dstSize, percentage);
                stop = high_resolution_clock::now();

                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC3, dstPtr);
            }
        }

        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "\nTime taken (milliseconds) = " << duration.count() << endl;

        Mat images(RPPMAX2(imageIn.rows, imageOut.rows), (imageIn.cols + imageOut.cols), imageIn.type());
        imageIn.copyTo(images(cv::Rect(0,0, imageIn.cols, imageIn.rows)));
        imageOut.copyTo(images(cv::Rect(imageIn.cols,0, imageOut.cols, imageOut.rows)));

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
        printf("\nInput Height - %d, Input Width - %d\n", srcSize.height, srcSize.width);
        Rpp8u srcPtr[12] = {130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108};
        
        rppi_scale_output_size_host(srcSize, &dstSize, percentage);
        printf("\nOutput Height - %d, Output Width - %d\n", dstSize.height, dstSize.width);
        Rpp8u *dstPtr = (Rpp8u *)calloc(channel * dstSize.height * dstSize.width, sizeof(Rpp8u));
        
        printf("\n\nInput:\n");
        displayPlanar(srcPtr, srcSize, channel);
        rppi_scale_u8_pln1_host(srcPtr, srcSize, dstPtr, dstSize, percentage);
        printf("\n\nOutput of scale:\n");
        displayPlanar(dstPtr, dstSize, channel);
    }
    else if (matrix == 2)
    {
        channel = 3;
        srcSize.height = 3;
        srcSize.width = 4;
        printf("\nInput Height - %d, Input Width - %d\n", srcSize.height, srcSize.width);
        if (type == 1)
        {
            Rpp8u srcPtr[36] = {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108, 65, 66, 67, 68, 69, 70, 71, 72, 13, 24, 15, 16};
            
            rppi_scale_output_size_host(srcSize, &dstSize, percentage);
            printf("\nOutput Height - %d, Output Width - %d\n", dstSize.height, dstSize.width);
            Rpp8u *dstPtr = (Rpp8u *)calloc(channel * dstSize.height * dstSize.width, sizeof(Rpp8u));

            printf("\n\nInput:\n");
            displayPlanar(srcPtr, srcSize, channel);
            rppi_scale_u8_pln3_host(srcPtr, srcSize, dstPtr, dstSize, percentage);
            printf("\n\nOutput of scale:\n");
            displayPlanar(dstPtr, dstSize, channel);
        }
        else if (type == 2)
        {
            Rpp8u srcPtr[36] = {255, 130, 65, 254, 129, 66, 253, 128, 67, 252, 127, 68, 251, 126, 69, 250, 117, 70, 249, 113, 71, 248, 121, 72, 247, 127, 13, 246, 111, 24, 245, 100, 15, 244, 108, 16};
            
            rppi_scale_output_size_host(srcSize, &dstSize, percentage);
            printf("\nOutput Height - %d, Output Width - %d\n", dstSize.height, dstSize.width);
            Rpp8u *dstPtr = (Rpp8u *)calloc(channel * dstSize.height * dstSize.width, sizeof(Rpp8u));

            printf("\n\nInput:\n");
            displayPacked(srcPtr, srcSize, channel);
            printf("Height - %d, Width - %d", dstSize.height, dstSize.width);
            rppi_scale_u8_pkd3_host(srcPtr, srcSize, dstPtr, dstSize, percentage);
            printf("\n\nOutput of scale:\n");
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
        int *intSrcPtr = (int *)calloc(channel * srcSize.height * srcSize.width, sizeof(int));
        printf("\nInput Height - %d, Input Width - %d\n", srcSize.height, srcSize.width);
        
        rppi_scale_output_size_host(srcSize, &dstSize, percentage);
        printf("\nOutput Height - %d, Output Width - %d\n", dstSize.height, dstSize.width);
        Rpp8u *dstPtr = (Rpp8u *)calloc(channel * dstSize.height * dstSize.width, sizeof(Rpp8u));

        if (type == 1)
        {
            printf("\n\n\n\nEnter elements in array of size %d x %d x %d in planar format: \n", channel, srcSize.height, srcSize.width);
            inputPlanar(intSrcPtr, srcSize, channel);
            cast(intSrcPtr, srcPtr, srcSize, channel);
            printf("\n\nInput:\n");
            displayPlanar(srcPtr, srcSize, channel);
            if (channel == 1)
            {
                rppi_scale_u8_pln1_host(srcPtr, srcSize, dstPtr, dstSize, percentage);
            }
            else if (channel == 3)
            {
                rppi_scale_u8_pln3_host(srcPtr, srcSize, dstPtr, dstSize, percentage);
            }
            printf("\n\nOutput of scale:\n");
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
                rppi_scale_u8_pln1_host(srcPtr, srcSize, dstPtr, dstSize, percentage);
            }
            else if (channel == 3)
            {
                rppi_scale_u8_pkd3_host(srcPtr, srcSize, dstPtr, dstSize, percentage);
            }
            printf("\n\nOutput of scale:\n");
            displayPacked(dstPtr, dstSize, channel);
        }
    }
}