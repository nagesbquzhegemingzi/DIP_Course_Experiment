#include "mySVM.h"

mySVM::mySVM(unsigned sample_num, unsigned feature_size, double c, double eps, double toler)
{
	labelVec.resize(sample_num);
	featureMAT.resize(feature_size, sample_num);
	_c = c;
	_eps = eps;
	_toler = toler;
	_w.resize(feature_size);
	_w.fill(0);
	_a.resize(sample_num);
	_a.fill(0);
}

int mySVM::feedFeature(std::vector<std::vector<double>> &hogs, bool pos)
{
	//if (hogs.size() != featureMAT.cols()) {
	//	std::cerr << "HOG size error!" << std::endl;
	//	return -1;
	//}
	int start;
	switch (pos)
	{
	case true:
		if (_nag == -1)
		{
			start = 0;
			_pos = 0;
		}
		else
		{
			start = _nag;
			_pos = 0;
		}
		break;
	case false:
		if (_pos == -1)
		{
			start = 0;
			_nag = 0;
		}
		else
		{
			start = _pos;
			_nag = 0;
		}
		break;
	default:
		break;
	}
	for (size_t i = start; i < hogs.size() + start; i++)
	{
		for (size_t j = 0; j < featureMAT.rows(); j++)
		{
			featureMAT(j, i) = hogs[i - start][j];
			labelVec(i) = pos ? 1 : -1;
		}
		pos ? _pos++ : _nag++;
	}
	if (_pos != -1 && _nag != -1)
		_ready = true;

	//if (_ready) {
	//	for (size_t i = 0; i < labelVec.size(); i++) {
	//		std::cout << labelVec(i) << ' ';
	//	}
	//	std::cout << std::endl;
	//	for (size_t i = 0; i < featureMAT.rows(); i++) {
	//		for (size_t j = 0; j < featureMAT.cols(); j++)
	//			std::cout << featureMAT(i, j) << ' ';
	//		std::cout << std::endl;
	//	}
	//}

	return 0;
}

int mySVM::train(int n, double gamma, double C)
{
	if (!_ready)
	{
		std::cerr << "Error! Features not ready." << std::endl;
		return -1;
	}
	_n = n;
	_gamma = gamma;
	_C = C;
	const unsigned sample_num = labelVec.size();
	const unsigned feature_size = featureMAT.rows();
	catch_k.resize(sample_num, sample_num);
	for (size_t i = 0; i < sample_num; i++)
	{
		for (size_t j = 0; j < sample_num; j++)
		{
			catch_k(i, j) = kernel(featureMAT.col(i), featureMAT.col(j));
		}
	}
	_E.resize(sample_num);
	for (size_t i = 0; i < sample_num; i++)
	{ //求∑αi yi K(xi, x) + bias - y
		_E(i) = ((_a.transpose().array() * labelVec.array()) * catch_k.col(i).array()).sum() + _b - labelVec(i);
		//std::cout << _E(i) << ' ';
	}
	//std::cout << std::endl;
	for (int i = 0; i < sample_num; i++)
	{
		_zero_alpha.insert(i);
	}

	smo();
	for (auto it = _support_vec.begin(); it != _support_vec.end(); it++)
	{
		std::cout << _a(*it) << ' ';
		_w += _a(*it) * labelVec(*it) * featureMAT.col(*it).transpose();
	}
	std::cout << std::endl;
	_w = _w * _gamma;

	_trained = true;
	return 0;
}

void mySVM::smo()
{
	while (true)
	{
		int i = _select_first();
		if (i < 0)
		{
			// no _a(i) violates kkt conditions, optimizing stops
			break;
		}
		int j;
		double E_i = _E(i), max_distance = 0;
		int res = -1;
		for (int jj = 0; jj < _E.size(); jj++)
		{
			if (abs(_E(jj) - E_i) > max_distance)
			{
				max_distance = abs(_E(jj) - E_i);
				res = jj;
			}
		}
		j = res;
		if (j < 0)
			break;

		double eta = catch_k(i, i) + catch_k(j, j) - 2 * catch_k(i, j);
		if (eta == 0)
			continue;
		double a_j_new = _a(j) + labelVec(j) * (_E(i) - _E(j)) / eta;
		a_j_new = _clip_a(i, j, a_j_new);
		// y_i*a_i + y_j*a_j = gama_const
		double a_i_new = _a(i) - (a_j_new - _a(j)) * labelVec(j) / labelVec(i);
		double diff = abs(a_i_new - _a(i)) + abs(a_j_new - _a(j));
		_update_aids_b_E(i, a_i_new, j, a_j_new);

		/*cout << "_E is: \n";
		_E.print();
		cout << "_a is: \n";
		_a.print();*/

		_a(i) = a_i_new;
		_a(j) = a_j_new;

		if (diff <= _eps)
			break;
	}
}

double mySVM::_clip_a(int i, int j, double a_j_new)
{
	double L = 0.0, H = 0.0;
	if (labelVec(i) * labelVec(j) < 0)
	{
		L = std::max(0.0, _a(j) - _a(i));
		H = std::min(_c, _c + _a(j) - _a(i));
	}
	else
	{
		L = std::max(0.0, _a(j) + _a(i) - _c);
		H = std::min(_c, _a(j) + _c);
	}
	a_j_new = std::min(H, a_j_new);
	a_j_new = std::max(L, a_j_new);
	return a_j_new;
}

