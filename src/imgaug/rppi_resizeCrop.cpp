// rppi_resizeCrop

// Uncomment the segment below to get this standalone to work for basic unit testing

#include <stdio.h>
#include <stdlib.h>
#include "rppdefs.h"
#include "cpu/host_resizeCrop.hpp"
#include "rppi_image_augumentation_functions.h"
#include <chrono> 
 
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
using namespace std::chrono;





RppStatus
rppi_resizeCrop_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize, Rpp32u x1, Rpp32u y1, Rpp32u x2, Rpp32u y2)
{
    resizeCrop_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr), dstSize,
                            x1, y1, x2, y2,
                            RPPI_CHN_PLANAR, 1);

    return RPP_SUCCESS;

}

RppStatus
rppi_resizeCrop_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize, Rpp32u x1, Rpp32u y1, Rpp32u x2, Rpp32u y2)
{
    resizeCrop_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr), dstSize,
                            x1, y1, x2, y2,
                            RPPI_CHN_PLANAR, 3);

    return RPP_SUCCESS;

}

RppStatus
rppi_resizeCrop_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize, Rpp32u x1, Rpp32u y1, Rpp32u x2, Rpp32u y2)
{
    resizeCrop_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr), dstSize,
                            x1, y1, x2, y2,
                            RPPI_CHN_PACKED, 3);

    return RPP_SUCCESS;

}




// Uncomment the segment below to get this standalone to work for basic unit testing

void inputPlanar(int *intSrcPtr, RppiSize srcSize, unsigned int channel)
{
    int p = 0;
    for(int c = 0; c < channel; c++)
    {
        printf("\n\nEnter %d elements for channel %d:\n", (srcSize.width * srcSize.height), c+1);
        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            scanf("%d", intSrcPtr + p);
            p += 1;
        }
    }
}

void inputPacked(int *intSrcPtr, RppiSize srcSize, unsigned int channel)
{
    int p = 0;
    printf("\n\nEnter %d elements for the image:\n", (channel * srcSize.height * srcSize.width));
    for (int i = 0; i < (srcSize.height * srcSize.width); i++)
    {
        for (int c = 0; c < channel; c++)
        {
            printf("Channel %d - ", c+1);
            scanf("%d", intSrcPtr + p);
            p += 1;
        }
        printf("\n");
    }
}

void cast(int *intSrcPtr, Rpp8u *srcPtr, RppiSize srcSize, unsigned int channel)
{
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        intSrcPtr[i] = (intSrcPtr[i] < 255) ? intSrcPtr[i] : 255;
        intSrcPtr[i] = (intSrcPtr[i] > 0) ? intSrcPtr[i] : 0;
        srcPtr[i] = (Rpp8u) intSrcPtr[i];

    }
}

void displayPlanar(Rpp8u *pArr, RppiSize size, unsigned int channel)
{
    int p = 0;
    for(int c = 0; c < channel; c++)
    {
        printf("\n\nChannel %d:\n", c+1);
        for (int i = 0; i < (size.height * size.width); i++)
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

void displayPacked(Rpp8u *pArr, RppiSize size, unsigned int channel)
{
    int p = 0;
    for (int i = 0; i < size.height; i++)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int j = 0; j < size.width; j++)
            {
                printf("%d\t\t", *(pArr + p + c + (j * channel)));
            }
            printf("\n");
        }
        printf("\n");
        p += (channel * size.width);
    }
}

