// rppi_tensor_matrix_multiply

// Uncomment the segment below to get this standalone to work for basic unit testing

#include "rppdefs.h"
#include "rppi_arithmetic_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include "cpu/rpp_cpu_input_and_display.hpp"
#include <cpu/rpp_cpu_pixel_arrangement_conversions.hpp>
#include "cpu/host_tensor_matrix_multiply.hpp"
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
using namespace std::chrono;





RppStatus
rppi_tensor_matrix_multiply_u8_host(RppPtr_t srcPtr1, RppPtr_t srcPtr2, RppPtr_t dstPtr, 
                                    RppPtr_t tensorDimensionValues1, RppPtr_t tensorDimensionValues2)
{
    tensor_matrix_multiply_host<Rpp8u>(static_cast<Rpp8u*>(srcPtr1), static_cast<Rpp8u*>(srcPtr2), static_cast<Rpp8u*>(dstPtr), 
                                       static_cast<Rpp32u*>(tensorDimensionValues1), static_cast<Rpp32u*>(tensorDimensionValues2));

    return RPP_SUCCESS;

}





int main(int argc, char** argv)
{
    unsigned int channel;
    unsigned int tensorDimensionValues1[2] = {3, 2};
    unsigned int tensorDimensionValues2[2] = {2, 4};
    //unsigned int *tensorDimensionValues = (unsigned int *)calloc(tensorDimension, sizeof(unsigned int));

    //Rpp8u srcPtr1[36] = {255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 130, 129, 128, 127, 126, 117, 113, 121, 127, 111, 100, 108, 65, 66, 67, 68, 69, 70, 71, 72, 13, 24, 15, 16};
    //Rpp8u srcPtr2[36] = {16, 15, 24, 13, 72, 71, 70, 69, 68, 67, 66, 65, 108, 100, 111, 127, 121, 113, 117, 126, 127, 128, 129, 130, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};
    Rpp8u srcPtr1[6] = {1, 2, 3, 4, 5, 6};
    Rpp8u srcPtr2[8] = {7, 8, 9, 10, 11, 12, 13, 14};
    Rpp8u dstPtr[12] = {0};

    printf("\n\nInput 1:\n");
    displayTensor(srcPtr1, 6);
    printf("\n\nInput 2:\n");
    displayTensor(srcPtr2, 8);
    printf("\n\nOutput Initialization:\n");
    displayTensor(dstPtr, 12);
    rppi_tensor_matrix_multiply_u8_host(srcPtr1, srcPtr2, dstPtr, tensorDimensionValues1, tensorDimensionValues2);
    printf("\n\nOutput of tensor_matrix_multiply:\n");
    displayTensor(dstPtr, 12);

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
