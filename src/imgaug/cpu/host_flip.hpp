template <typename T>
RppStatus host_flip(T* srcPtr, RppiSize srcSize, T* dstPtr, 
                    RppiAxis flipAxis,
                    RppiChnFormat chnFormat, unsigned channel)
{
    if (chnFormat == RPPI_CHN_PLANAR)
    {
        if (flipAxis == RPPI_HORIZONTAL_AXIS)
        {
            int srcLoc = 0, dstLoc = 0;
            for (int i = (srcSize.height - 1); i >= 0; i--)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    srcLoc = (i * srcSize.width) + j;
                    for (int c = 0; c < channel; c++)
                    {
                        dstPtr[dstLoc + (c * srcSize.height * srcSize.width)] = srcPtr[srcLoc + (c * srcSize.height * srcSize.width)];
                    }
                    dstLoc += 1;
                }
            }
        }
        else if (flipAxis == RPPI_VERTICAL_AXIS)
        {
            int srcLoc = 0, dstLoc = 0;
            for (int i = (srcSize.width - 1); i >= 0; i--)
            {
                dstLoc = srcSize.width - 1 - i;
                for (int j = 0; j < srcSize.height; j++)
                {
                    srcLoc = (j * srcSize.width) + i;
                    for (int c = 0; c < channel; c++)
                    {
                        dstPtr[dstLoc + (c * srcSize.height * srcSize.width)] = srcPtr[srcLoc + (c * srcSize.height * srcSize.width)];
                    }
                    dstLoc += srcSize.width;
                }
            }
        }
        else if (flipAxis == RPPI_BOTH_AXIS)
        {
            Rpp8u *pInter = (Rpp8u *)malloc(channel * srcSize.width * srcSize.height * sizeof(Rpp8u));
            int srcLoc = 0, interLoc = 0;
            for (int i = (srcSize.height - 1); i >= 0; i--)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    srcLoc = (i * srcSize.width) + j;
                    for (int c = 0; c < channel; c++)
                    {
                        pInter[interLoc + (c * srcSize.height * srcSize.width)] = srcPtr[srcLoc + (c * srcSize.height * srcSize.width)];
                    }
                    interLoc += 1;
                }
            }
            int dstLoc = 0;
            interLoc = 0;
            for (int i = (srcSize.width - 1); i >= 0; i--)
            {
                dstLoc = srcSize.width - 1 - i;
                for (int j = 0; j < srcSize.height; j++)
                {
                    interLoc = (j * srcSize.width) + i;
                    for (int c = 0; c < channel; c++)
                    {
                        dstPtr[dstLoc + (c * srcSize.height * srcSize.width)] = pInter[interLoc + (c * srcSize.height * srcSize.width)];
                    }
                    dstLoc += srcSize.width;
                }
            }
        }
    }
    else if (chnFormat == RPPI_CHN_PACKED)
    {
        if (flipAxis == RPPI_HORIZONTAL_AXIS)
        {
            int srcLoc = 0, dstLoc = 0;
            for (int i = (srcSize.height - 1); i >= 0; i--)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    srcLoc = (i * 3 * srcSize.width) + (3 * j);
                    for (int c = 0; c < channel; c++)
                    {
                        dstPtr[dstLoc + c] = srcPtr[srcLoc + c];
                    }
                    srcLoc += 3;
                    dstLoc += 3;
                }
            }
        }
        else if (flipAxis == RPPI_VERTICAL_AXIS)
        {
            int srcLoc = 0, dstLoc = 0;
            for (int i = (srcSize.width - 1); i >= 0; i--)
            {
                dstLoc = 3 * (srcSize.width - 1 - i);
                for (int j = 0; j < srcSize.height; j++)
                {
                    srcLoc = (j * 3 * srcSize.width) + (i * 3);
                    for (int c = 0; c < channel; c++)
                    {
                        dstPtr[dstLoc + c] = srcPtr[srcLoc + c];
                    }
                    dstLoc += (srcSize.width * 3);
                }
            }
        }
        else if (flipAxis == RPPI_BOTH_AXIS)
        {
            Rpp8u *pInter = (Rpp8u *)malloc(channel * srcSize.width * srcSize.height * sizeof(Rpp8u));
            int srcLoc = 0, interLoc = 0;

            for (int i = (srcSize.height - 1); i >= 0; i--)
            {
                for (int j = 0; j < srcSize.width; j++)
                {
                    srcLoc = (i * 3 * srcSize.width) + (3 * j);
                    for (int c = 0; c < channel; c++)
                    {
                        pInter[interLoc + c] = srcPtr[srcLoc + c];
                    }
                    srcLoc += 3;
                    interLoc += 3;
                }
            }

            int dstLoc = 0;
            interLoc = 0;


            for (int i = (srcSize.width - 1); i >= 0; i--)
            {
                dstLoc = 3 * (srcSize.width - 1 - i);
                for (int j = 0; j < srcSize.height; j++)
                {
                    interLoc = (j * 3 * srcSize.width) + (i * 3);
                    for (int c = 0; c < channel; c++)
                    {
                        dstPtr[dstLoc + c] = pInter[interLoc + c];
                    }
                    dstLoc += (srcSize.width * 3);
                }
            }
        }
    }

    return RPP_SUCCESS;
}