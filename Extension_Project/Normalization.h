#pragma once
#include "PreDefinedArgs.h"

using std::vector;

void vectorNormalization(vector<vector<double>>& nor_htgram_set, vector<vector<double>>& htgram, size_t widSquareNum, size_t heiSquareNum);

double* createDataSet(vector<vector<double>>& nor_htgram_set);

void createDataSetVector(vector<vector<double>>& nor_htgram_set, vector<double>& dset);