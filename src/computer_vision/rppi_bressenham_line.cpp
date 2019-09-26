// rppi_bressenham_line

// Uncomment the segment below to get this standalone to work for basic unit testing

#include "rppdefs.h"
#include "rppi_computer_vision.h"
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include "cpu/rpp_cpu_input_and_display.hpp"
#include <cpu/rpp_cpu_pixel_arrangement_conversions.hpp>
#include "cpu/host_bressenham_line.hpp"
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
using namespace std::chrono;




RppStatus
rppi_bressenham_line_u8_pln1_host(RppPtr_t dstPtr, RppiSize dstSize, Rpp32u *endpoints, Rpp32u *rasterCoordinates)
{
    bressenham_line_host<Rpp8u>(static_cast<Rpp8u*>(dstPtr), dstSize, 
                                endpoints, rasterCoordinates, 
                                RPPI_CHN_PLANAR, 1);
    return RPP_SUCCESS;
}

//RppStatus
//rppi_bressenham_line_u8_pln3_host(RppPtr_t dstPtr, RppiSize dstSize, Rpp32u x1, Rpp32u y1, Rpp32u x2, Rpp32u y2)
//{
//    bressenham_line_host<Rpp8u>(static_cast<Rpp8u*>(dstPtr), dstSize, 
//                                x1, y1, x2, y2, 
//                                RPPI_CHN_PLANAR, 3);
//    return RPP_SUCCESS;
//}
//
//RppStatus
//rppi_bressenham_line_u8_pkd3_host(RppPtr_t dstPtr, RppiSize dstSize, Rpp32u x1, Rpp32u y1, Rpp32u x2, Rpp32u y2)
//{
//    bressenham_line_host<Rpp8u>(static_cast<Rpp8u*>(dstPtr), dstSize, 
//                                x1, y1, x2, y2, 
//                                RPPI_CHN_PACKED, 3);
//    return RPP_SUCCESS;
//}





int main(int argc, char** argv)
{
    RppiSize dstSize;
    unsigned int channel = 1;
    
    unsigned int x1;
    printf("\nEnter x1: ");
    scanf("%d", &x1);

    unsigned int y1;
    printf("\nEnter y1: ");
    scanf("%d", &y1);

    unsigned int x2;
    printf("\nEnter x2: ");
    scanf("%d", &x2);

    unsigned int y2;
    printf("\nEnter y2: ");
    scanf("%d", &y2);

    Rpp32u *endpoints = (Rpp32u*)calloc(4, sizeof(Rpp32u));
    *endpoints = x1;
    *(endpoints + 1) = y1;
    *(endpoints + 2) = x2;
    *(endpoints + 3) = y2;

    int height;
    printf("\nEnter height of destination image: ");
    scanf("%d", &height);

    int width;
    printf("\nEnter width of destination image: ");
    scanf("%d", &width);

    int displayType;
    printf("\nEnter display type (as image / as pixel values - 1 / 2): ");
    scanf("%d", &displayType);

    dstSize.height = height;
    dstSize.width = width;
    
    Rpp8u *dstPtr = (Rpp8u *)calloc(channel * dstSize.height * dstSize.width, sizeof(Rpp8u));
    memset(dstPtr, 0, dstSize.height * dstSize.width * sizeof(Rpp8u));
    Rpp32u *rasterCoordinates= (Rpp32u *)calloc(2 * (dstSize.height + dstSize.width), sizeof(Rpp32u));

    int flag = 0;
    Rpp32u *rasterCoordinatesTemp;
    rasterCoordinatesTemp = rasterCoordinates;
    
    if (displayType == 1)
    {
        Mat imageIn;
        imageIn = Mat(dstSize.height, dstSize.width, CV_8UC1, dstPtr);

        namedWindow("Input Image", WINDOW_NORMAL );
        imshow("Input Image", imageIn);

        auto start = high_resolution_clock::now();
        rppi_bressenham_line_u8_pln1_host(dstPtr, dstSize, endpoints, rasterCoordinates);
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "\nTime taken (milliseconds) = " << duration.count() << endl;

        printf("\n\nRaster Coordinates:\n");
        for (int i = 0; i < 2 * (dstSize.height + dstSize.width); i+=2)
        {
            if(*rasterCoordinatesTemp == 0 && *(rasterCoordinatesTemp + 1) == 0)
            {
                flag++;
                if (flag == 2)
                {
                    break;
                }
            }
            printf("%d,%d\n", *rasterCoordinatesTemp, *(rasterCoordinatesTemp + 1));
            rasterCoordinatesTemp+=2;
        }

        Mat imageOut;
        imageOut = Mat(dstSize.height, dstSize.width, CV_8UC1, dstPtr);

        namedWindow("Output Image", WINDOW_NORMAL );
        imshow("Output Image", imageOut);

        waitKey(0);
        return 0;
    }
    else if (displayType == 2)
    {
        displayPlanar(dstPtr, dstSize, 1);

        auto start = high_resolution_clock::now();
        rppi_bressenham_line_u8_pln1_host(dstPtr, dstSize, endpoints, rasterCoordinates);
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "\nTime taken (milliseconds) = " << duration.count() << endl;

        printf("\n\nRaster Coordinates:\n");
        for (int i = 0; i < 2 * (dstSize.height + dstSize.width); i+=2)
        {
            if(*rasterCoordinatesTemp == 0 && *(rasterCoordinatesTemp + 1) == 0)
            {
                flag++;
                if (flag == 2)
                {
                    break;
                }
            }
            printf("%d,%d\n", *rasterCoordinatesTemp, *(rasterCoordinatesTemp + 1));
            rasterCoordinatesTemp+=2;
        }

        displayPlanar(dstPtr, dstSize, 1);

        return 0;
    }

}