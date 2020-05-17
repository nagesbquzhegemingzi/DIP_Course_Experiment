#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <cstdio>
#include "PreDefinedArgs.h"
#include "Gamma_Correction.h"
#include "GradGraph.h"
#include "Histogram.h"
#include "Normalization.h"

using std::vector;

void HOG_Process(const char* fileName, vector<double>& dsetExt);
