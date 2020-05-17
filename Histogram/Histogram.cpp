#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>

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

int main()
{
    FILE* imgIN, * imgOUT;
    if ((imgIN = fopen("low_contrast.bmp", "rb")) == nullptr)
    {
        printf("Failed to open img file.\n");
        return 0;
    }
    if ((imgOUT = fopen("high_contrast.bmp", "wb")) == nullptr)
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

    LONG biGraphSize = bi.biWidth * bi.biHeight;
    BYTE* dBuf = new BYTE[biGraphSize * 3];
    fread(dBuf, bi.biWidth * bi.biHeight * 3, 1, imgIN);

    double dSheet[256]; for (int i = 0; i < 256; i++) { dSheet[i] = 0; }
    for (DWORD i = 0; i < (DWORD)(biGraphSize * 3); i += 3)
    {
        dSheet[dBuf[i]]++;
    }
    for (int i = 0; i < 256; i++) { dSheet[i] /= biGraphSize; }

    int dTrans[256] = { 0 };
    double dSum = 0;
    for (int i = 1; i < 256; i++)
    {
        dSum += dSheet[i];
        dTrans[i] = int((256 - 1) * dSum + 0.5);
    }

    for (DWORD i = 0; i < (DWORD)(biGraphSize * 3); i += 3)
    {
        BYTE tmp = dTrans[dBuf[i]];
        dBuf[i] = dBuf[i + 1] = dBuf[i + 2] = tmp;
    }

    fwrite(dBuf, bi.biWidth * bi.biHeight * 3, 1, imgOUT);

    delete[] dBuf;
    fclose(imgIN);
    fclose(imgOUT);


    return 0;
}
