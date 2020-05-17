#include "Gamma_Correction.h"

void BuildLookupTable(double CorrectionNumber)
{
	double tmp;
	for (int i = 0; i < 256; i++)
	{
		tmp = (i + 0.5) / 256;
		tmp = std::pow(tmp, CorrectionNumber);
		gammaCTable[i] = (BYTE)(tmp * 256 - 0.5);
	}
}

void GammaCorrection(double CorrectionNumber, vector<vector<BYTE>>& imgBuf, size_t imgWidth, size_t imgHeight)
{
	BuildLookupTable(1 / CorrectionNumber);
	for (int i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; j++)
		{
			imgBuf[i][j] = gammaCTable[imgBuf[i][j]];
		}
	}
}