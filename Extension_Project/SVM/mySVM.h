#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <Eigen/Dense>

class mySVM
{
	//使用多项式核函数
public:
	mySVM(unsigned sample_num, unsigned feature_num, double c = 0.01, double eps = 0.00000001, double toler = 0.00000001);
	~mySVM(){};
	int feedFeature(std::vector<std::vector<double>> &hogs, bool pos);
	int saveSVM(const char *path);
	int loadSVM(const char *path);
	int train(int n = 3, double gamma = 1, double c = 0);
	std::vector<int> predict(std::vector<std::vector<double>>& x);
	void clear();
	bool trained()
	{
		return _trained;
	}
	void show_attributes();

private:
	Eigen::MatrixXd featureMAT, catch_k;
	Eigen::VectorXd labelVec;
	double _c, _eps, _toler; //松弛，偏移，最小变化阈值，alpha变化容忍度
	Eigen::RowVectorXd _w;	 //决策函数参数
	double _b = 0.0;		 //决策函数参数
	Eigen::RowVectorXd _a;	 //拉格朗日系数
	Eigen::RowVectorXd _E;	 //g(x)-y 决策函数和标签差距
	void smo();
	std::set<int> _zero_alpha, _support_vec; //为零的alpha值的ID和支持向量
	int _n;									 //核函数参数
	double _gamma, _C;						 //核函数参数
	double kernel(Eigen::VectorXd x1, Eigen::VectorXd x2)
	{
		return pow(_gamma * x1.dot(x2) + _C, _n);
	}
	bool _trained = false; //是否训练过
	bool _ready = false;
	double _clip_a(int i, int j, double a_j_new);
	void _update_aids_b_E(int i, double a_i_new, int j, double a_j_new);
	void _update_aids(int i, double val);
	int _select_first();

	int _pos = -1;
	int _nag = -1;
};