int main(int argc, char** argv)
{
    RppiSize srcSize, dstSize;
    unsigned int channel;
    unsigned int x1, y1, x2, y2;

    int input;
    printf("\nEnter input: 1 = image, 2 = pixel values: ");
    scanf("%d", &input);

    if (input == 1)
    {
        dstSize.width = 2700;
        dstSize.height = 1400;
        x1 = 320;
        x2 = 960;
        y1 = 648;
        y2 = 72;

        //int xDiff = (int) x2 - (int) x1;
        //int yDiff = (int) y2 - (int) y1;
        //dstSize.width = (Rpp32u) RPPABS(xDiff);
        //dstSize.height = (Rpp32u) RPPABS(yDiff);





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
        printf("\nInput Height - %d, Input Width - %d\n", srcSize.height, srcSize.width);
        channel = imageIn.channels();
        Rpp8u *srcPtr = imageIn.data;
        
        int xDiff = (int) x2 - (int) x1;
        int yDiff = (int) y2 - (int) y1;
        printf("\nCropped Image Height - %d, Cropped Image Width - %d\n", (Rpp32u) RPPABS(yDiff), (Rpp32u) RPPABS(xDiff));

        printf("\nOutput Height - %d, Output Width - %d\n", dstSize.height, dstSize.width);
        Rpp8u *dstPtr = (Rpp8u *)calloc(channel * dstSize.height * dstSize.width, sizeof(Rpp8u));
        
        auto start = high_resolution_clock::now();
        rppi_resizeCrop_u8_pkd3_host(srcPtr, srcSize, dstPtr, dstSize, x1, y1, x2, y2);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "\nTime taken (milliseconds) = " << duration.count() << endl;
        
        Mat imageOut(dstSize.height, dstSize.width, CV_8UC3, dstPtr);


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

    int type;
    printf("\nEnter type of arrangement: 1 = planar, 2 = packed: ");
    scanf("%d", &type);
    
    if (matrix == 1)
    {
        channel = 1;
        srcSize.height = 3;
        srcSize.width = 4;
        dstSize.height = 6;
        dstSize.width = 8;
        x1 = 0;
        x2 = 1;
        y1 = 0;
        y2 = 1;
        printf("\nInput Height - %d, Input Width - %d\n", srcSize.height, srcSize.width);
        printf("\nOutput Height - %d, Output Width - %d\n", dstSize.height, dstSize.width);
        Rpp8u srcPtr[12] = {130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108};
        
        Rpp8u *dstPtr = (Rpp8u *)calloc(channel * dstSize.height * dstSize.width, sizeof(Rpp8u));
        
        printf("\n\nInput:\n");
        displayPlanar(srcPtr, srcSize, channel);
        rppi_resizeCrop_u8_pln1_host(srcPtr, srcSize, dstPtr, dstSize, x1, y1, x2, y2);
        printf("\n\nOutput of resizeCrop:\n");
        displayPlanar(dstPtr, dstSize, channel);
    }
    else if (matrix == 2)
    {
        channel = 3;
        srcSize.height = 3;
        srcSize.width = 4;
        dstSize.height = 9;
        dstSize.width = 10;
        x1 = 0;
        x2 = 1;
        y1 = 0;
        y2 = 1;
        printf("\nInput Height - %d, Input Width - %d\n", srcSize.height, srcSize.width);
        printf("\nOutput Height - %d, Output Width - %d\n", dstSize.height, dstSize.width);
        if (type == 1)
        {
            Rpp8u srcPtr[36] = {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108, 65, 66, 67, 68, 69, 70, 71, 72, 13, 24, 15, 16};
            
            Rpp8u *dstPtr = (Rpp8u *)calloc(channel * dstSize.height * dstSize.width, sizeof(Rpp8u));

            printf("\n\nInput:\n");
            displayPlanar(srcPtr, srcSize, channel);
            rppi_resizeCrop_u8_pln3_host(srcPtr, srcSize, dstPtr, dstSize, x1, y1, x2, y2);
            printf("\n\nOutput of resizeCrop:\n");
            displayPlanar(dstPtr, dstSize, channel);
        }
        else if (type == 2)
        {
            Rpp8u srcPtr[36] = {255, 130, 65, 254, 129, 66, 253, 128, 67, 252, 127, 68, 251, 126, 69, 250, 117, 70, 249, 113, 71, 248, 121, 72, 247, 127, 13, 246, 111, 24, 245, 100, 15, 244, 108, 16};
            
            Rpp8u *dstPtr = (Rpp8u *)calloc(channel * dstSize.height * dstSize.width, sizeof(Rpp8u));

            printf("\n\nInput:\n");
            displayPacked(srcPtr, srcSize, channel);
            rppi_resizeCrop_u8_pkd3_host(srcPtr, srcSize, dstPtr, dstSize, x1, y1, x2, y2);
            printf("\n\nOutput of resizeCrop:\n");
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
        printf("Enter height of output in pixels: ");
        scanf("%d", &dstSize.height);
        printf("Enter width of output in pixels: ");
        scanf("%d", &dstSize.width);

        x1 = 0;
        x2 = 1;
        y1 = 0;
        y2 = 1;

        printf("\nInput Height - %d, Input Width - %d\n", srcSize.height, srcSize.width);
        printf("\nOutput Height - %d, Output Width - %d\n", dstSize.height, dstSize.width);

        printf("Channels = %d, Height = %d, Width = %d", channel, srcSize.height, srcSize.width);
        Rpp8u *srcPtr = (Rpp8u *)malloc(channel * srcSize.height * srcSize.width * sizeof(Rpp8u));
        int *intSrcPtr = (int *)malloc(channel * srcSize.height * srcSize.width * sizeof(int));
        
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
                rppi_resizeCrop_u8_pln1_host(srcPtr, srcSize, dstPtr, dstSize, x1, y1, x2, y2);
            }
            else if (channel == 3)
            {
                rppi_resizeCrop_u8_pln3_host(srcPtr, srcSize, dstPtr, dstSize, x1, y1, x2, y2);
            }
            printf("\n\nOutput of resizeCrop:\n");
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
                rppi_resizeCrop_u8_pln1_host(srcPtr, srcSize, dstPtr, dstSize, x1, y1, x2, y2);
            }
            else if (channel == 3)
            {
                rppi_resizeCrop_u8_pkd3_host(srcPtr, srcSize, dstPtr, dstSize, x1, y1, x2, y2);
            }
            printf("\n\nOutput of resizeCrop:\n");
            displayPacked(dstPtr, dstSize, channel);
        }
    }
}