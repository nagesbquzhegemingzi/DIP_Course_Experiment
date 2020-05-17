#include "HOG_Process.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb-master/stb_image_write.h"

void HOG_Process(const char* fileName, vector<double>& dsetExt)
{
	int biWidth, biHeight, biTunnel;
	BYTE* dBuf = stbi_load(fileName, &biWidth, &biHeight, &biTunnel, 0);
	int biSizeImage = biWidth * biHeight * 3;

	if (biSizeImage <= 3 || biHeight <= 1 || biWidth <= 1)
	{
		throw std::exception();
	}

	vector<vector<BYTE>> imgBuf(biHeight, vector<BYTE>(biWidth));
	for (int i = 0; i < biHeight; i++)
	{
		for (int j = 0; j < biWidth; j++)
		{
			imgBuf[i][j] = (dBuf[i * biWidth * 3 + j * 3 + 0] + dBuf[i * biWidth * 3 + j * 3 + 1] + dBuf[i * biWidth * 3 + j * 3 + 2]) / 3;
		}
	}
	GammaCorrection(GammaCorrectionMagicNumber, imgBuf, imgBuf[0].size(), imgBuf.size());
	delete[] dBuf;

	vector<vector<double>> ManGrad((size_t)biHeight, vector<double>(biWidth));
	vector<vector<double>> DirGrad((size_t)biHeight, vector<double>(biWidth));
	GradGraph(ManGrad, DirGrad, imgBuf, biWidth, biHeight);

	size_t widSquareNum = biWidth / HistogramBlockEdge;
	size_t heiSquareNum = biHeight / HistogramBlockEdge;
	//ֱ��ͼ�ķŷ���ѡ��һ����ά��������һά��ʾSquare��λ�ã���˳���ʾ���ڶ�ά��ʾֱ��ͼ�����ݣ�ͬ����˳���ʾ��
	vector<vector<double>> htgram(widSquareNum * heiSquareNum, vector<double>(HistogramBlockNum));
	//����ͼƬ�ĳ��������ܱ�HistoGramBlockEdge����PreDefinedArgs.h�ж��壩����������ú��������쳣��
	buildHistogram(ManGrad, DirGrad, htgram, biWidth, biHeight);

	vector<vector<double>> nor_htgram_set((widSquareNum - 1) * (heiSquareNum - 1), vector<double>(HistogramBlockNum * 4));
	vectorNormalization(nor_htgram_set, htgram, widSquareNum, heiSquareNum);
	createDataSetVector(nor_htgram_set, dsetExt);

	return;
}