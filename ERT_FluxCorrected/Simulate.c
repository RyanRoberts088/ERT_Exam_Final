#include "flux_corrected_transport.h"
#include "Simulate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

EXPORT int Simulate()
//int Simulate()
{
	// read file with simulation options
	FILE* fp = fopen("simOptions.dat", "r");
	if (fp == NULL)
	{
		printf("Error opening file\n");
		return -1;
	}
	char line[256];

	int arrLen = 0;
	double dx = 0.;
	double dt = 0.;
	double tFinal = 0.;
	double tOutRate = 0.;
	int maxSteps = 0;

	while (fgets(line, sizeof(line), fp)) {
		char str[100];
		double value;
		sscanf(line, "%s %lf", &str, &value);
		if (strcmp(str, "NumCells") == 0)
		{
			printf("NumCells: %lf\n", value);
			arrLen = (int)value;
		}
		else if (strcmp(str, "dx") == 0)
		{
			printf("dx: %lf\n", value);
			dx = value;
		}
		else if (strcmp(str, "dt") == 0)
		{
			printf("dt: %lf\n", value);
			dt = value;
		}
		else if (strcmp(str, "Tfinal") == 0)
		{
			printf("Tfinal: %lf\n", value);
			tFinal = value;
		}
		else if (strcmp(str, "Toutrate") == 0)
		{
			printf("Toutrate: %lf\n", value);
			tOutRate = value;
		}
		else if (strcmp(str, "MaxSteps") == 0)
		{
			printf("MaxSteps: %lf\n", value);
			if (value >= INT_MAX)
			{
				printf("MaxSteps exceed integer tolerance, resetting to 10000000");
				maxSteps = 10000000;
			}
			else
				maxSteps = (int)value;
		}
	}
	fclose(fp);

	// read file with initial condition
	fp = fopen("simInitial.dat", "r");
	if (fp == NULL)
	{
		printf("Error opening file\n");
		return -1;
	}
	char line2[256];
	// velocity
	double* V = (double*)malloc(arrLen * sizeof(double));
	// density
	double* U = (double*)malloc(arrLen * sizeof(double));
	// output density
	double* U_trnsp = (double*)malloc(arrLen * sizeof(double));
	for (int i = 0; i < arrLen; i++)
		U_trnsp[i] = 0;
	// output velocity
	double* V_trnsp = (double*)malloc(arrLen * sizeof(double));
	for (int i = 0; i < arrLen; i++)
		V_trnsp[i] = 0;

	int numLines = 0;
	printf("\n");
	printf("Initial Values\n");
	while (fgets(line2, sizeof(line2), fp)) {
		numLines++;
		if (numLines > arrLen)
		{
			printf("initial velocity and density profile contains too many points\n");
			return -2;
		}
		double rho; double vel;
		sscanf(line2, "%lf %lf", &rho, &vel);
		V[numLines - 1] = vel;
		U[numLines - 1] = rho;
		printf("%lf %lf\n", U[numLines - 1], V[numLines - 1]);
	}
	printf("\n");
	fclose(fp);

	// simulate 
	// - Runga-Kutta for velocity
	// - then use flux-corrected method
	// - if at dump rate, write density profile
	//   to file with each profile separated
	//   by newline
	double t = 0.;
	double nextDump = tOutRate;
	int numSteps = 0;
	if (dx <= 0. || dt <= 0.)
	{
		printf("zero-valued grid cell or timestep length\n");
		return -3;
	}
	double C = dt / dx;
	double C2 = C * C;

	FILE* outFile = fopen("simResults.dat", "w");
	if (outFile == NULL)
	{
		printf("Error opening file\n");
		return -1;
	}

	while (t < tFinal && numSteps < maxSteps)
	{
		numSteps++;
		t += dt;

		flux_corr_method(U, V, arrLen, dt, dx, U_trnsp);
		//IntegrateGodunov(V, arrLen, C, V_trnsp);
		IntegrateRK4(V, arrLen, C, V_trnsp);
		for (int i = 0; i < arrLen; i++)
		{
			U[i] = U_trnsp[i];
			V[i] = V_trnsp[i];
		}

		if (t > nextDump)
		{
			nextDump += tOutRate;
			for (int i = 0; i < arrLen; i++)
			{
				fprintf(outFile, "%lf %lf\n", U[i], V[i]);
			}
			fprintf(outFile, "\n");
		}
	}
	fclose(outFile);

	return 0;
}

