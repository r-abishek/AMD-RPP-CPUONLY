// rppi_match_template

// Uncomment the segment below to get this standalone to work for basic unit testing

#include "rppdefs.h"
#include "rppi_filter_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include "cpu/rpp_cpu_input_and_display.hpp"
#include <cpu/rpp_cpu_pixel_arrangement_conversions.hpp>
#include "cpu/host_match_template.hpp"
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
using namespace std::chrono;




RppStatus
rppi_match_template_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize, 
                                 RppPtr_t templateImage, RppiSize templateImageSize)
{
    match_template_host<Rpp8u, Rpp16u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp16u*>(dstPtr), dstSize, 
                                   static_cast<Rpp8u*>(templateImage), templateImageSize, 
                                   RPPI_CHN_PLANAR, 1);
    return RPP_SUCCESS;
}

RppStatus
rppi_match_template_u8_pln3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize, 
                                 RppPtr_t templateImage, RppiSize templateImageSize)
{
    match_template_host<Rpp8u, Rpp16u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp16u*>(dstPtr), dstSize, 
                                   static_cast<Rpp8u*>(templateImage), templateImageSize, 
                                   RPPI_CHN_PLANAR, 3);
    return RPP_SUCCESS;
}

RppStatus
rppi_match_template_u8_pkd3_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t dstPtr, RppiSize dstSize, 
                                 RppPtr_t templateImage, RppiSize templateImageSize)
{
    match_template_host<Rpp8u, Rpp16u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp16u*>(dstPtr), dstSize, 
                                   static_cast<Rpp8u*>(templateImage), templateImageSize, 
                                   RPPI_CHN_PACKED, 3);
    return RPP_SUCCESS;
}





