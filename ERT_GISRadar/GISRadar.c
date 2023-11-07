#include "GISRadar.h"
#include "math.h"
#include <stdio.h>

#define EPSILON 0.0001

double R = 3958.8;
double degToRad = acos(-1) / 180.0;
double radToDeg = 180.0 / acos(-1);

double func(double sinLa1, double Ad, double cosLa1, double cosTheta,
	double sinLa2)
{
	return sinLa1 * cos(Ad) + cosLa1 * sin(Ad) * cosTheta - sinLa2;
}

double derivFunc(double sinLa1, double Ad, double cosLa1, double cosTheta)
{
	return -sinLa1 * sin(Ad) + cosLa1 * cos(Ad) * cosTheta;
}

int GIS2Radar(double* range, double* bearing, double glonInit,
	double glatInit, double glonFinal, double glatFinal)
{
	double thetab = degToRad * glatFinal;
	double thetaa = degToRad * glatInit;
	double deltaL = degToRad * (glonFinal - glonInit);
	(*bearing) = radToDeg * atan2(cos(thetab) * sin(deltaL), 
		cos(thetaa) * sin(thetab) - sin(thetaa) * cos(thetab) * cos(deltaL));
	double sinLa1 = sin(thetaa);
	double cosLa1 = cos(thetaa);
	double cosBearing = cos((*bearing) * degToRad);
	double sinLa2 = sin(thetab);

	double Ad = 1.0;
	double h = func(sinLa1, Ad, cosLa1, cosBearing, sinLa2) /
		derivFunc(sinLa1, Ad, cosLa1, cosBearing);
	int iter = 0;
	int maxIter = 10000;
	while (fabs(h) >= EPSILON && iter < maxIter)
	{
		iter++;
		h = func(sinLa1, Ad, cosLa1, cosBearing, sinLa2) /
			derivFunc(sinLa1, Ad, cosLa1, cosBearing);
		Ad = Ad - h;
	}

	(*range) = Ad * R;
	return iter >= maxIter;
}

int RtoG(double range, double bearing, double glonInit,
	double glatInit, double* glonFinal,	double* glatFinal)
{
	double la1 = glatInit * degToRad;
	double Ad = range / R;
	double theta = bearing * degToRad;
	double lo1 = glonInit * degToRad;
	double glatFinalRad = asin(sin(la1) * cos(Ad) + cos(la1) * sin(Ad) * cos(theta));
	(*glatFinal) = radToDeg * glatFinalRad;
	(*glonFinal) = radToDeg * (lo1 + atan2(sin(theta) * sin(Ad) * cos(la1),
		cos(Ad) - sin(la1) * sin(glatFinalRad)));
	return 0;
}

void ConvertGIS2Radar()
{
	printf("Enter first latitude and direction (1 for south or 0 for north): \n");
	double lat1; int dir1;
	scanf("%lf %d", &lat1, &dir1);
	if (dir1)
		lat1 = -lat1;

	printf("Enter first longitude and direction (1 for west or 0 for east): \n");
	double lon1; int dir2;
	scanf("%lf %d", &lon1, &dir2);
	if (dir2)
		lon1 = -lon1;

	printf("Enter second latitude and direction (1 for south or 0 for north): \n");
	double lat2; int dir3;
	scanf("%lf %d", &lat2, &dir3);
	if (dir3)
		lat2 = -lat2;

	printf("Enter second longitude and direction (1 for west or 0 for east): \n");
	double lon2; int dir4;
	scanf("%lf %d", &lon2, &dir4);
	if (dir4)
		lon2 = -lon2;

	double range; double bearing;
	int res = GIS2Radar(&range, &bearing, lon1, lat1, lon2, lat2);
	if (res)
		printf("Failed solve for range and bearing. Consider reducing solve tolerance.");

	printf("Bearing: %lf degrees\n", bearing);
	printf("Range: %lf miles\n", range);
}

void ConvertRtoG()
{
	printf("Enter first latitude and direction (1 for south or 0 for north): \n");
	double lat1; int dir1;
	scanf("%lf %d", &lat1, &dir1);
	if (dir1)
		lat1 = -lat1;

	printf("Enter first longitude and direction (1 for west or 0 for east): \n");
	double lon1; int dir2;
	scanf("%lf %d", &lon1, &dir2);
	if (dir2)
		lon1 = -lon1;

	printf("Enter range (miles) and bearing (degrees with 0 being north): \n");
	double range; double bearing;
	scanf("%lf %lf", &range, &bearing);

	double lon2; double lat2;
	int res = RtoG(range, bearing, lon1, lat1, &lon2, &lat2);

	printf("Latitude: %lf degrees\n", lat2);
	printf("Longitude: %lf degrees\n", lon2);
}