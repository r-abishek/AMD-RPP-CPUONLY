// rppi_tensor_look_up_table

// Uncomment the segment below to get this standalone to work for basic unit testing

#include "rppdefs.h"
#include "rppi_arithmetic_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include "cpu/rpp_cpu_input_and_display.hpp"
#include <cpu/rpp_cpu_pixel_arrangement_conversions.hpp>
#include "cpu/host_tensor_look_up_table.hpp"
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
using namespace std::chrono;





RppStatus
rppi_tensor_look_up_table_u8_host(RppPtr_t srcPtr, RppPtr_t dstPtr, RppPtr_t lutPtr, 
                        Rpp32u tensorDimension, RppPtr_t tensorDimensionValues)
{
    tensor_look_up_table_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr), static_cast<Rpp8u*>(dstPtr), static_cast<Rpp8u*>(lutPtr), 
                           tensorDimension, static_cast<Rpp32u*>(tensorDimensionValues));

    return RPP_SUCCESS;

}





int main(int argc, char** argv)
{
    Rpp8u *lutPtr = (Rpp8u *)calloc(256, sizeof(Rpp8u));
    Rpp8u *lutPtrTemp;
    lutPtrTemp = lutPtr;
    
    for (int j = 0; j < 256; j++)
    {
        *lutPtrTemp = (Rpp8u) (255 - j);
        lutPtrTemp++;
    }

    unsigned int channel;
    unsigned int tensorDimension = 4;
    unsigned int tensorDimensionValues[4] = {3,2,2,3};
    //unsigned int *tensorDimensionValues = (unsigned int *)calloc(tensorDimension, sizeof(unsigned int));

    Rpp8u srcPtr[36] = {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108, 65, 66, 67, 68, 69, 70, 71, 72, 13, 24, 15, 16};
    //Rpp8u srcPtr[36] = {16, 15, 24, 13, 72, 71, 70, 69, 68, 67, 66, 65, 108, 100, 111, 127, 121, 113, 117, 126, 127, 128, 129, 130, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};
    Rpp8u dstPtr[36] = {0};
    
    printf("\n\nInput:\n");
    displayTensor(srcPtr, 36);
    rppi_tensor_look_up_table_u8_host(srcPtr, dstPtr, lutPtr, tensorDimension, tensorDimensionValues);
    printf("\n\nOutput of tensor_look_up_table:\n");
    displayTensor(dstPtr, 36);
/*
    auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();

    start = high_resolution_clock::now();
    rppi_add_u8_pln3_host(srcPtr1Temp, srcPtr2Temp, srcSize, dstPtrTemp);
    stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "\nTime taken (milliseconds) = " << duration.count() << endl;
*/    
}