int main(int argc, char** argv)
{
    RppiSize srcSize, dstSize, templateImageSize;
    unsigned int channel;

    int outputWidth;
    printf("\nEnter dstSize.width: ");
    scanf("%d", &outputWidth);

    int outputHeight;
    printf("\nEnter dstSize.height: ");
    scanf("%d", &outputHeight);
    
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
            printf("usage: DisplayImage.out <Image_Path> <templateImage_Path>\n");
            return -1;
        }

        do
        {   printf("\nThe image input/inputs can be interpreted as 1 or 3 channel (greyscale or RGB). Please choose - only 1 or 3: ");
            scanf("%d", &channel);
        }while (channel != 1 && channel != 3);

        Mat imageIn, templateImageIn;

        if (channel == 1)
        {
            imageIn = imread( argv[1], 0 );
            templateImageIn = imread( argv[2], 0 );
        }
        else if (channel ==3)
        {
            imageIn = imread( argv[1], 1 );
            templateImageIn = imread( argv[2], 1 );
        }

        if ( !imageIn.data )
        {
            printf("No image data \n");
            return -1;
        }

        srcSize.height = imageIn.rows;
        srcSize.width = imageIn.cols;
        dstSize.height = outputHeight;
        dstSize.width = outputWidth;
        templateImageSize.height = templateImageIn.rows;
        templateImageSize.width = templateImageIn.cols;

        printf("\nInput Height - %d, Input Width - %d, Input Channels - %d\n", srcSize.height, srcSize.width, channel);
        Rpp8u *srcPtr = imageIn.data;

        printf("\nTemplate Height - %d, Template Width - %d, Template Channels - %d\n", templateImageSize.height, templateImageSize.width, channel);
        Rpp8u *templateImage = templateImageIn.data;
        
        printf("\nOutput Height - %d, Output Width - %d, Output Channels - %d\n", dstSize.height, dstSize.width, channel);
        Rpp16u *dstPtr = (Rpp16u *)calloc(dstSize.height * dstSize.width, sizeof(Rpp16u));
        
        auto start = high_resolution_clock::now();
        auto stop = high_resolution_clock::now();

        Mat imageOut;

        if (type == 1)
        {   
            if (channel == 1)
            {
                printf("\nExecuting pln1...\n");
                start = high_resolution_clock::now();
                rppi_match_template_u8_pln1_host(srcPtr, srcSize, dstPtr, dstSize, templateImage, templateImageSize);
                stop = high_resolution_clock::now();

                imageOut = Mat(dstSize.height, dstSize.width, CV_16UC1, dstPtr);
                
            }
            else if (channel == 3)
            {
                printf("\nExecuting pln3...\n");
                Rpp8u *srcPtrTemp = (Rpp8u *)calloc(channel * srcSize.height * srcSize.width, sizeof(Rpp8u));
                Rpp8u *dstPtrTemp = (Rpp8u *)calloc(channel * dstSize.height * dstSize.width, sizeof(Rpp8u));
                rppi_packed_to_planar_u8_pkd3_host(srcPtr, srcSize, srcPtrTemp);

                start = high_resolution_clock::now();
                rppi_match_template_u8_pln3_host(srcPtrTemp, srcSize, dstPtrTemp, dstSize, templateImage, templateImageSize);
                stop = high_resolution_clock::now();

                rppi_planar_to_packed_u8_pln3_host(dstPtrTemp, dstSize, dstPtr);

                imageOut = Mat(dstSize.height, dstSize.width, CV_16UC1, dstPtr);
            }
        }
        else if (type == 2)
        {   
            if (channel == 1)
            {
                printf("\nExecuting pln1 for pkd1...\n");
                start = high_resolution_clock::now();
                rppi_match_template_u8_pln1_host(srcPtr, srcSize, dstPtr, dstSize, templateImage, templateImageSize);
                stop = high_resolution_clock::now();

                imageOut = Mat(dstSize.height, dstSize.width, CV_16UC1, dstPtr);
            }
            else if (channel ==3)
            {
                printf("\nExecuting pkd3...\n");
                start = high_resolution_clock::now();
                rppi_match_template_u8_pkd3_host(srcPtr, srcSize, dstPtr, dstSize, templateImage, templateImageSize);
                stop = high_resolution_clock::now();

                imageOut = Mat(dstSize.height, dstSize.width, CV_16UC1, dstPtr);
            }
        }

        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "\nTime taken (milliseconds) = " << duration.count() << endl;

        //Mat images(imageIn.rows, imageIn.cols*2, imageIn.type());
        //imageIn.copyTo(images(cv::Rect(0,0, imageIn.cols, imageIn.rows)));
        //imageOut.copyTo(images(cv::Rect(imageIn.cols,0, imageIn.cols, imageIn.rows)));

        namedWindow("Input Image", WINDOW_NORMAL );
        imshow("Input Image", imageIn);

        namedWindow("Output Image", WINDOW_NORMAL );
        imshow("Output Image", imageOut);

        waitKey(0);

        Rpp16u min = *dstPtr;
        Rpp16u max = *dstPtr;
        Rpp16u *dstPtrTemp;
        dstPtrTemp = dstPtr;
        for (int i = 0; i < (dstSize.height * dstSize.width); i++)
        {
            if (*dstPtrTemp < min)
            {
                min = *dstPtrTemp;
            }
            if (*dstPtrTemp > max && *dstPtrTemp != 65535)
            {
                max = *dstPtrTemp;
            }
            dstPtrTemp++;
        }
        printf("\nMinimum metric value in the match_template output = %d", min);
        printf("\nMaximum metric value in the match_template output = %d\n", max);

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
        dstSize.height = outputHeight;
        dstSize.width = outputWidth;
        templateImageSize.height = 1;
        templateImageSize.width = 3;
        Rpp8u srcPtr[12] = {130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108};
        Rpp16u dstPtr[12] = {0};
        Rpp8u templateImage[3] = {117, 113, 121};
        printf("\n\nInput:\n");
        displayPlanar(srcPtr, srcSize, channel);
        rppi_match_template_u8_pln1_host(srcPtr, srcSize, dstPtr, dstSize, templateImage, templateImageSize);
        printf("\n\nOutput of match_template:\n");
        displayPlanar(dstPtr, dstSize, 1);

        Rpp16u min = *dstPtr;
        Rpp16u *dstPtrTemp;
        dstPtrTemp = dstPtr;
        for (int i = 0; i < (dstSize.height * dstSize.width); i++)
        {
            if (*dstPtrTemp < min)
            {
                min = *dstPtrTemp;
            }
            dstPtrTemp++;
        }
        printf("\n\nMinimum metric value in the match_template output = %d\n", min);
    }
    else if (matrix == 2)
    {
        channel = 3;
        srcSize.height = 3;
        srcSize.width = 4;
        dstSize.height = outputHeight;
        dstSize.width = outputWidth;
        templateImageSize.height = 1;
        templateImageSize.width = 3;
        Rpp8u templateImage[3] = {117, 113, 121};
        if (type == 1)
        {
            Rpp8u srcPtr[36] = {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108, 65, 66, 67, 68, 69, 70, 71, 72, 13, 24, 15, 16};
            Rpp16u dstPtr[12] = {0};
            printf("\n\nInput:\n");
            displayPlanar(srcPtr, srcSize, channel);
            rppi_match_template_u8_pln3_host(srcPtr, srcSize, dstPtr, dstSize, templateImage, templateImageSize);
            printf("\n\nOutput of match_template:\n");
            displayPlanar(dstPtr, dstSize, 1);

            Rpp16u min = *dstPtr;
            Rpp16u *dstPtrTemp;
            dstPtrTemp = dstPtr;
            for (int i = 0; i < (dstSize.height * dstSize.width); i++)
            {
                if (*dstPtrTemp < min)
                {
                    min = *dstPtrTemp;
                }
                dstPtrTemp++;
            }
            printf("\n\nMinimum metric value in the match_template output = %d\n", min);
        }
        else if (type == 2)
        {
            Rpp8u srcPtr[36] = {255, 130, 65, 254, 129, 66, 253, 128, 67, 252, 127, 68, 251, 126, 69, 250, 117, 70, 249, 113, 71, 248, 121, 72, 247, 127, 13, 246, 111, 24, 245, 100, 15, 244, 108, 16};
            Rpp16u dstPtr[12] = {0};
            printf("\n\nInput:\n");
            displayPacked(srcPtr, srcSize, channel);
            rppi_match_template_u8_pkd3_host(srcPtr, srcSize, dstPtr, dstSize, templateImage, templateImageSize);
            printf("\n\nOutput of match_template:\n");
            displayPacked(dstPtr,dstSize, 1);

            Rpp16u min = *dstPtr;
            Rpp16u *dstPtrTemp;
            dstPtrTemp = dstPtr;
            for (int i = 0; i < (dstSize.height * dstSize.width); i++)
            {
                if (*dstPtrTemp < min)
                {
                    min = *dstPtrTemp;
                }
                dstPtrTemp++;
            }
            printf("\n\nMinimum metric value in the match_template output = %d\n", min);
        } 
    }
    else if (matrix == 3)
    {
        printf("\nOption not available!");
    }
}
