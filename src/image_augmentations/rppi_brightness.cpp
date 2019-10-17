// rppi_brightness

// Uncomment the segment below to get this standalone to work for basic unit testing

#include "rppdefs.h"
#include "rppi_image_augumentations.h"
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include "cpu/rpp_cpu_input_and_display.hpp"
#include <cpu/rpp_cpu_pixel_arrangement_conversions.hpp>
#include "cpu/host_brightness.hpp"
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
using namespace std::chrono;





//RppStatus
//rppi_brightness_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
//                             Rpp32f alpha, Rpp32s beta)
//{
//    brightness_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
//                                    alpha, beta,
//                                    RPPI_CHN_PLANAR, 1);
//
//    return RPP_SUCCESS;
//
//}
//
//RppStatus
//rppi_brightness_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
//                             Rpp32f alpha, Rpp32s beta)
//{
//    brightness_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
//                                    alpha, beta,
//                                    RPPI_CHN_PLANAR, 3);
//
//    return RPP_SUCCESS;
//
//}
//
//RppStatus
//rppi_brightness_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr,
//                             Rpp32f alpha, Rpp32s beta)
//{
//    brightness_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp8u*>(dstPtr),
//                                    alpha, beta,
//                                    RPPI_CHN_PACKED, 3);
//
//    return RPP_SUCCESS;
//
//}





int main(int argc, char** argv)
{
    RppiSize srcSize, dstSize;
    unsigned int channel;
    Rpp32f alpha = 1;
    Rpp32s beta = 35;

    int nbatchSize;
    printf("\nEnter batch size: ");
    scanf("%d", &nbatchSize);

    int imgNumber;
    printf("\nCan display only 1 image. Enter which image to display (0 to nbatchSize - 1): ");
    scanf("%d", &imgNumber);

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
        dstSize.height = srcSize.height;
        dstSize.width = srcSize.width;

        Rpp32u imageDim = srcSize.height * srcSize.width;
        Rpp32u totalImageDim = channel * srcSize.height * srcSize.width;

        printf("\nInput Height - %d, Input Width - %d, Input Channels - %d\n", srcSize.height, srcSize.width, channel);
        Rpp8u *srcPtr = imageIn.data;

        printf("\nOutput Height - %d, Output Width - %d, Output Channels - %d\n", dstSize.height, dstSize.width, channel);
        Rpp8u *dstPtr = (Rpp8u *)calloc(totalImageDim, sizeof(Rpp8u));

        RppiSize *srcSizeBatch = (RppiSize *)calloc(nbatchSize, sizeof(RppiSize));
        for (int i = 0; i < nbatchSize; i++)
        {
            srcSizeBatch[i].height = srcSize.height;
            srcSizeBatch[i].width = srcSize.width;
        }

        Rpp8u *srcPtrBatch = (Rpp8u *)calloc(nbatchSize * totalImageDim, sizeof(Rpp8u));
        Rpp8u *srcPtrBatchTemp;
        srcPtrBatchTemp = srcPtrBatch;
        for (int i = 0; i < nbatchSize; i++)
        {
            memcpy(srcPtrBatchTemp, srcPtr, totalImageDim * sizeof(Rpp8u));
            srcPtrBatchTemp += totalImageDim;
        }

        Rpp8u *dstPtrBatch = (Rpp8u *)calloc(nbatchSize * totalImageDim, sizeof(Rpp8u));
        Rpp8u *dstPtrBatchTemp;
        dstPtrBatchTemp = dstPtrBatch;
        
        Rpp32f *alphaBatch = (Rpp32f *)calloc(nbatchSize, sizeof(Rpp32f));
        Rpp32f *betaBatch = (Rpp32f *)calloc(nbatchSize, sizeof(Rpp32f));

        for (int i = 0; i < nbatchSize; i++)
        {
            alphaBatch[i] = alpha;
            betaBatch[i] = beta;
        }

        RppiROI roi;
        //roi.x = 0;
        //roi.y = 0;
        //roi.roiHeight = srcSize.height;
        //roi.roiWidth = srcSize.width;
        roi.x = 640;
        roi.y = 360;
        roi.roiHeight = 300;
        roi.roiWidth = 360;
        RppiROI *roiBatch = (RppiROI *)calloc(nbatchSize, sizeof(RppiROI));
        for (int i = 0; i < nbatchSize; i++)
        {
            roiBatch[i].x = roi.x;
            roiBatch[i].y = roi.y;
            roiBatch[i].roiHeight = roi.roiHeight;
            roiBatch[i].roiWidth = roi.roiWidth;
        }
        
        auto start = high_resolution_clock::now();
        auto stop = high_resolution_clock::now();

        Mat imageOut;

// Running once
///*
        if (type == 1)
        {   
            if (channel == 1)
            {
                printf("\nExecuting pln1...\n");
                start = high_resolution_clock::now();
                brightness_host(srcPtrBatch, srcSizeBatch, dstPtrBatch, alphaBatch, betaBatch, roiBatch, nbatchSize, RPPI_CHN_PLANAR, 1);
                stop = high_resolution_clock::now();

                for (int i = 0; i < imgNumber; i++)
                {
                    dstPtrBatchTemp = dstPtrBatchTemp + totalImageDim;
                }
                memcpy(dstPtr, dstPtrBatchTemp, totalImageDim * sizeof(Rpp8u));
                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC1, dstPtr);
                
            }
/*            else if (channel == 3)
            {
                printf("\nExecuting pln3...\n");
                Rpp8u *srcPtrTemp = (Rpp8u *)calloc(channel * srcSize.height * srcSize.width, sizeof(Rpp8u));
                Rpp8u *dstPtrTemp = (Rpp8u *)calloc(channel * dstSize.height * dstSize.width, sizeof(Rpp8u));
                rppi_packed_to_planar_u8_pkd3_host(srcPtr, srcSize, srcPtrTemp);

                start = high_resolution_clock::now();
                rppi_brightness_u8_pln3_host(srcPtrTemp, srcSize, dstPtrTemp, alpha, beta);
                stop = high_resolution_clock::now();

                rppi_planar_to_packed_u8_pln3_host(dstPtrTemp, dstSize, dstPtr);

                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC3, dstPtr);
            }*/
        }
        else if (type == 2)
        {   
            if (channel == 1)
            {
                printf("\nExecuting pln1 for pkd1...\n");
                start = high_resolution_clock::now();
                brightness_host(srcPtrBatch, srcSizeBatch, dstPtrBatch, alphaBatch, betaBatch, roiBatch, nbatchSize, RPPI_CHN_PLANAR, 1);
                stop = high_resolution_clock::now();

                for (int i = 0; i < imgNumber; i++)
                {
                    dstPtrBatchTemp = dstPtrBatchTemp + totalImageDim;
                }
                memcpy(dstPtr, dstPtrBatchTemp, totalImageDim * sizeof(Rpp8u));
                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC1, dstPtr);
            }
            else if (channel ==3)
            {
                printf("\nExecuting pkd3...\n");
                start = high_resolution_clock::now();
                brightness_host(srcPtrBatch, srcSizeBatch, dstPtrBatch, alphaBatch, betaBatch, roiBatch, nbatchSize, RPPI_CHN_PACKED, 3);
                stop = high_resolution_clock::now();

                for (int i = 0; i < imgNumber; i++)
                {
                    dstPtrBatchTemp = dstPtrBatchTemp + totalImageDim;
                }
                memcpy(dstPtr, dstPtrBatchTemp, totalImageDim * sizeof(Rpp8u));
                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC3, dstPtr);
            }
        }
