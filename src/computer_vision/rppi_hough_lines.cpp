// rppi_hough_lines

// Uncomment the segment below to get this standalone to work for basic unit testing

#include "rppdefs.h"
#include "rppi_computer_vision.h"
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include "cpu/rpp_cpu_input_and_display.hpp"
#include <cpu/rpp_cpu_pixel_arrangement_conversions.hpp>
#include "cpu/host_hough_lines_trial.hpp"
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
using namespace std::chrono;




RppStatus
rppi_hough_lines_u8_pln1_host(RppPtr_t srcPtr, RppiSize srcSize, RppPtr_t lines, 
                              Rpp32f rho, Rpp32f theta, Rpp32u threshold, 
                              Rpp32u minLineLength, Rpp32u maxLineGap, Rpp32u linesMax)
{
    hough_lines_host<Rpp8u, Rpp32u>(static_cast<Rpp8u*>(srcPtr), srcSize, static_cast<Rpp32u*>(lines), rho, theta, threshold, minLineLength, maxLineGap, linesMax);

    return RPP_SUCCESS;
}





int main(int argc, char** argv)
{
    RppiSize srcSize;
    RppiSize dstSize;
    unsigned int channel = 1;
    Rpp32f minLineLength = 100, maxLineGap = 3;
    
    Rpp32u linesMax = 100;
    printf("\nEnter maximum number of lines you want to detect: ");
    scanf("%d", &linesMax);
    
    float theta = PI/180;
    printf("\nEnter theta in degrees (increment for angle in r-theta notation): ");
    scanf("%f", &theta);
    theta = theta * (PI / 180);

    float rho = 1;
    printf("\nEnter rho (increment for r in r-theta notation): ");
    scanf("%f", &rho);

    Rpp32u threshold;
    printf("\nEnter threshold for accumulator: ");
    scanf("%d", &threshold);

    Rpp32u *lines = (Rpp32u *)calloc(4 * linesMax, sizeof(Rpp32u));
    
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
        imageIn = imread( argv[1], 0 );

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
        
        printf("\nOutput Height - %d, Output Width - %d, Output Channels - %d\n", dstSize.height, dstSize.width, channel);
        Rpp8u *dstPtr = (Rpp8u *)calloc(channel * srcSize.height * srcSize.width, sizeof(Rpp8u));

        //memcpy(dstPtr, srcPtr, srcSize.height * srcSize.width * sizeof(Rpp8u));
        
        auto start = high_resolution_clock::now();
        auto stop = high_resolution_clock::now();

        printf("\nExecuting pln1...\n");
        start = high_resolution_clock::now();
        rppi_hough_lines_u8_pln1_host(srcPtr, srcSize, lines, rho, theta, threshold, (Rpp32u) minLineLength, (Rpp32u) maxLineGap, linesMax);
        stop = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "\nTime taken (milliseconds) = " << duration.count() << endl;

        Rpp32u *endpoints = (Rpp32u*)calloc(4, sizeof(Rpp32u));
        Rpp32u *rasterCoordinates= (Rpp32u *)calloc(2 * (dstSize.height + dstSize.width), sizeof(Rpp32u));
        Rpp32u *linesTemp;
        linesTemp = lines;
        for (int i = 0; i < linesMax; i++)
        {
            *endpoints = *linesTemp;
            *(endpoints + 1) = *(linesTemp+1);
            *(endpoints + 2) = *(linesTemp+2);
            *(endpoints + 3) = *(linesTemp+3);

            //printf("\n line %d endpoints - (%d,%d) to (%d,%d)", i+1, *endpoints, *(endpoints+1), *(endpoints+2), *(endpoints+3));

            generate_bressenham_line_host(dstPtr, dstSize, endpoints, rasterCoordinates);

            linesTemp += 4;
        }
        //printf("\n");

        Mat imageOut;
        imageOut = Mat(dstSize.height, dstSize.width, CV_8UC1, dstPtr);

        Mat images(imageIn.rows, imageIn.cols*2, imageIn.type());
        imageIn.copyTo(images(cv::Rect(0,0, imageIn.cols, imageIn.rows)));
        imageOut.copyTo(images(cv::Rect(imageIn.cols,0, imageIn.cols, imageIn.rows)));

        namedWindow("Input and Output Images", WINDOW_NORMAL );
        imshow("Input and Output Images", images);

        waitKey(0);

        return 0;
    }

    printf("\nThis option isn't available for hough lines!");

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
        rppi_hough_lines_u8_pln1_host(srcPtr, srcSize, dstPtr, lines, rho, theta, threshold, lineLength, lineGap, thetaMax, thetaMin, accumulator, accumulatorSize);
        printf("\n\nOutput of hough_lines:\n");
        displayPlanar(dstPtr, srcSize, channel);
    }
    else if (matrix == 2)
    {
        printf("\nThis option isn't available for hough lines!");
    }
    else if (matrix == 3)
    {
        channel = 1;
        printf("Enter height of image in pixels: ");
        scanf("%d", &srcSize.height);
        printf("Enter width of image in pixels: ");
        scanf("%d", &srcSize.width);
        printf("Channels = %d, Height = %d, Width = %d", channel, srcSize.height, srcSize.width);
        Rpp8u *srcPtr = (Rpp8u *)calloc(channel * srcSize.height * srcSize.width, sizeof(Rpp8u));
        Rpp8u *dstPtr = (Rpp8u *)calloc(channel * srcSize.height * srcSize.width, sizeof(Rpp8u));
        int *intSrcPtr = (int *)calloc(channel * srcSize.height * srcSize.width, sizeof(int));
        
        printf("\n\n\n\nEnter elements in array of size %d x %d x %d in planar format: \n", channel, srcSize.height, srcSize.width);
        inputPlanar(intSrcPtr, srcSize, channel);
        cast(intSrcPtr, srcPtr, srcSize, channel);
        printf("\n\nInput:\n");
        displayPlanar(srcPtr, srcSize, channel);

        rppi_hough_lines_u8_pln1_host(srcPtr, srcSize, dstPtr, lines, rho, theta, threshold, lineLength, lineGap, thetaMax, thetaMin, accumulator, accumulatorSize);
        
        printf("\n\nOutput of hough_lines:\n");
        displayPlanar(dstPtr, srcSize, channel);
    }
*/
    return 0;
}
