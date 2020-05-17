#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <random>
#include <string>
#include <cstdio>
#include "SVM/mySVM.h"
#include "HOG_Process.h"

using namespace Eigen;
using namespace std;
const int POS_NUM = 150;
const int NEG_NUM = 90;
const int DATA_DIMENSION = 3780;
const int TEST_NUM = 20;

int main()
{
	vector<vector<double>> posSetExt(POS_NUM, vector<double>(DATA_DIMENSION));
	vector<vector<double>> negSetExt(NEG_NUM, vector<double>(DATA_DIMENSION));
	vector<vector<double>> predictdata(TEST_NUM * 2, vector<double>(DATA_DIMENSION));
	string pNameTop = "./Data/POS/per (";
	string nNameTop = "./Data/NEG/nop (";
	string fNameTail = ").jpg";
	string pFileName;
	string nFileName;

	/*
	for (size_t i = 1; i <= POS_NUM; i++)
	{
		string mid = to_string(i);
		pFileName = pNameTop + mid + fNameTail;
		HOG_Process(pFileName.c_str(), posSetExt[i - 1]);
	}
	for (size_t i = 1; i <= NEG_NUM; i++)
	{
		string mid = to_string(i);
		nFileName = nNameTop + mid + fNameTail;
		HOG_Process(nFileName.c_str(), negSetExt[i - 1]);
	}
	*/
	
	mySVM svm(POS_NUM + NEG_NUM, DATA_DIMENSION);
	svm.loadSVM("./svm_test1.svm");
	
	/*
	svm.feedFeature(posSetExt, true);
	svm.feedFeature(negSetExt, false);
	cout << "Training..." << endl;
	svm.train();
	svm.saveSVM("./svm_test1.svm");
	*/
	
	for (size_t i = 1; i <= TEST_NUM + 60; i++)
	{
		if (i % 2 == 0)
		{
			string mid = to_string(i);
			pFileName = pNameTop + mid + fNameTail;
			HOG_Process(pFileName.c_str(), predictdata[i - 1]);
		}
	}

	vector<int> res = svm.predict(predictdata);
	int count = 1;
	cout << "1: ";
	for (int i : res) {
		cout << i << ' ';
		if (count % 10 == 0) {
			cout << endl;
			cout << count << ": ";
		}
		count++;
	}
	

	return 0;
}