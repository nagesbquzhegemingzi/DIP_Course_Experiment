#pragma once
#include "PreDefinedArgs.h"
#include <stdexcept>

using std::vector;

const size_t GramCutVar[HistogramBlockNum + 1] = { 0, 20, 40, 60, 80, 100, 120, 140, 160, 180 };

void buildHistogram(vector<vector<double>>& ManGrad, vector<vector<double>>& DirGrad, vector<vector<double>>& htgram, LONG biWidth, LONG biHeight);