#include "Convolution.h"
#include <fstream>
#include <sstream>
#include <cstddef>
#include <climits>


XYData::XYData()
{
}

int XYData::GetSize()
{
	return xys.size();
}

void XYData::AddXY(double x, double y, double val, std::unordered_set<std::pair<double, double>, coord_hash, coord_equal>& coords)
{
	std::pair<double, double> p;
	p = std::make_pair(x, y);
	std::unordered_map<std::pair<double, double>, double>::const_iterator it = xys.find(p);
	if (it != xys.end())
		xys[p] = val;
	else
		xys.insert({ p, val });

	std::unordered_set<std::pair<double, double>>::const_iterator itc = coords.find(p);
	if (itc == coords.end())
		coords.insert(p);
}

double XYData::GetValAt(double x, double y)
{
	std::pair<double, double> p;
	p = std::make_pair(x, y);
	std::unordered_map<std::pair<double, double>, double>::const_iterator it = xys.find(p);
	if (it != xys.end())
		return it->second;
	else
		return 0.0;
}

void XYData::ReadDataFromFile(const char* filePath, double* stepSizeX, double* stepSizeY, std::unordered_set<std::pair<double, double>, coord_hash, coord_equal>& coords)
{
	std::ifstream test(filePath);
	if (!test)
		return;

	std::ifstream infile(filePath);
	std::string line;

	int i = -1;
	std::stringstream ss;
	double x1; bool checkX = true;
	double y1; bool checkY = true;
	while (std::getline(infile, line)) {
		i++;
		ss.clear();
		ss.str(line);
		std::string item;
		int ctr = -1;
		double x; double y; double val;
		while (std::getline(ss, item, '\t')) {
			ctr++;
			if (ctr == 0)
				x = std::stod(item);
			else if (ctr == 1)
				y = std::stod(item);
			else if (ctr == 2)
				val = std::stod(item);
		}
		AddXY(x, y, val, coords);
		if (i == 0)
		{
			x1 = x;
			y1 = y;
		}
		else
		{
			if (checkX && x1 != x)
			{
				*stepSizeX = abs(x1 - x);
				checkX = false;
			}
			if (checkY && y1 != y)
			{
				*stepSizeY = abs(y1 - y);
				checkY = false;
			}
		}
	}
}

double Convolution::GetMinX()
{
	double minX = INT_MAX;
	for (std::pair<double, double> val : coords)
		minX = std::min(val.first, minX);
	return minX;
}

double Convolution::GetMaxX()
{
	double maxX = INT_MIN;
	for (std::pair<double, double> val : coords)
		maxX = std::max(val.first, maxX);
	return maxX;
}

double Convolution::GetMinY()
{
	double minY = INT_MAX;
	for (std::pair<double, double> val : coords)
		minY = std::min(val.second, minY);
	return minY;
}

double Convolution::GetMaxY()
{
	double maxY = INT_MIN;
	for (std::pair<double, double> val : coords)
		maxY = std::max(val.second, maxY);
	return maxY;
}

Convolution::Convolution()
{
}

Convolution::~Convolution()
{
	if (data1 != nullptr)
		delete data1;
	if (data2 != nullptr)
		delete data2;
}

bool Convolution::ReadDataFromFiles(const char* filePath1, const char* filePath2)
{
	std::ifstream test1(filePath1);
	if (!test1)
		return false;

	if (data1 != nullptr)
		delete data1;
	data1 = new XYData();
	data1->ReadDataFromFile(filePath1, &stepSizeX, &stepSizeY, coords);

	std::ifstream test2(filePath2);
	if (!test2)
		return false;

	if (data2 != nullptr)
		delete data2;
	data2 = new XYData();
	double dummy1; double dummy2;
	data2->ReadDataFromFile(filePath2, &dummy1, &dummy2, coords);
}

void Convolution::Convolve()
{
	minK1 = GetMinX();
	minK2 = GetMinY();
	maxK1 = GetMaxX();
	maxK2 = GetMaxY();

	// build two 2D grids for each of the XY coords
	int numX = int((maxK1 - minK1) / stepSizeX);
	int numY = int((maxK2 - minK2) / stepSizeY);
	std::vector<std::vector<double>> data1Grid(numX + 1, std::vector<double>(numY + 1));
	std::vector<std::vector<double>> data2Grid(numX + 1, std::vector<double>(numY + 1));
	double dx = (maxK1 - minK1) / (numX + 1.0);
	double dy = (maxK2 - minK2) / (numY + 1.0);
	double k1; double k2;
	for (int i = 0; i <= numX; i++)
	{
		k1 = (i * stepSizeX) + minK1;
		for (int j = 0; j <= numY; j++)
		{
			k2 = (j * stepSizeY) + minK2;
			data1Grid[i][j] = data1->GetValAt(k1, k2);
			data2Grid[i][j] = data2->GetValAt(k1, k2);
		}
	}

	double cVal;
	for (int xi = 0; xi <= numX; xi++)
	{
		double coordFirst = (xi * stepSizeX) + minK1;
		for (int yj = 0; yj <= numY; yj++)
		{
			double coordSecond = (yj * stepSizeY) + minK2;
			cVal = 0;
			for (int i = 0; i <= numX; i++)
			{
				int n1k1 = int((coordFirst - ((i * stepSizeX) + minK1) - minK1) / stepSizeX);
				for (int j = 0; j <= numY; j++)
				{
					int n2k2 = int((coordSecond - ((j * stepSizeY) + minK2) - minK2) / stepSizeY);
					double nk = 0.0;
					if (n1k1 > -1 && n1k1 <= numX && n2k2 > -1 && n2k2 <= numY)
						nk = data2Grid[n1k1][n2k2];
					cVal += (data1Grid[i][j] * nk);
				}
			}
			if (cVal < 0.000001)
				cVal = 0.000001;
			std::vector<double> cData{ 1.0 * xi, 1.0 * yj, cVal * dx * dy };
			cnvltn.push_back(cData);
			if (xi == 0)
				cnvY.push_back(coordSecond);
		}
		cnvX.push_back(coordFirst);
	}
}

void Convolution::WriteConvolutionToFile(const char* filePath, const char* xFilePath, const char* yFilePath)
{
	std::ofstream outFile(filePath);
	for (std::vector<double> cVals : cnvltn)
	{
		outFile << cVals[0];
		for (int i = 1; i < cVals.size(); i++)
			outFile << "\t" << cVals[i];
		outFile << "\n";
	}
	outFile.close();

	std::ofstream xOutFile(xFilePath);
	for (double val : cnvX)
	{
		xOutFile << val << "\n";
	}
	xOutFile.close();

	std::ofstream yOutFile(yFilePath);
	for (double val : cnvY)
	{
		yOutFile << val << "\n";
	}
	yOutFile.close();
}