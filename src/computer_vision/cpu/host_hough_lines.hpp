#include <cpu/rpp_cpu_common.hpp>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <vector>
using namespace std;

template <typename T, typename U>
RppStatus hough_lines_host(T* srcPtr, RppiSize srcSize, U* lines, 
                           Rpp32f rho, Rpp32f theta, Rpp32u threshold, 
                           Rpp32u lineLength, Rpp32u lineGap, Rpp32u linesMax)
{
    // Initializations

    srand (time(NULL));
    
    U *linesTemp;
    linesTemp = lines;
    Rpp32u numofLines = 0;

    Rpp8u *srcPtrTemp;
    srcPtrTemp = srcPtr;

    RppiSize accumulatorSize;
    accumulatorSize.width = round(((srcSize.width + srcSize.height) * 2 + 1) / rho);
    accumulatorSize.height = round(PI / theta);

    Rpp32s *accumulator = (Rpp32s*)calloc(accumulatorSize.width * accumulatorSize.height, sizeof(Rpp32s));
    Rpp32s *accumulatorTemp, *accumulatorTemp2;

    T *validPixelMask = (T*)calloc(srcSize.height * srcSize.width, sizeof(T));
    T *validPixelMaskTemp;
    validPixelMaskTemp = validPixelMask;
    
    Rpp32f *cosLookUpTable = (Rpp32f*)calloc(accumulatorSize.height, sizeof(Rpp32f));
    Rpp32f *sinLookUpTable = (Rpp32f*)calloc(accumulatorSize.height, sizeof(Rpp32f));
    Rpp32f *cosLookUpTableTemp, *sinLookUpTableTemp;
    cosLookUpTableTemp = cosLookUpTable;
    sinLookUpTableTemp = sinLookUpTable;

    for(int n = 0; n < accumulatorSize.height; n++ )
    {
        *cosLookUpTableTemp = (Rpp32f) (cos((Rpp64f) (n * theta)) / rho);
        *sinLookUpTableTemp = (Rpp32f) (sin((Rpp64f) (n * theta)) / rho);
        cosLookUpTableTemp++;
        sinLookUpTableTemp++;
    }
    std::vector<Rpp32u> validPixelLocations;

    // Valid pixel locations
    for( int i = 0; i < srcSize.height; i++ )
    {
        for( int j = 0; j < srcSize.width; j++ )
        {
            if(*srcPtrTemp)
            {
                *validPixelMaskTemp = (T) 1;
                validPixelLocations.push_back(j);
                validPixelLocations.push_back(i);
            }
            else
            {
                *validPixelMaskTemp = (T) 0;
            }
            srcPtrTemp++;
            validPixelMaskTemp++;
        }
    }

    int count = (int)validPixelLocations.size() / 2;
    Rpp32s *endpoints = (Rpp32s*)calloc(4, sizeof(Rpp32s));

    // Pick random pixels and process them
    for( ; count > 0; count-- )
    {
        // Choose a random point
        Rpp32u randomPixelLoc = rand() % (count + 1);
        int max_val = threshold-1, max_n = 0;
        Rpp32u pixelLocX, pixelLocY;
        pixelLocX = validPixelLocations[2 * randomPixelLoc];
        pixelLocY = validPixelLocations[(2 * randomPixelLoc) + 1];

        Rpp32f a, b;
        accumulatorTemp = accumulator;
        Rpp32s i = pixelLocY, j = pixelLocX, k, x0, y0, dx0, dy0, xflag;
        Rpp32s good_line;
        Rpp32s shift = 16;

        // "remove" it by overriding it with the last element
        validPixelLocations[2 * randomPixelLoc] = validPixelLocations[2 * (count-1)];
        validPixelLocations[(2 * randomPixelLoc) + 1] = validPixelLocations[(2 * (count-1)) + 1];

        // Check if pixel is part of some other line
        if(!*(validPixelMask + (i * srcSize.width) + j))
            continue;

        // Update the accumulator
        cosLookUpTableTemp = cosLookUpTable;
        sinLookUpTableTemp = sinLookUpTable;
        for( int n = 0; n < accumulatorSize.height; n++, accumulatorTemp += accumulatorSize.width )
        {
            Rpp32s r = round( j * *cosLookUpTableTemp + i * *sinLookUpTableTemp );
            r += (accumulatorSize.width - 1) / 2;
            *(accumulatorTemp + r) += 1;
            int val = *(accumulatorTemp + r);
            if( max_val < val )
            {
                max_val = val;
                max_n = n;
            }
            cosLookUpTableTemp++;
            sinLookUpTableTemp++;
        }

        // Compare against threshold given by the user
        if( max_val < threshold )
            continue;

        // From the selected point, walk in each direction along the found line and extract line segment
        a = -*(sinLookUpTable + max_n);
        b = *(cosLookUpTable + max_n);
        x0 = j;
        y0 = i;
        if( RPPABS(a) > RPPABS(b) )
        {
            xflag = 1;
            dx0 = a > 0 ? 1 : -1;
            dy0 = round( b*(1 << shift)/RPPABS(a) );
            y0 = (y0 << shift) + (1 << (shift-1));
        }
        else
        {
            xflag = 0;
            dy0 = b > 0 ? 1 : -1;
            dx0 = round( a*(1 << shift)/RPPABS(b) );
            x0 = (x0 << shift) + (1 << (shift-1));
        }

        for( k = 0; k < 4; k += 2 )
        {
            Rpp32s gap = 0, x = x0, y = y0, dx = dx0, dy = dy0;

            if( k > 0 )
                dx = -dx, dy = -dy;

            // Walk along the line, Stop at either the image border or in case of too big gap
            for( ;; x += dx, y += dy )
            {
                Rpp32s i1, j1;

                if( xflag )
                {
                    j1 = x;
                    i1 = y >> shift;
                }
                else
                {
                    j1 = x >> shift;
                    i1 = y;
                }

                if( j1 < 0 || j1 >= srcSize.width || i1 < 0 || i1 >= srcSize.height )
                    break;

                validPixelMaskTemp = validPixelMask + (i1 * srcSize.width) + j1;

                // For all valid points update line end, clear the mask element and reset the gap
                if(*validPixelMaskTemp)
                {
                    gap = 0;
                    *(endpoints + k + 1) = i1;
                    *(endpoints + k) = j1;
                }
                else if( ++gap > lineGap )
                    break;
            }
        }

        good_line = RPPABS(*(endpoints + 2) - *(endpoints + 0)) >= lineLength ||
                    RPPABS(*(endpoints + 3) - *(endpoints + 1)) >= lineLength;

        for( k = 0; k < 4; k += 2 )
        {
            int x = x0, y = y0, dx = dx0, dy = dy0;

            if( k > 0 )
                dx = -dx, dy = -dy;

            // Walk along the line, Stop at either the image border or in case of too big gap
            for( ;; x += dx, y += dy )
            {
                Rpp32s i1, j1;

                if( xflag )
                {
                    j1 = x;
                    i1 = y >> shift;
                }
                else
                {
                    j1 = x >> shift;
                    i1 = y;
                }

                validPixelMaskTemp = validPixelMask + (i1 * srcSize.width) + j1;

                // For all valid points update line end, clear the mask element and reset the gap
                if(*validPixelMaskTemp)
                {
                    if( good_line )
                    {
                        accumulatorTemp2 = accumulator;
                        cosLookUpTableTemp = cosLookUpTable;
                        sinLookUpTableTemp = sinLookUpTable;
                        for( int n = 0; n < accumulatorSize.height; n++, accumulatorTemp2 += accumulatorSize.width )
                        {
                            Rpp32s r = round( j1 * *cosLookUpTableTemp + i1 * *sinLookUpTableTemp);
                            r += (accumulatorSize.width - 1) / 2;
                            *(accumulatorTemp2 + r) -= 1;
                            cosLookUpTableTemp++;
                            sinLookUpTableTemp++;
                        }
                    }
                    *validPixelMaskTemp = (T) 0;
                }

                if( i1 == *(endpoints + k + 1) && j1 == *(endpoints + k) )
                    break;
            }
        }

        if( good_line )
        {
            *linesTemp = (U) *(endpoints + 0);
            linesTemp++;
            *linesTemp = (U) *(endpoints + 1);
            linesTemp++;
            *linesTemp = (U) *(endpoints + 2);
            linesTemp++;
            *linesTemp = (U) *(endpoints + 3);
            linesTemp++;

            numofLines++;
            if(numofLines >= linesMax)
                return RPP_SUCCESS;
        }
    }

    return RPP_SUCCESS;
}