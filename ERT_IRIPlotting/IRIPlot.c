#include "IRIPlot.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>

float** Alloc_2d_float(int rows, int cols)
{
	float* data = (float*)malloc(rows * cols * sizeof(float));
	float** array = (float**)malloc(rows * sizeof(float*));
	for (int i = 0; i < rows; i++)
		array[i] = &(data[cols * i]);
	return array;
}

void RunIRI()
{
	read_ig_rz_();
	readapf107_();

	int mOutf = 20;
	int n = 1000;
	int mOar = 100;

	// variable declarations
	float** outf = Alloc_2d_float(n, mOutf);
	float** oar = Alloc_2d_float(n, mOar);

	bool jfi[6];
	int jf[50];
	char pname[7][9];
	int nummax = n;
	
	// transposed because FORTRAN stores column-first in memory
	for (int i = 0; i < mOar; i++)
		oar[0][i] = -1.0;

	printf("jmag=(=0/1,geog/geom),lati/deg,long/deg\n");
	int jm; float xlat; float xlon;
	scanf("%d %f %f", &jm, &xlat, &xlon);

	printf("year(yyyy),mmdd(or -ddd),iut(=0/1,LT/UT),hour\n");
	int iy; int imd; int iut; float hour;
	scanf("%d %d %d %f", &iy, &imd, &iut, &hour);

	printf("height/km\n");
	float hx;
	scanf("%f", &hx);

	printf("variable? (1/2/../8 for height/lat/long/year/month/day/day of year/hour)\n");
	int ivar;
	scanf("%d", &ivar);

	printf("begin, end, and stepsize for the selected variable\n");
	float vbeg; float vend; float vstp;
	scanf("%f %f %f", &vbeg, &vend, &vstp);

	printf("upper height [km] for TEC integration (0 for no TEC)\n");
	float htec_mas;
	scanf("%f", &htec_mas);

	printf("Options: 1(true) or 0(false)\n");
	printf("Enter 0 to use standard or 1 to enter your own\n");
	int jchoice;
	scanf("%d", &jchoice);

	for (int i = 0; i < 50; i++)
		jf[i] = true;
	if (!jchoice)
	{
		jf[3] = false;
		jf[4] = false;
		jf[5] = false;
		jf[20] = false;
		jf[22] = false;
		jf[27] = false;
		jf[28] = false;
		jf[29] = false;
		jf[32] = false;
		jf[34] = false;
		jf[38] = false;
		jf[39] = false;
		jf[46] = false;
	}
	else
	{
		jf[0] = 1;
		printf("Ne lower boundary: 1=65/80km day/night, 0=user input {1}\n");
		scanf("%d", &(jf[44]));
		printf("Ne upper boundary: 1=2000km day/night, 0=user input {1}\n");
		scanf("%d", &(jf[45]));
		printf("LAY version: 1=standard ver., 0=LAY version. {standard:1}\n");
		scanf("%d", &(jf[10]));
		printf("Ne Topside: jf(29),jf(30)                  {0,0}\n");
		printf("1,1) IRI-2001, (1,0) COR2, (0,1)=IRIcor, 0,0=NeQuick  \n");
		scanf("%d %d", &(jf[28]), &(jf[29]));
		printf("Ne Topside: 1=F10.7<188, 0=unlimited {1}\n");
		scanf("%d", &(jf[6]));
		printf("F2 peak density or foF2: 1=model, 0=user input {1}\n");
		scanf("%d", &(jf[7]));
		if (jf[7])
		{
			printf("foF2 model: 1=CCIR, 0=URSI-88 {0}\n");
			scanf("%d", &(jf[4]));
			printf("foF2: 1=with storm model, 0=without {1}\n");
			scanf("%d", &(jf[25]));
			printf("Ne Topside: 1=w/o foF2 storm model, 0=with {1}\n");
			scanf("%d", &(jf[36]));
		}
		printf("F2 peak height or M3000F2: 1=model, 0=user input {1}\n");
		scanf("%d", &(jf[8]));
		if (jf[8])
		{
			printf("hmF2: 1=f(M3000F2), 0=new models {0}\n");
			scanf("%d", &(jf[38]));
			printf("hmF2: 1=AMTB-model, 0=Shubin-COSMIC model {1}\n");
			scanf("%d", &(jf[39]));
			if (jf[38])
			{
				printf("hmF2: 1=w/o foF2 storm model, 0=with {1}\n");
				scanf("%d", &(jf[35]));
			}
		}
		printf("B0 bottomside thickness: 1=model, 0=user input {1}\n");
		scanf("%d", &(jf[42]));
		printf("B1 bottomside shape: 1=model, 0=user input {1}\n");
		scanf("%d", &(jf[43]));
		if (jf[42])
		{
			printf("Bottomside thickness B0: 1=Bil-2000, 0=other options {0}\n");
			scanf("%d", &(jf[3]));
			printf("Bottomside thickness B0: 1=ABT-2009, 0= Gul-1987 {1}\n");
			scanf("%d", &(jf[30]));
		}
		printf("F1 peak density or foF1: 1=model, 0=user input {1}\n");
		scanf("%d", &(jf[12]));
		if (!jf[10])
		{
			printf("F1 peak height: 1=model, 0=user input {1}\n");
			scanf("%d", &(jf[13]));
		}
		printf("F1: jf(19),jf(20)                        {1,1}\n");
		printf("(1,1) stdard f1 prob, (1,0) f1-prob with L-cond\n");
		printf("(0,1) only if foF1>0 and not NIGHT, (0,0) no F1\n");
		scanf("%d %d", &(jf[18]), &(jf[19]));
		printf("E peak density or foE: 1=model, 0=user input {1}\n");
		scanf("%d", &(jf[14]));
		printf("E peak height: 1=model, 0=user input {1}\n");
		scanf("%d", &(jf[15]));
		printf("E peak auroral storm model: 1=on, 0=off {0}\n");
		scanf("%d", &(jf[34]));
		printf("D: 1=IRI-1990, 0= FT-2001 {1}\n");
		scanf("%d", &(jf[23]));

		printf("Equat. Vert. Ion Drift: 1=computed, 0=not computed {1}\n");
		scanf("%d", &(jf[20]));
		printf("Spread-F probability: 1=computed, 0=not computed {1}\n");
		scanf("%d", &(jf[27]));
		printf("Auroral boundary model: 1=on, 0=off {0}\n");
		scanf("%d", &(jf[32]));
		printf("COV: 1: COV=F10.7_365, 0: COV=func(IG12).  {1}\n");
		scanf("%d", &(jf[40]));
		printf("Sunspot index: 1=from file, 0=user input.  {1}\n");
		scanf("%d", &(jf[16]));
		printf("Ionospheric index: 1=from file, 0=user input. {1}\n");
		scanf("%d", &(jf[26]));
		printf("F10.7D Index: 1=from file, 0=user input {1}\n");
		scanf("%d", &(jf[24]));
		printf("F10.7_81 Index: 1=from file, 0=user input {1}\n");
		scanf("%d", &(jf[31]));
		printf("dip, magbr, modip: 1=IGRF, 0=old FIELDG using POGO68/10 for 1973 {1}\n");
		scanf("%d", &(jf[17]));
		printf("CGM computation: 1=on, 0=off {0}\n");
		scanf("%d", &(jf[46]));
		printf("Messages on (1) off (0) {1}\n");
		scanf("%d", &(jf[33]));
		printf("Message output unit: 1=(UNIT=6), 0=(UNIT=11). {1}\n");
		scanf("%d", &(jf[11]));
	}

	printf(" ");
	printf(" ");
	printf(" ");
	int numstp = 1;
	if (jf[0] && (!jf[7] || !jf[8] || !jf[12] || !jf[13] || !jf[14] || !jf[15] || !jf[42] || !jf[43]))
	{
		float var = vbeg;
		int i = 0;
		if (!jf[7])
		{
			jf[25] = false;
			printf("foF2/Mhz or NmF2/m-3 for   H  =%d\n", &var);
			scanf("%f", &(oar[0][0]));
			sprintf(pname[0], "foF2/MHz");
			if (oar[0][0] > 30)
				sprintf(pname[0], "NmF2/m-3");
		}
		if (!jf[9])
		{
			printf("hmF2/km or M3000F2 for   H  =%d\n", &var);
			scanf("%f", &(oar[0][1]));
			sprintf(pname[1], "M(3000)F2");
			if (oar[0][1] > 50)
				sprintf(pname[1], "hmF2/km");
		}
		if (!jf[12])
		{
			printf("foF1/MHz or NmF1/m-3 for   H  =%d\n", &var);
			scanf("%f", &(oar[0][2]));
			sprintf(pname[2], "foF1/MHz");
			if (oar[0][2] > 30)
				sprintf(pname[2], "NmF1/m-3");
		}
		if (!jf[13])
		{
			printf("hmF1/km for   H  =%d\n", &var);
			scanf("%f", &(oar[0][3]));
			sprintf(pname[3], "hmF1/km");
		}
		if (!jf[14])
		{
			printf("foE/MHz or NmE/m-3 for   H  =%d\n", &var);
			scanf("%f", &(oar[0][4]));
			sprintf(pname[4], "foE/MHz");
			if (oar[0][4] > 30)
				sprintf(pname[4], "NmE/m-3");
		}
		if (!jf[15])
		{
			printf("hmE/km for   H  =%d\n", &var);
			scanf("%f", &(oar[0][5]));
			sprintf(pname[5], "hmE/km");
		}
		if (!jf[42])
		{
			printf("B0/km for   H  =%d\n", &var);
			scanf("%f", &(oar[0][9]));
			sprintf(pname[6], "B0/km");
		}
		if (!jf[43])
		{
			printf("B1/km for   H  =%d\n", &var);
			scanf("%f", &(oar[0][34]));
			sprintf(pname[6], "B1/km");
		}
	}

	if (!jf[24])
	{
		printf("User input for F10.7D\n");
		scanf("%f", &(oar[0][40]));
		for (int i = 1; i < 100; i++)
			oar[i][40] = oar[0][40];
	}
	if (!jf[31])
	{
		printf("User input for F10.7_81avg\n");
		scanf("%f", &(oar[0][45]));
		for (int i = 1; i < 100; i++)
			oar[i][45] = oar[0][45];
	}

	if (!jf[16])
	{
		printf("User input for Rz12\n");
		scanf("%f", &(oar[0][32]));
		for (int i = 1; i < 100; i++)
			oar[i][32] = oar[0][32];
	}
	if (!jf[26])
	{
		printf("User input for IG12\n");
		scanf("%f", &(oar[0][38]));
		for (int i = 1; i < 100; i++)
			oar[i][38] = oar[0][38];
	}

	if (!jf[44])
	{
		printf("User input for HNEA/km\n");
		scanf("%f", &(oar[0][88]));
		for (int i = 1; i < 100; i++)
			oar[i][88] = oar[0][88];
	}
	if (!jf[45])
	{
		printf("User input for HNEE/km\n");
		scanf("%f", &(oar[0][89]));
		for (int i = 1; i < 100; i++)
			oar[i][89] = oar[0][89];
	}

	int num1 = (vend - vbeg) / vstp + 1;
	numstp = abs(num1);
	if (numstp > nummax)
		numstp = nummax;

	float hxx = hx;
	int jmag = jm;
	int mmdd = imd;
	iri_web_(&jmag, jf, &xlat, &xlon, &iy, &mmdd, &iut, &hour, &hxx, 
		&htec_mas, &ivar, &vbeg, &vend, &vstp, outf[0], oar[0]);

	float xcor = vbeg;
	FILE* fptr;
	fptr = fopen("EDP.dat", "w");
	for (int i = 0; i < numstp; i++)
	{
		fprintf(fptr, "%f %f\n", xcor, sqrt(outf[i][0] / 1.24e10));
		xcor += vstp;
	}
	fclose(fptr);

	bool hasGNUPlot = false;
	FILE* gnuFile;
	if (gnuFile = fopen("plot.gnu", "r"))
	{
		fclose(gnuFile);
		hasGNUPlot = true;
	}

	if (hasGNUPlot)
		system("gnuplot plot.gnu");


	// CLEAN UP
	for (int i = 0; i < n; i++)
	{
		free(outf[i]);
		free(oar[i]);
	}
	free(outf);
	free(oar);
}