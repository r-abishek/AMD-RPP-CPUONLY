void inputPlanar(int *intSrcPtr, RppiSize srcSize, Rpp32u channel)
{
    int p = 0;
    for(int c = 0; c < channel; c++)
    {
        printf("\n\nEnter %d elements for channel %d:\n", (srcSize.height * srcSize.width), c+1);
        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            scanf("%d", intSrcPtr + p);
            p += 1;
        }
    }
}

void inputPacked(int *intSrcPtr, RppiSize srcSize, Rpp32u channel)
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

void inputPlanarF(Rpp32f *Rpp32fSrcPtr, RppiSize srcSize, Rpp32u channel)
{
    int p = 0;
    for(int c = 0; c < channel; c++)
    {
        printf("\n\nEnter %d elements for channel %d:\n", (srcSize.height * srcSize.width), c+1);
        for (int i = 0; i < (srcSize.height * srcSize.width); i++)
        {
            scanf("%f", Rpp32fSrcPtr + p);
            p += 1;
        }
    }
}

void inputPackedF(Rpp32f *Rpp32fSrcPtr, RppiSize srcSize, Rpp32u channel)
{
    int p = 0;
    printf("\n\nEnter %d elements for the image:\n", (channel * srcSize.height * srcSize.width));
    for (int i = 0; i < (srcSize.height * srcSize.width); i++)
    {
        for (int c = 0; c < channel; c++)
        {
            printf("Channel %d - ", c+1);
            scanf("%f", Rpp32fSrcPtr + p);
            p += 1;
        }
        printf("\n");
    }
}

void cast(int *intSrcPtr, Rpp8u *srcPtr, RppiSize srcSize, Rpp32u channel)
{
    for (int i = 0; i < (channel * srcSize.height * srcSize.width); i++)
    {
        intSrcPtr[i] = (intSrcPtr[i] < 255) ? intSrcPtr[i] : 255;
        intSrcPtr[i] = (intSrcPtr[i] > 0) ? intSrcPtr[i] : 0;
        srcPtr[i] = (Rpp8u) intSrcPtr[i];

    }
}

template <typename T>
void displayPlanar(T *pArr, RppiSize size, Rpp32u channel)
{
    int p = 0;
    for(int c = 0; c < channel; c++)
    {
        printf("\n\nChannel %d:\n", c+1);
        for (int i = 0; i < (size.height * size.width); i++)
        {
            printf("%d\t\t", (int) *(pArr + p));
            if (((i + 1) % size.width) == 0)
            {
                printf("\n");
            }
            p += 1;
        }
    }
}

template <typename T>
void displayPacked(T *pArr, RppiSize size, Rpp32u channel)
{
    int p = 0;
    for (int i = 0; i < size.height; i++)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int j = 0; j < size.width; j++)
            {
                printf("%d\t\t", (int) *(pArr + p + c + (j * channel)));
            }
            printf("\n");
        }
        printf("\n");
        p += (channel * size.width);
    }
}

void displayPlanarF(Rpp32f *pArr, RppiSize size, Rpp32u channel)
{
    int p = 0;
    for(int c = 0; c < channel; c++)
    {
        printf("\n\nChannel %d:\n", c+1);
        for (int i = 0; i < (size.height * size.width); i++)
        {
            printf("%0.6f\t\t", *(pArr + p));
            if (((i + 1) % size.width) == 0)
            {
                printf("\n");
            }
            p += 1;
        }
    }
}

void displayPackedF(Rpp32f *pArr, RppiSize size, Rpp32u channel)
{
    int p = 0;
    for (int i = 0; i < size.height; i++)
    {
        for (int c = 0; c < channel; c++)
        {
            for (int j = 0; j < size.width; j++)
            {
                printf("%0.6f\t\t", *(pArr + p + c + (j * channel)));
            }
            printf("\n");
        }
        printf("\n");
        p += (channel * size.width);
    }
}

template <typename T>
void displayTensor(T *pArr, Rpp32u size)
{
    int p = 0, count = 0;
    for (int i = 0; i < size; i++, count++)
    {
        printf("%d\t", (int) *(pArr + p));
        p++;
        if (count == 19)
        {
            printf("\n");
            count = 0;
        }
    }
    printf("\n");
}

template <typename T>
void displayTensorF(T *pArr, Rpp32u size)
{
    int p = 0, count = 0;
    for (int i = 0; i < size; i++, count++)
    {
        printf("%0.6f\t", (int) *(pArr + p));
        p++;
        if (count == 19)
        {
            printf("\n");
            count = 0;
        }
    }
    printf("\n");
}