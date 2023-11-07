
#ifndef GISRADAR_H
#define GISRADAR_H

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

void ConvertGIS2Radar();

void ConvertRtoG();

#endif