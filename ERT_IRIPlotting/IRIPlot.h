#pragma once
extern void read_ig_rz_();
extern void readapf107_();
extern void iri_web_(int* jmag, int jf[], float* xlat, float* xlon,
	int* iyyyy, int* mmdd, int* iut, float* hour, float* height,
	float* h_tec_max, int* ivar, float* vbeg, float* vend,
	float* vstp, float outf[], float oar[]);

#ifndef IRIPlot_H
#define IRIPlot_H

void RunIRI();

#endif