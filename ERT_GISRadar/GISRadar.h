
#ifndef GISRADAR_H
#define GISRADAR_H
#define EXPORT __declspec(dllexport)

int GIS2Radar(double* range,
	double* bearing,
	double glonInit,
	double glatInit,
	double glonFinal,
	double glatFinal);

int RtoG(double range,
	double bearing,
	double glonInit,
	double glatInit,
	double* glonFinal,
	double* glatFinal);

EXPORT void ConvertGIS2Radar();

EXPORT void ConvertRtoG();

#endif