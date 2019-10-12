#include <cpu/rpp_cpu_common.hpp>

template <typename T>
RppStatus optical_flow_pyramid_host(T* srcPtr1, T* srcPtr2, RppiSize srcSize, T* dstPtr, 
                                    Rpp32u* oldPoints, Rpp32u* newPointsEstimates, Rpp32u* newPoints, 
                                    Rpp32u numPoints, Rpp32f threshold, Rpp32u numIterations, Rpp32u kernelSize, 
                                    RppiChnFormat chnFormat, Rpp32u channel)
{
    // RGB to Greyscale Conversion
    
    Rpp32u imageDim = srcSize.height * srcSize.width;
    T *srcPtr1Greyscale = (T *)calloc(imageDim, sizeof(T));
    compute_rgb_to_greyscale_host(srcPtr1, srcSize, srcPtr1Greyscale, chnFormat, channel);
    T *srcPtr2Greyscale = (T *)calloc(imageDim, sizeof(T));
    compute_rgb_to_greyscale_host(srcPtr2, srcSize, srcPtr2Greyscale, chnFormat, channel);
    //memcpy(dstPtr, srcPtr2Greyscale, imageDim * sizeof(T));

    Rpp32u newChannel = 1;

    // Scharr Filter
    
    RppiSize rppiScharrKernelSize, srcSizeMod;
    Rpp32u scharrKernelSize = 3;
    int scharrBound = (scharrKernelSize - 1) / 2;

    srcSizeMod.width = srcSize.width + (2 * scharrBound);
    srcSizeMod.height = srcSize.height + (2 * scharrBound);

    T *srcPtrModScharr = (T *)calloc(srcSizeMod.height * srcSizeMod.width * newChannel, sizeof(T));
    generate_evenly_padded_image_host(srcPtr1Greyscale, srcSize, srcPtrModScharr, srcSizeMod, chnFormat, newChannel);

    rppiScharrKernelSize.height = scharrKernelSize;
    rppiScharrKernelSize.width = scharrKernelSize;

    Rpp32f *kernelX = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));
    generate_scharr_kernel_host(kernelX, 1);
    T *srcPtrDerivativeX = (T *)calloc(srcSize.height * srcSize.width * newChannel, sizeof(T));
    convolve_image_host(srcPtrModScharr, srcSizeMod, srcPtrDerivativeX, srcSize, kernelX, rppiScharrKernelSize, chnFormat, newChannel);
    //memcpy(dstPtr, srcPtrDerivativeX, imageDim * sizeof(T));

    Rpp32f *kernelY = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));
    generate_scharr_kernel_host(kernelY, 2);
    T *srcPtrDerivativeY = (T *)calloc(srcSize.height * srcSize.width * newChannel, sizeof(T));
    convolve_image_host(srcPtrModScharr, srcSizeMod, srcPtrDerivativeY, srcSize, kernelY, rppiScharrKernelSize, chnFormat, newChannel);
    //memcpy(dstPtr, srcPtrDerivativeY, imageDim * sizeof(T));

    // Pad x and y gradient images
    
    int bound = (kernelSize - 1) / 2;
    RppiSize srcSizeDerivativeMod;
    srcSizeDerivativeMod.height = srcSize.height + (2 * bound);
    srcSizeDerivativeMod.width = srcSize.width + (2 * bound);

    T *srcPtrDerivativeXmod = (T *)calloc(srcSizeDerivativeMod.height * srcSizeDerivativeMod.width * newChannel, sizeof(T));
    generate_evenly_padded_image_host(srcPtrDerivativeX, srcSize, srcPtrDerivativeXmod, srcSizeDerivativeMod, chnFormat, newChannel);

    T *srcPtrDerivativeYmod = (T *)calloc(srcSizeDerivativeMod.height * srcSizeDerivativeMod.width * newChannel, sizeof(T));
    generate_evenly_padded_image_host(srcPtrDerivativeY, srcSize, srcPtrDerivativeYmod, srcSizeDerivativeMod, chnFormat, newChannel);

    printf("\nbound = %d, kernelSize = %d", bound, kernelSize);

    // Create padded images for both inputs based on kernelSize to find residual for Lucas-Kanade
    
    srcSizeMod.height = srcSizeDerivativeMod.height;
    srcSizeMod.width = srcSizeDerivativeMod.width;

    T *srcPtr1Mod = (T *)calloc(srcSizeMod.height * srcSizeMod.width * newChannel, sizeof(T));
    generate_evenly_padded_image_host(srcPtr1Greyscale, srcSize, srcPtr1Mod, srcSizeMod, chnFormat, newChannel);

    T *srcPtr2Mod = (T *)calloc(srcSizeMod.height * srcSizeMod.width * newChannel, sizeof(T));
    generate_evenly_padded_image_host(srcPtr2Greyscale, srcSize, srcPtr2Mod, srcSizeMod, chnFormat, newChannel);

    T *srcPtr1Window, *srcPtr2Window;

    Rpp32u remainingElementsInRow = srcSizeDerivativeMod.width - kernelSize;

    // Run Lucas-Kanade algorithm for every pixel in the list of oldPoints

    T *srcPtrWindowX, *srcPtrWindowY;
    Rpp32u *oldPointsTemp, *newPointsEstimatesTemp, *newPointsTemp;
    oldPointsTemp = oldPoints;
    newPointsEstimatesTemp = newPointsEstimates;
    newPointsTemp = newPoints;

    Rpp32s rowOld, colOld, rowNewEst, colNewEst, rowNew, colNew, lostTrackFlag = 0;
    Rpp32f VxPrev, VxIntermediate, VxDiff, Vx, VyPrev, VyIntermediate, VyDiff, Vy;
    //Rpp32f colNewFloat, rowNewFloat;
    Rpp32f residual;

    memcpy(newPoints, oldPoints, 2 * numPoints * sizeof(Rpp32u));

    for(int i = 0; i < numPoints; i++)
    {
        colOld = *oldPointsTemp;
        oldPointsTemp++;
        rowOld = *oldPointsTemp;
        oldPointsTemp++;

        colNewEst = *newPointsEstimatesTemp;
        newPointsEstimatesTemp++;
        rowNewEst = *newPointsEstimatesTemp;
        newPointsEstimatesTemp++;

        VxPrev = colNewEst - colOld;
        VyPrev = rowNewEst - rowOld;

        printf("\n\n\ncolOld,rowOld = %d,%d and colNewEst,rowNewEst = %d,%d", colOld,rowOld,colNewEst,rowNewEst);
        printf("\nVxPrev,VyPrev = %f,%f", VxPrev, VyPrev);

        srcPtrWindowX = srcPtrDerivativeXmod + (rowOld * srcSizeDerivativeMod.width) + colOld;
        srcPtrWindowY = srcPtrDerivativeYmod + (rowOld * srcSizeDerivativeMod.width) + colOld;

        //Rpp32f* G = (Rpp32f*) calloc(4, sizeof(Rpp32f));
        //Rpp32f* Ginverse = (Rpp32f*) calloc(4, sizeof(Rpp32f));
        //Rpp32f Vx, Vy;

        lucas_kanade_matrix_kernel_host(srcPtrWindowX, srcPtrWindowY, srcSize, &VxIntermediate, &VyIntermediate, 
                                        kernelSize, remainingElementsInRow, lostTrackFlag);

        printf("\nPre-Iteration VxIntermediate, VyIntermediate = %f,%f and lostTrackFlag = %d", VxIntermediate, VyIntermediate, lostTrackFlag);
        
        if (lostTrackFlag == 1)
        {
            lostTrackFlag = 0;
            newPointsTemp += 2;
            continue;
        }

        Rpp32f It;

        for (int j = 0; j < numIterations; j++)
        {
            printf("\n\nIteration - %d", j+1);

            It = *(srcPtr1Greyscale + (rowOld * srcSize.width) + colOld) - *(srcPtr2Greyscale + (rowNewEst * srcSize.width) + colNewEst);
            printf("\nIt = %f", It);

            VxDiff = VxIntermediate * It;
            VyDiff = VyIntermediate * It;
            printf("\nVxDiff = %f, VyDiff = %f", VxDiff, VyDiff);

            //if (VxDiff < threshold || VyDiff < threshold)
            //{
            //    break;
            //}
            
            Vx = VxPrev + VxDiff;
            Vy = VyPrev + VyDiff;
            printf("\nVx = %f, Vy = %f", Vx, Vy);

            VxPrev = Vx;
            VyPrev = Vy;

            colNewEst = colOld + VxPrev;
            rowNewEst = rowOld + VxPrev;
            printf("\ncolNewEst,rowNewEst = %d,%d",colNewEst,rowNewEst);

            //srcPtrWindowX = srcPtrDerivativeXmod + (rowOld * srcSizeDerivativeMod.width) + colOld;
            //srcPtrWindowY = srcPtrDerivativeYmod + (rowOld * srcSizeDerivativeMod.width) + colOld;
            //lucas_kanade_residual_kernel_host(srcPtrWindowX, srcPtrWindowY, srcSize, residual, 
            //                            kernelSize, remainingElementsInRow, );
            srcPtr1Window = srcPtr1Mod + (rowOld * srcSizeMod.width) + colOld;
            srcPtr2Window = srcPtr2Mod + (rowNewEst * srcSizeMod.width) + colNewEst;
            lucas_kanade_residual_kernel_host(srcPtr1Window, srcPtr2Window, srcSize, &residual, 
                                              kernelSize, remainingElementsInRow);
            printf("\nResidual = %f", residual);

            if (residual < threshold)
            {
                break;
            }
        }

        printf("\nFinished point!\n\n");
        *newPointsTemp = (Rpp32u) colNewEst;
        newPointsTemp++;
        *newPointsTemp = (Rpp32u) rowNewEst;
        newPointsTemp++;
    }

    return RPP_SUCCESS;

}






