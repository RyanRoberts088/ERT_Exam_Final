
#ifndef SIMULATE_H
#define SIMULATE_H

#define EXPORT __declspec(dllexport)

EXPORT int Simulate();

/**
 * Integrates 1D velocity field using the first-order Godunov method
 *
 * @param V[in] Velocity values
 * @param arrlen[in] Size of V and V_trnsp arrays
 * @param C[in] Courant-Friedrichs-Levy condition
 * @param V_trnsp[out] Integrated velocity values
 */
void IntegrateGodunov(double* V, int arrLen, double C, double* V_trnsp);

double Flux(double v, double w);

double func(double x);

/**
 * Integrates 1D velocity field using the fourth-order RK method described in
 *
 * The algorithm here is described in:
 *    Seydaoglu, M., Erdogan, U., and Ozis, T. (2015). Numerical solution of Burgers’
 *    equation with high order splitting methods. Journal of Computational and
 *    applied mathematics Vol. 291 410-421
 *
 * @param V[in] Velocity values
 * @param arrlen[in] Size of V and V_trnsp arrays
 * @param C[in] Courant-Friedrichs-Levy condition
 * @param V_trnsp[out] Integrated velocity values
 */
void IntegrateRK4(double* V, int arrLen, double C, double* V_trnsp);

/**
 * Calculates updated velocity value for term j using the fourth-order RK method described in
 *
 * The algorithm here is described in:
 *    Seydaoglu, M., Erdogan, U., and Ozis, T. (2015). Numerical solution of Burgers’ 
 *    equation with high order splitting methods. Journal of Computational and
 *    applied mathematics Vol. 291 410-421
 *
 * @param ujn2 Velocity two grid spaces back (j-2)
 * @param ujn1 Velocity one grid space back (j-1)
 * @param uj   Velocity at current space (j)
 * @param ujp1 Velocity one grid space forward (j+1)
 * @param ujp2 Velocity two grid spaces forward (j+2)
 */
double RK4(double ujn2, double ujn1, double uj, double ujp1, double ujp2);

#endif