#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>

using namespace std;

typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef long LONG;

#pragma pack(1)
typedef struct tagBITMAPFILEHEADER
{
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
}BITMAPFILEHEADER;
#pragma pack()

#pragma pack(1)
typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
}BITMAPINFOHEADER;
#pragma pack()

int cmp_BYTE(const void* a, const void* b)
{
    BYTE Arg1 = *(BYTE*)a;
    BYTE Arg2 = *(BYTE*)b;
    if (Arg1 < Arg2) return -1;
    if (Arg1 > Arg2) return 1;
    return 0;
}

int main()
{
    FILE* imgIN, * imgOUT;
    if ((imgIN = fopen("imgIN.bmp", "rb")) == nullptr)
    {
        printf("Failed to open img file.\n");
        return 0;
    }
    if ((imgOUT = fopen("imgOUT.bmp", "wb")) == nullptr)
    {
        printf("Failed to create img file.\n");
        return 0;
    }

    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, imgIN);
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, imgIN);
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, imgOUT);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, imgOUT);

    LONG ImageSize;
    if ((ImageSize = bi.biWidth * bi.biHeight) < 9)
    {
        fclose(imgIN);
        fclose(imgOUT);
        return 0;
    }
    BYTE* dBuf = new BYTE[ImageSize * 3];
    fread(dBuf, ImageSize * 3, 1, imgIN);

    BYTE Filter[3 * 3];
    for (LONG i = 0; i < ImageSize * 3; i += 3)
    {
        if (i < bi.biWidth * 3 || i >(bi.biHeight - 1) * bi.biWidth * 3)
        {
            continue;
        }
        else if (i % bi.biWidth == 0 || i % bi.biWidth == ((bi.biWidth - 1) * 3 + 1))
        {
            continue;
        }

        Filter[0] = dBuf[i - bi.biWidth * 3 - 3];
        Filter[1] = dBuf[i - bi.biWidth * 3];
        Filter[2] = dBuf[i - bi.biWidth * 3 + 3];
        Filter[3] = dBuf[i - 3];
        Filter[4] = dBuf[i];
        Filter[5] = dBuf[i + 3];
        Filter[6] = dBuf[i + bi.biWidth * 3 - 3];
        Filter[7] = dBuf[i + bi.biWidth * 3];
        Filter[8] = dBuf[i + bi.biWidth * 3 + 3];

        qsort(Filter, 9, sizeof(BYTE), cmp_BYTE);

        dBuf[i] = dBuf[i + 1] = dBuf[i + 2] = Filter[4];
    }

    fwrite(dBuf, ImageSize * 3, 1, imgOUT);

    delete[] dBuf;
    fclose(imgIN);
    fclose(imgOUT);


    return 0;
}