#include "Normalization.h"

void vectorNormalization(vector<vector<double>>& nor_htgram_set, vector<vector<double>>& htgram, size_t widSquareNum, size_t heiSquareNum)
{
	for (size_t i = 0; i < (widSquareNum - 1) * (heiSquareNum - 1); i++)
	{
		size_t ulpos = i / (widSquareNum - 1) * widSquareNum + i % (widSquareNum - 1);
		double PowerSum = 0;
		for (size_t j = 0; j < 4; j++)
		{
			for (size_t k = 0; k < HistogramBlockNum; k++)
			{
				switch (j)
				{
				case 0:
					nor_htgram_set[i][j * HistogramBlockNum + k] = htgram[ulpos][k];
					break;
				case 1:
					nor_htgram_set[i][j * HistogramBlockNum + k] = htgram[ulpos + 1][k];
					break;
				case 2:
					nor_htgram_set[i][j * HistogramBlockNum + k] = htgram[ulpos + widSquareNum][k];
					break;
				case 3:
					nor_htgram_set[i][j * HistogramBlockNum + k] = htgram[ulpos + widSquareNum + 1][k];
					break;
				default:
					break;
				}
				PowerSum += nor_htgram_set[i][j * HistogramBlockNum + k] * nor_htgram_set[i][j * HistogramBlockNum + k];
			}
		}
		double NormalizedSum = std::sqrt(PowerSum);
		for (size_t j = 0; j < HistogramBlockNum * 4; j++)
		{
			nor_htgram_set[i][j] /= NormalizedSum;
		}
	}
}

double* createDataSet(vector<vector<double>>& nor_htgram_set)
{
	size_t fir = nor_htgram_set.size(), sec = nor_htgram_set[0].size();
	double* DataSet = new double[fir * sec];
	for (size_t i = 0; i < fir; i++)
	{
		for (size_t j = 0; j < sec; j++)
		{
			if (fir * sec <= i * sec + j)
			{
				return nullptr;
			}
			DataSet[i * sec + j] = nor_htgram_set[i][j];
		}
	}

	return DataSet;
}

void createDataSetVector(vector<vector<double>>& nor_htgram_set, vector<double>& dset)
{
	size_t fir = nor_htgram_set.size(), sec = nor_htgram_set[0].size();

	for (size_t i = 0; i < fir; i++)
	{
		for (size_t j = 0; j < sec; j++)
		{
			if (fir * sec <= i * sec + j)
			{
				throw std::exception();
			}
			dset[i * sec + j] = nor_htgram_set[i][j];
		}
	}
}