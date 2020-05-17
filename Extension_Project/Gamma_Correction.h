#pragma once
#include "PreDefinedArgs.h"

using std::vector;
static BYTE gammaCTable[256];

void BuildLookupTable(double CorrectionNumber);

void GammaCorrection(double CorrectionNumber, vector<vector<BYTE>>& imgBuf, size_t imgWidth, size_t imgHeight);