//*/

// Running n times
/*
        int n = 0;
        printf("\nEnter number of times to run - ");
        scanf("%d", &n);

        if (type == 1)
        {   
            if (channel == 1)
            {
                printf("\nExecuting pln1...\n");
                start = high_resolution_clock::now();
                for (int i = 0; i < n; i++)
                {
                    rppi_brightness_u8_pln1_host(srcPtr, srcSize, dstPtr, alpha, beta);
                }
                stop = high_resolution_clock::now();

                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC1, dstPtr);
                
            }
            else if (channel == 3)
            {
                printf("\nExecuting pln3...\n");
                Rpp8u *srcPtrTemp = (Rpp8u *)calloc(channel * srcSize.height * srcSize.width, sizeof(Rpp8u));
                Rpp8u *dstPtrTemp = (Rpp8u *)calloc(channel * dstSize.height * dstSize.width, sizeof(Rpp8u));
                rppi_packed_to_planar_u8_pkd3_host(srcPtr, srcSize, srcPtrTemp);

                start = high_resolution_clock::now();
                for (int i = 0; i < n; i++)
                {
                    rppi_brightness_u8_pln3_host(srcPtrTemp, srcSize, dstPtrTemp, alpha, beta);
                }
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
                for (int i = 0; i < n; i++)
                {
                    rppi_brightness_u8_pln1_host(srcPtr, srcSize, dstPtr, alpha, beta);
                }
                stop = high_resolution_clock::now();

                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC1, dstPtr);
            }
            else if (channel ==3)
            {
                printf("\nExecuting pkd3...\n");
                start = high_resolution_clock::now();
                for (int i = 0; i < n; i++)
                {
                    rppi_brightness_u8_pkd3_host(srcPtr, srcSize, dstPtr, alpha, beta);
                }
                stop = high_resolution_clock::now();

                imageOut = Mat(dstSize.height, dstSize.width, CV_8UC3, dstPtr);
            }
        }
*/

        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "\nTime taken (milliseconds) = " << duration.count() << endl;

        Mat images(imageIn.rows, imageIn.cols*2, imageIn.type());
        imageIn.copyTo(images(cv::Rect(0,0, imageIn.cols, imageIn.rows)));
        imageOut.copyTo(images(cv::Rect(imageIn.cols,0, imageIn.cols, imageIn.rows)));

        namedWindow("Input and Output Images", WINDOW_NORMAL );
        imshow("Input and Output Images", images);

        waitKey(0);

        return 0;
    }

    printf("\nPixel Values mode not supported!");

    return 0;
    
}
