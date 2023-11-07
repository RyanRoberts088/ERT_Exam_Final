#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstddef>
#include <cmath>

struct coord_hash {
	auto operator()(const std::pair<double, double> &c) const -> size_t{
		return std::hash<double>()(round(c.first * 100000.0)) ^ 
			std::hash<double>()(round(c.second * 100000.0));
	}
};

struct coord_equal {
	bool operator()(const std::pair<double, double> &c1, const std::pair<double, double> &c2) const
	{
		double c11 = round(c1.first * 100000.0);
		double c12 = round(c1.second * 100000.0);
		double c21 = round(c2.first * 100000.0);
		double c22 = round(c2.second * 100000.0);
		return c11 == c21 && c12 == c22;
	}
};

class XYData
{
	public:
		XYData();
		void AddXY(double x, double y, double value, std::unordered_set<std::pair<double, double>, coord_hash, coord_equal>& coords);
		int GetSize();
		double GetValAt(double x, double y);
		void ReadDataFromFile(const char* filePath, double* stepSizeX, double* stepSizeY, std::unordered_set<std::pair<double, double>, coord_hash, coord_equal>& coords);
	private:
		std::unordered_map<std::pair<double, double>, double, coord_hash, coord_equal> xys;
};

class Convolution
{
	public:
		Convolution();
		bool ReadDataFromFiles(const char* filePath1, const char* filePath2);
		void Convolve();
		void WriteConvolutionToFile(const char* filePath, const char* xFilePath, const char* yFilePath);
		~Convolution();
		double GetMinX();
		double GetMaxX();
		double GetMinY();
		double GetMaxY();
	private:
		XYData *data1;
		XYData *data2;
		std::vector<std::vector<double>> cnvltn;
		std::unordered_set<std::pair<double, double>, coord_hash, coord_equal> coords;
		std::vector<double> cnvX;
		std::vector<double> cnvY;
		double minK1;
		double maxK1;
		double minK2;
		double maxK2;
		double stepSizeX;
		double stepSizeY;
};