// Working Version
/*
{
    // RGB to Greyscale Conversion
    
    Rpp32u imageDim = srcSize.height * srcSize.width;
    T *srcPtr1Greyscale = (T *)calloc(imageDim, sizeof(T));
    compute_rgb_to_greyscale_host(srcPtr1, srcSize, srcPtr1Greyscale, chnFormat, channel);
    T *srcPtr2Greyscale = (T *)calloc(imageDim, sizeof(T));
    compute_rgb_to_greyscale_host(srcPtr2, srcSize, srcPtr2Greyscale, chnFormat, channel);
    //memcpy(dstPtr, srcPtr2Greyscale, imageDim * sizeof(T));

    Rpp32u newChannel = 1;

    // Scharr Filter
    
    RppiSize rppiScharrKernelSize, srcSizeMod;
    Rpp32u scharrKernelSize = 3;
    int scharrBound = (scharrKernelSize - 1) / 2;

    srcSizeMod.width = srcSize.width + (2 * scharrBound);
    srcSizeMod.height = srcSize.height + (2 * scharrBound);

    T *srcPtrMod = (T *)calloc(srcSizeMod.height * srcSizeMod.width * newChannel, sizeof(T));
    generate_evenly_padded_image_host(srcPtr1Greyscale, srcSize, srcPtrMod, srcSizeMod, chnFormat, newChannel);

    rppiScharrKernelSize.height = scharrKernelSize;
    rppiScharrKernelSize.width = scharrKernelSize;

    Rpp32f *kernelX = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));
    generate_scharr_kernel_host(kernelX, 1);
    T *srcPtrDerivativeX = (T *)calloc(srcSize.height * srcSize.width * newChannel, sizeof(T));
    convolve_image_host(srcPtrMod, srcSizeMod, srcPtrDerivativeX, srcSize, kernelX, rppiScharrKernelSize, chnFormat, newChannel);
    //memcpy(dstPtr, srcPtrDerivativeX, imageDim * sizeof(T));

    Rpp32f *kernelY = (Rpp32f *)calloc(3 * 3, sizeof(Rpp32f));
    generate_scharr_kernel_host(kernelY, 2);
    T *srcPtrDerivativeY = (T *)calloc(srcSize.height * srcSize.width * newChannel, sizeof(T));
    convolve_image_host(srcPtrMod, srcSizeMod, srcPtrDerivativeY, srcSize, kernelY, rppiScharrKernelSize, chnFormat, newChannel);
    //memcpy(dstPtr, srcPtrDerivativeY, imageDim * sizeof(T));

    // Pad x and y gradient images
    
    int bound = (kernelSize - 1) / 2;
    RppiSize srcSizeDerivativeMod;
    srcSizeDerivativeMod.height = srcSize.height + (2 * bound);
    srcSizeDerivativeMod.width = srcSize.width + (2 * bound);

    T *srcPtrDerivativeXmod = (T *)calloc(srcSizeDerivativeMod.height * srcSizeDerivativeMod.width * newChannel, sizeof(T));
    generate_evenly_padded_image_host(srcPtrDerivativeX, srcSize, srcPtrDerivativeXmod, srcSizeDerivativeMod, chnFormat, newChannel);

    T *srcPtrDerivativeYmod = (T *)calloc(srcSizeDerivativeMod.height * srcSizeDerivativeMod.width * newChannel, sizeof(T));
    generate_evenly_padded_image_host(srcPtrDerivativeY, srcSize, srcPtrDerivativeYmod, srcSizeDerivativeMod, chnFormat, newChannel);

    printf("\nbound = %d, kernelSize = %d", bound, kernelSize);

    // Run Lucas-Kanade algorithm for every pixel in the list of oldPoints

    T *srcPtrWindowX, *srcPtrWindowY;
    Rpp32u *oldPointsTemp, *newPointsEstimatesTemp, *newPointsTemp;
    oldPointsTemp = oldPoints;
    newPointsEstimatesTemp = newPointsEstimates;
    newPointsTemp = newPoints;

    Rpp32s rowOld, colOld, rowNewEst, colNewEst, rowNew, colNew, lostTrackFlag = 0;
    Rpp32f VxPrev, VxIntermediate, VxDiff, Vx, VyPrev, VyIntermediate, VyDiff, Vy;
    //Rpp32f colNewFloat, rowNewFloat;
    Rpp32f residual;

    memcpy(newPoints, oldPoints, 2 * numPoints * sizeof(Rpp32u));

    for(int i = 0; i < numPoints; i++)
    {
        colOld = *oldPointsTemp;
        oldPointsTemp++;
        rowOld = *oldPointsTemp;
        oldPointsTemp++;

        colNewEst = *newPointsEstimatesTemp;
        newPointsEstimatesTemp++;
        rowNewEst = *newPointsEstimatesTemp;
        newPointsEstimatesTemp++;

        VxPrev = colNewEst - colOld;
        VyPrev = rowNewEst - rowOld;

        printf("\n\n\ncolOld,rowOld = %d,%d and colNewEst,rowNewEst = %d,%d", colOld,rowOld,colNewEst,rowNewEst);
        printf("\nVxPrev,VyPrev = %f,%f", VxPrev, VyPrev);

        srcPtrWindowX = srcPtrDerivativeXmod + (rowOld * srcSizeDerivativeMod.width) + colOld;
        srcPtrWindowY = srcPtrDerivativeYmod + (rowOld * srcSizeDerivativeMod.width) + colOld;

        Rpp32u remainingElementsInRow = srcSizeDerivativeMod.width - kernelSize;

        //Rpp32f* G = (Rpp32f*) calloc(4, sizeof(Rpp32f));
        //Rpp32f* Ginverse = (Rpp32f*) calloc(4, sizeof(Rpp32f));
        //Rpp32f Vx, Vy;

        lucas_kanade_matrix_kernel_host(srcPtrWindowX, srcPtrWindowY, srcSize, &VxIntermediate, &VyIntermediate, 
                                        kernelSize, remainingElementsInRow, lostTrackFlag);

        printf("\nPre-Iteration VxIntermediate, VyIntermediate = %f,%f and lostTrackFlag = %d", VxIntermediate, VyIntermediate, lostTrackFlag);
        
        if (lostTrackFlag == 1)
        {
            lostTrackFlag = 0;
            newPointsTemp += 2;
            continue;
        }

        Rpp32f It;

        for (int j = 0; j < numIterations; j++)
        {
            It = *(srcPtr1Greyscale + (rowOld * srcSize.width) + colOld) - *(srcPtr2Greyscale + (rowNewEst * srcSize.width) + colNewEst);
            printf("\n\nIt = %f", It);

            VxDiff = VxIntermediate * It;
            VyDiff = VyIntermediate * It;
            printf("\nVxDiff = %f, VyDiff = %f", VxDiff, VyDiff);

            if (VxDiff < threshold || VyDiff < threshold)
            {
                break;
            }
            
            Vx = VxPrev + VxDiff;
            Vy = VyPrev + VyDiff;
            printf("\nVx = %f, Vy = %f", Vx, Vy);

            VxPrev = Vx;
            VyPrev = Vy;

            colNewEst = colOld + VxPrev;
            rowNewEst = rowOld + VxPrev;
            printf("\ncolNewEst,rowNewEst = %d,%d",colNewEst,rowNewEst);

            //srcPtrWindowX = srcPtrDerivativeXmod + (rowOld * srcSizeDerivativeMod.width) + colOld;
            //srcPtrWindowY = srcPtrDerivativeYmod + (rowOld * srcSizeDerivativeMod.width) + colOld;
            //lucas_kanade_residual_kernel_host(srcPtrWindowX, srcPtrWindowY, srcSize, residual, 
            //                            kernelSize, remainingElementsInRow, );
        }

        printf("\nFinished point!\n\n");
        *newPointsTemp = (Rpp32u) colNewEst;
        newPointsTemp++;
        *newPointsTemp = (Rpp32u) rowNewEst;
        newPointsTemp++;
    }

    return RPP_SUCCESS;

}
*/








