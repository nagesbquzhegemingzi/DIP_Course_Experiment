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
    if ((imgIN = fopen("in.bmp", "rb")) == nullptr)
    {
        printf("Failed to open img file.\n");
        return 0;
    }
    if ((imgOUT = fopen("out.bmp", "wb")) == nullptr)
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

    BYTE* dBuf = new BYTE[bi.biWidth * bi.biHeight * 3];
    fread(dBuf, bi.biWidth * bi.biHeight * 3, 1, imgIN);
    for (int i = 0; i < bi.biWidth * bi.biHeight; i++)
    {
        BYTE* p = dBuf + i * 3;
        BYTE tmp = (BYTE)(*(p) * 0.299 + *(p + 1) * 0.587 + *(p + 2) * 0.114);
        *(p) = *(p + 1) = *(p + 2) = tmp;
    }
    fwrite(dBuf, bi.biWidth * bi.biHeight * 3, 1, imgOUT);

    delete[] dBuf;
    fclose(imgIN);
    fclose(imgOUT);


    return 0;
}