void mySVM::_update_aids_b_E(int i, double a_i_new, int j, double a_j_new)
{
	// update a_ids sets for heuristic select in _select_first()
	_update_aids(i, a_i_new);
	_update_aids(j, a_j_new);

	// update _b
	double b1 = _b - _E(i) - labelVec(i) * catch_k(i, i) * (a_i_new - _a(i)) -
				labelVec(j) * catch_k(j, i) * (a_j_new - _a(j));
	double b2 = _b - _E(j) - labelVec(i) * catch_k(i, j) * (a_i_new - _a(i)) -
				labelVec(j) * catch_k(j, j) * (a_j_new - _a(j));
	double b_delta = (b1 + b2) / 2 - _b;
	_b += b_delta;

	// update _E
	Eigen::RowVectorXd tmpvec(_E.size());
	tmpvec.fill(b_delta);
	_E = _E + (a_i_new - _a(i)) * labelVec(i) * catch_k.row(i) + (a_j_new - _a(j)) * labelVec(j) * catch_k.row(j) + tmpvec;
}

void mySVM::_update_aids(int i, double val)
{
	if (_zero_alpha.find(i) != _zero_alpha.end() && val > 0 + _toler)
	{
		_zero_alpha.erase(i);
		_support_vec.insert(i);
	}
	else if (_support_vec.find(i) != _support_vec.end() && val <= 0 + _toler)
	{
		_support_vec.erase(i);
		_zero_alpha.insert(i);
	}
}

int mySVM::_select_first()
{
	// select first alpha a(i) from where a_i > 0
	for (auto it = _support_vec.begin(); it != _support_vec.end(); it++)
	{
		int i = *it;
		if (labelVec(i) * _E(i) < 0 && _a(i) >= _c + _toler || labelVec(i) * _E(i) != 0 && _a(i) < _c + _toler)
			return i;
	}
	// select from a_i == 0
	for (auto it = _zero_alpha.begin(); it != _zero_alpha.end(); it++)
	{
		int i = *it;
		if (labelVec(i) * _E(i) <= 0)
			return *it;
	}
	// no alpha, to stop opotimization
	return -1;
}

//Eigen::VectorXd mySVM::predict(Eigen::MatrixXd x) {
//	if (x.rows() != _w.size()) {
//		std::cerr << "[Error] Input wrong test sample feature dimensions";
//		throw - 1;
//	}
//	Eigen::VectorXd res(x.cols());
//	for (size_t i = 0; i < x.cols(); i++) {
//		//cached the _w
//		res(i) = _w.dot(x.col(i)) + _b > 0 ? 1 : -1;
//	}
//	return res;
//}

std::vector<int> mySVM::predict(std::vector<std::vector<double>> &x)
{
	std::vector<int> res;
	Eigen::VectorXd tmp(_w.size());
	double avg = 0;
	for (auto i : x)
	{
		for (size_t j = 0; j < _w.size(); j++)
		{
			tmp(j) = i[j];
		}
		avg += (_w.transpose().dot(tmp) + _b) / x.size();
	}
	for (auto i : x)
	{
		for (size_t j = 0; j < _w.size(); j++)
		{
			tmp(j) = i[j];
		}
		std::cout << ((_w.transpose().dot(tmp) + _b) - avg) << std::endl;
		if (((_w.transpose().dot(tmp) + _b) - avg) > 0)
		{
			res.push_back(1);
		}
		else
		{
			res.push_back(-1);
		}
	}

	return res;
}

void mySVM::clear()
{
	if (!_ready)
		return;
	featureMAT = Eigen::MatrixXd();
	labelVec = Eigen::MatrixXd();
	_c = 1.0;
	_b = 0.0;
	_toler = 0.0001;
	_b = 0.0;
	_zero_alpha.clear();
	_support_vec.clear();
	_trained = false;
	_ready = true;
}

int mySVM::saveSVM(const char *p)
{
	std::ofstream outFile(p, std::ios::out | std::ios::binary);
	if (!outFile.is_open())
	{
		std::cerr << "Error writing file!" << std::endl;
		return -1;
	}
	int w = _w.size();
	outFile.write((const char *)&w, sizeof(int));
	outFile.write((const char *)&_b, sizeof(double));
	for (size_t i = 0; i < w; i++)
	{
		double tmp = _w(i);
		outFile.write((const char *)&tmp, sizeof(double));
	}
	outFile.close();
	return 0;
}

int mySVM::loadSVM(const char *path)
{
	clear();
	std::ifstream File(path, std::ios::out | std::ios::binary);
	if (!File.is_open())
	{
		std::cerr << "Error writing file!" << std::endl;
		return -1;
	}
	int w;
	File.read((char *)&w, sizeof(int));
	File.read((char *)&_b, sizeof(double));
	_w.resize(w);
	double tmp;
	for (size_t i = 0; i < w; i++)
	{
		File.read((char *)&tmp, sizeof(double));
		_w(i) = tmp;
	}
}

void mySVM::show_attributes()
{
	std::cout << "b = " << _b << std::endl;
	std::cout << "w: ";
	for (size_t i = 0; i < _w.size(); i++)
	{
		std::cout << "   " << _w(i) << std::endl;
	}
}