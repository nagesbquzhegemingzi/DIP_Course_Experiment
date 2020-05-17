#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef long LONG;

const int _FILTER_OSD = 35;
const int _FILTER_NUM = _FILTER_OSD * _FILTER_OSD;

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
    if ((ImageSize = bi.biWidth * bi.biHeight) < _FILTER_NUM)
    {
        fclose(imgIN);
        fclose(imgOUT);
        return 0;
    }
    BYTE* dBuf = new BYTE[ImageSize * 3];
    fread(dBuf, ImageSize * 3, 1, imgIN);

    BYTE* newBuf = new BYTE[ImageSize * 3];
    BYTE BufMatrix[_FILTER_NUM];
    BYTE Filter[_FILTER_NUM];
    memset(Filter, 1, _FILTER_NUM);
    int CurrentPixel = 0;
    for (LONG i = 0; i < ImageSize * 3; i += 3)
    {
        if (i <= bi.biWidth * 3 * (_FILTER_OSD / 2) || i >= (bi.biHeight - (_FILTER_OSD / 2)) * bi.biWidth * 3)
        {
            newBuf[i] = newBuf[i + 1] = newBuf[i + 2] = dBuf[i];
            continue;
        }
        else if (i % (bi.biWidth * 3) <= ((_FILTER_OSD / 2) * 3) || i % (bi.biWidth * 3) >= ((bi.biWidth - (_FILTER_OSD / 2)) * 3))
        {
            newBuf[i] = newBuf[i + 1] = newBuf[i + 2] = dBuf[i];
            continue;
        }

        for (int j = 0; j < _FILTER_OSD; j++)
        {
            for (int k = 0; k < _FILTER_OSD; k++)
            {
                BufMatrix[j * _FILTER_OSD + k] = dBuf[i + (j - _FILTER_OSD / 2) * bi.biWidth * 3 + (k - _FILTER_OSD / 2) * 3];
            }
        }

        CurrentPixel = 0;
        for (int m = 0; m < _FILTER_NUM; m++)
        {
            CurrentPixel += (int)BufMatrix[m] * (int)Filter[m];
        }
        CurrentPixel /= _FILTER_NUM;

        newBuf[i] = newBuf[i + 1] = newBuf[i + 2] = (BYTE)CurrentPixel;
    }

    fwrite(newBuf, ImageSize * 3, 1, imgOUT);

    delete[] dBuf;
    fclose(imgIN);
    fclose(imgOUT);


    return 0;
}