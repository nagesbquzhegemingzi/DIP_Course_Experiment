#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <random>
#include "mySVM.h"
using namespace Eigen;
using namespace arma;
using namespace std;
const int POSNUM = 50;
const int NAGNUM = 50;
const int FEATURENUM = 20;
int main()
{
	vector<int *> posdata, nagdata, testdata;
	default_random_engine e;
	normal_distribution<double> pos(15, 1);
	uniform_int_distribution<int> u(0, 100);
	// 构造正样本
	for (size_t i = 0; i < POSNUM; i++)
	{
		posdata.push_back(new int[FEATURENUM]);
		for (size_t ii = 0; ii < FEATURENUM; ii++)
			posdata[i][ii] = 0;
		for (size_t j = 0; j < FEATURENUM; j++)
		{
			int buf = floor(pos(e));
			if (buf < FEATURENUM && buf >= 0)
				posdata[i][buf]++;
		}
	}
	// 构造负样本
	for (size_t i = 0; i < NAGNUM; i++)
	{
		nagdata.push_back(new int[FEATURENUM]);
		for (size_t ii = 0; ii < FEATURENUM; ii++)
			nagdata[i][ii] = u(e);
	}
	mySVM svm(POSNUM + NAGNUM, FEATURENUM);
	svm.feedFeature(posdata, true);
	svm.feedFeature(nagdata, false);
	svm.train();
	svm.show_attributes();
	cout << endl;

	// 构造10个测试样本
	for (size_t i = 0; i < 5; i++)
	{
		testdata.push_back(new int[FEATURENUM]);
		for (size_t ii = 0; ii < FEATURENUM; ii++)
			testdata[i][ii] = 0;
		for (size_t j = 0; j < FEATURENUM; j++)
		{
			int buf = floor(pos(e));
			if (buf < FEATURENUM && buf >= 0)
				testdata[i][buf]++;
		}
	}
	for (size_t i = 5; i < 10; i++)
	{
		testdata.push_back(new int[FEATURENUM]);
		for (size_t ii = 0; ii < FEATURENUM; ii++)
			testdata[i][ii] = u(e);
	}
	//输出测试样本
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t ii = 0; ii < FEATURENUM; ii++)
			cout << testdata[i][ii] << ' ';
		cout << endl;
	}
	vector<int> result = svm.predict(testdata);
	int count = 1;
	cout << "1: ";
	for (int i : result)
	{
		cout << i << ' ';
		if (count % 10 == 0)
		{
			cout << endl;
			cout << count << ": ";
		}
		count++;
	}
	cout << endl;
	//vector<int *>a;
	//int aa[10] = { 0 };
	//a.push_back(aa);
	system("pause");
	return 0;
}