#include "GradGraph.h"

void GradGraph(vector<vector<double>>& ManGrad, vector<vector<double>>& DirGrad, vector<vector<BYTE>> imgBuf, LONG biWidth, LONG biHeight)
{
	for (size_t i = 0; i < biHeight; i++)
	{
		for (size_t j = 0; j < biWidth; j++)
		{
			if (i == 0 || i == biHeight - (size_t)1)
			{
				DirGrad[i][j] = 0;
				ManGrad[i][j] = 0;
			}
			else if (j == 0 || j == biWidth - (size_t)1)
			{
				DirGrad[i][j] = 0;
				ManGrad[i][j] = 0;
			}
			else
			{
				int gx = (int)imgBuf[i + 1][j] - (int)imgBuf[i - 1][j];
				int gy = (int)imgBuf[i][j + 1] - (int)imgBuf[i][j - 1];
				double pi = acos(-1);
				if (gx == 0)
				{
					DirGrad[i][j] = 180;
				}
				else
				{
					double gradOri = atan((double)gy / (double)gx) / pi * 180;
					gradOri = gradOri < 0 ? gradOri + 180 : gradOri;
					DirGrad[i][j] = gradOri;
				}
				ManGrad[i][j] = hypot(gx, gy);
			}
		}
	}
}