// Lucas Kanade from AMDRPP gpu
/*
??
*/






















// Lucas Kanade from website
/*
Mat lucasKanade(Mat imgA, Mat imgB, int xsarea, int ysarea, int xarea, int yarea, deque<Point> corners, bool verbose=true, char filename[] = NULL)
{
    Mat outimg = imgB.clone();
    
    int dimx = imgA.cols, dimy = imgA.rows;
    
    //iterate through each corner to find flow vectors
    for(int i=0;i<corners.size();i++)
    {
        Point cur_corner = corners[i];
        Point corner_mid = Point(corners[i].x+(int)(xarea/2),cur_corner.y+(int)(yarea/2));
        
        //Draw the corner in the out-image
        rectangle(outimg, cur_corner, cur_corner+Point(xarea,yarea), Scalar(0), 2);
        
        //Set range parameters
        Range sry = Range(max(0,corner_mid.y-(int)(ysarea/2)), min(dimy,corner_mid.y+(int)(ysarea/2)));
        Range srx = Range(max(0,corner_mid.x-(int)(xsarea/2)), min(dimx,corner_mid.x+(int)(xsarea/2)));
        
        //Now that we've found search windows, we can proceed to calculate Ix and Iy for each pixel in the search window
        //Ix
        Mat Ix = imgA.clone(), Iy=imgA.clone();
        
        int range=1;
        double G[2][2] = {0,0,0,0};
        double b[2] = {0,0};
        
        for(int x=srx.start; x<=srx.end; x++)
            for(int y=sry.start;y<=sry.end;y++)
            {
                int px=x-range, nx=x+range;
                int py=y-range, ny=y+range;
                if(x==0) px=x;
                if(x>=(dimx-1)) nx=x;
                if(y==0) py=0;
                if(y>=(dimy-1)) ny=y;

                double curIx = ((int)imgA.at<uchar>(y,px)-(int)imgA.at<uchar>(y,nx))/2;
                double curIy = ((int)imgA.at<uchar>(py,x)-(int)imgA.at<uchar>(ny,x))/2;
                Ix.at<uchar>(y,x) = curIx;
                Iy.at<uchar>(y,x) = curIy;
                
                //calculate G and b
                G[0][0] += curIx*curIx;
                G[0][1] += curIx*curIy;
                G[1][0] += curIx*curIy;
                G[1][1] += curIy*curIy;
                
                double curdI = ((int)imgA.at<uchar>(y,x)-(int)imgB.at<uchar>(y,x));
                
                b[0] += curdI*curIx;
                b[1] += curdI*curIy;
            }
        
        //Since it's a royal pain-in-the-ass to download and link matrix libraries to my XCode
        //for just a single operation, we're just gonna do it by hand
        double detG = (G[0][0]*G[1][1])-(G[0][1]*G[1][0]);
        double Ginv[2][2] = {0,0,0,0};
        Ginv[0][0] = G[1][1]/detG;
        Ginv[0][1] = -G[0][1]/detG;
        Ginv[1][0] = -G[1][0]/detG;
        Ginv[1][1] = G[0][0]/detG;
        
        double V[2] = {Ginv[0][0]*b[0]+Ginv[0][1]*b[1],Ginv[1][0]*b[0]+Ginv[1][1]*b[1]};
        if(verbose)
            printf("\nFor the corner (%d,%d) - v = (%f,%f)",cur_corner.x,cur_corner.y,V[0],V[1]);
        
        line(outimg, corner_mid, corner_mid+Point(V[0]*10,V[1]*10), Scalar(1), 1);
    }
    string winCorImg = "Corners found";
    namedWindow(winCorImg, WINDOW_AUTOSIZE);
    imshow(winCorImg, outimg);
    waitKey(0);
    if(filename!=NULL)
    {
        cout << string("\nWriting lkoutput to o") << string(filename);
        imwrite(string("o")+string(filename), outimg);
    }
    return outimg;
}

int main(int ac, char** av) {
    
//    if (ac != 2) {
//        help(av);
//        return 1;
//    }
//    std::string arg = av[1];
//    VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file or image sequence
//    if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
//        capture.open(atoi(arg.c_str()));
//    if (!capture.isOpened()) {
//        cerr << "Failed to open the video device, video file or image sequence!\n" << endl;
//        help(av);
//        return 1;
//    }
//    return process(capture);
    
//    Step 1 - Implementing Corner Detection
//    Read the file
    Mat src = imread(av[1]);
    Mat src_color, img1, img2; //To store the file after conversion
    cvtColor(src, src_color, CV_BGR2GRAY);
    src = imread("filename000.jpg");
    cvtColor(src, img1, CV_BGR2GRAY);
    src = imread("filename001.jpg");
    cvtColor(src, img2, CV_BGR2GRAY);
    
    printf("Dimensions of the Image: %d, %d", src_color.cols, src_color.rows);
    
    for(;;)
    {
        cout << "\nPlease enter x,y parameters to read intensity (-1to begin corner detection):";
        int x,y;
        scanf("%d,%d",&x,&y);
        if (x<0 || y<0) break;
        int i = src_color.at<uchar>(y,x);
        printf("\n\nYou entered x - %d and y - %d.\nIntensity(%d,%d) = %d:", x,y,x,y,i);
    }
    
    int xarea=10,yarea=10,thres=8;
    
    cout << "\n Enter 1 to start Lucas-Kanade, 0 to skip: ";
    
    int inp=0;
    scanf("%d",&inp);
    if(inp==1)
    {
        //    Step 2 - Implementing Lucas-Kanade Tracker
        Mat previmg = img1, curimg;
        for(int i=1; i < 30;i++)
        {
            char buffer[17];
        
            sprintf(buffer, "filename%.3d.jpg", i-1);
            printf("\nLoaded %s for prev",buffer);
            src = imread(buffer);
            cvtColor(src, previmg, CV_BGR2GRAY);
        
            sprintf(buffer, "filename%.3d.jpg", i);
            printf("\nLoaded %s",buffer);
            src = imread(buffer);
            cvtColor(src, curimg, CV_BGR2GRAY);
        
            deque<Point> corn = findCorners(previmg, xarea, yarea, thres, false);
        
            lucasKanade(previmg, curimg, 3, 3, xarea, yarea, corn, true, buffer);
        }
    }

    
    //Next we try to implement Horn-Shunck
}
*/