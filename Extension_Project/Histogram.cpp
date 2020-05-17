#include "Histogram.h"

void buildHistogram(vector<vector<double>>& ManGrad, vector<vector<double>>& DirGrad, vector<vector<double>>& htgram, LONG biWidth, LONG biHeight)
{
	size_t widSquareNum = biWidth / HistogramBlockEdge;
	size_t heiSquareNum = biHeight / HistogramBlockEdge;
	if (biWidth % HistogramBlockEdge != 0 || biHeight % HistogramBlockEdge != 0)
	{
		throw std::invalid_argument("Parameter must be divisible by HistogramBlockEdge");
	}
	size_t htgBlockCount = HistogramBlockEdge * HistogramBlockEdge;
	for (size_t i = 0; i < widSquareNum * heiSquareNum; i++)
	{
		for (size_t k = 0; k < HistogramBlockNum; k++)
		{
			htgram[i][k] = 0;
		}
		for (size_t j = 0; j < htgBlockCount; j++)
		{
			size_t k;
			size_t thisRow = j / HistogramBlockEdge + i / widSquareNum * HistogramBlockEdge;
			size_t thisCol = j % HistogramBlockEdge + i % widSquareNum * HistogramBlockEdge;
			for (k = 1; GramCutVar[k] <= DirGrad[thisRow][thisCol] && k < 9; k++) {}
			if (k > 8)
			{
				htgram[i][k - 1] += (double)(ManGrad[thisRow][thisCol] * ((double)(GramCutVar[k] - DirGrad[thisRow][thisCol]) / (double)(GramCutVar[1] - GramCutVar[0])));
				htgram[i][0] += (double)(ManGrad[thisRow][thisCol] * ((double)(DirGrad[thisRow][thisCol] - GramCutVar[k - 1]) / (double)(GramCutVar[1] - GramCutVar[0])));
				continue;
			}
			else
			{
				htgram[i][k - 1] += (double)(ManGrad[thisRow][thisCol] * ((double)(GramCutVar[k] - DirGrad[thisRow][thisCol]) / (double)(GramCutVar[1] - GramCutVar[0])));
				htgram[i][k] += (double)(ManGrad[thisRow][thisCol] * ((double)(DirGrad[thisRow][thisCol] - GramCutVar[k - 1]) / (double)(GramCutVar[1] - GramCutVar[0])));
			}
		}
	}
}