void IntegrateGodunov(double* V, int arrLen, double C, double* V_trnsp)
{
	V_trnsp[0] = V[0] - C * (Flux(V[0], V[1]) - Flux(V[arrLen - 1], V[0]));

	V_trnsp[arrLen - 1] = V[arrLen - 1] - C * (Flux(V[arrLen - 1], V[0]) - Flux(V[arrLen - 2], V[arrLen - 1]));

	for (int i = 1; i < arrLen - 1; i++)
	{
		V_trnsp[i] = V[i] - C * (Flux(V[i], V[i + 1]) - Flux(V[i - 1], V[i]));
	}
}

double Flux(double v, double w)
{
	if ((v < 0.0 && w > 0.0) || (v == w && v == 0.0))
		return 0.0;
	double v2 = 0.5 * v * v;
	double w2 = 0.5 * w * w;

	if (v >= 0.0 && w >= 0.0)
		return v2;
	else if (v <= 0.0 && w <= 0.0)
		return w2;
	else
	{
		if (((w2 - v2) / (w - v)) > 0)
			return v2;
		else
			return w2;
	}
}

void IntegrateRK4(double* V, int arrLen, double C, double* V_trnsp)
{
	double rkn3 = RK4(V[arrLen - 5], V[arrLen - 4], V[arrLen - 3], V[arrLen - 2], V[arrLen - 1]);
	double rkn2 = RK4(V[arrLen - 4], V[arrLen - 3], V[arrLen - 2], V[arrLen - 1], V[0]);
	double rkn1 = RK4(V[arrLen - 3], V[arrLen - 2], V[arrLen - 1], V[0], V[1]);
	double rk0 = RK4(V[arrLen - 2], V[arrLen - 1], V[0], V[1], V[2]);
	double rk1 = RK4(V[arrLen - 1], V[0], V[1], V[2], V[3]);
	double rk2 = RK4(V[0], V[1], V[2], V[3], V[4]);

	V_trnsp[0] = V[0] - C * (rk0 - rkn1);
	V_trnsp[1] = V[1] - C * (rk1 - rk0);
	V_trnsp[2] = V[2] - C * (rk2 - rk1);

	V_trnsp[arrLen - 1] = V[arrLen - 1] - C * (rkn1 - rkn2);
	V_trnsp[arrLen - 2] = V[arrLen - 2] - C * (rkn2 - rkn3);

	double rkip;
	double rkin;
	for (int i = 3; i < arrLen - 2; i++)
	{
		rkip = RK4(V[i - 2], V[i - 1], V[i], V[i + 1], V[i + 2]);
		rkin = RK4(V[i - 3], V[i - 2], V[i - 1], V[i], V[i + 1]);
		V_trnsp[i] = V[i] - C * (rkip - rkin);
	}
}

double RK4(double ujn2, double ujn1, double uj, double ujp1, double ujp2)
{
	double fjn2 = func(ujn2);
	double fjn1 = func(ujn1);
	double fj   = func(uj);
	double fjp1 = func(ujp1);
	double fjp2 = func(ujp2);
	double ep = 1.0e-6;

	double fh1 = (1.0 / 3.0) * fjn2 - (7.0 / 6.0) * fjn1 + (11.0 / 6.0) * fj;
	double fh2 = (-1.0 / 6.0) * fjn1 + (5.0 / 6.0) * fj + (1.0 / 3.0) * fjp1;
	double fh3 = (1.0 / 3.0) * fj + (5.0 / 6.0) * fjp1 + (-1.0 / 6.0) * fjp2;

	double fb11 = fjn2 - 2.0 * fjn1 + fj;
	double fb12 = fb11 - 2.0 * fjn1 + 2.0 * fj;
	double fb22 = fjn1 - fjp1;
	double fb21 = fb22 - 2.0 * fj + 2.0 * fjp1;
	double fb31 = fj - 2.0 * fjp1 + fjp2;
	double fb32 = fb31 + 2.0 * fj - 2.0 * fjp1;

	double b1 = (13.0 / 12.0) * fb11 * fb11 + 0.25 * fb12 * fb12;
	double b2 = (13.0 / 12.0) * fb21 * fb21 + 0.25 * fb22 * fb22;
	double b3 = (13.0 / 12.0) * fb31 * fb31 + 0.25 * fb32 * fb32;

	double wt1 = 0.1 / ((ep + b1) * (ep + b1));
	double wt2 = 0.6 / ((ep + b2) * (ep + b2));
	double wt3 = 0.3 / ((ep + b3) * (ep + b3));
	double sumw = wt1 + wt2 + wt3;

	double w1 = wt1 / sumw;
	double w2 = wt2 / sumw;
	double w3 = wt3 / sumw;

	return w1 * fh1 + w2 * fh2 + w3 * fh3;
}

double func(double x)
{
	return 0.5 * x * x;
}