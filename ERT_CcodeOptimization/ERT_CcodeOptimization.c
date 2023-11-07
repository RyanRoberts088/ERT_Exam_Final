#include <stdio.h>
#include <math.h>
#include <time.h>
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

// global variables
double pi = 0;
double fptildemin = 0;
double aX = 0;
double gX = 0;
double fpt = 0;
double sigma_a = 0;
double sigma_b = 0;
double prefactor = 0;
double logAlphaPrefactor = 0;
double logGamma = 0;
double logfVar = 0;
double fFactor = 0;

double approxExp(double x)
{
    if (x < -20)
        return 0.0;
    else if (x > 10.0)
        return exp(x);
    double val = x;
    val = 1.0 + val / 4096;
    val *= val; val *= val; val *= val; val *= val;
    val *= val; val *= val; val *= val; val *= val;
    val *= val; val *= val; val *= val; val *= val;
    return val;
}

void function_j_tilde(double fptilde)
{
    fpt = MAX(fptilde, fptildemin);
    logAlphaPrefactor = log(0.0317 * pow(fpt, aX)) + log(prefactor);
    logGamma = log(5.87 * pow(fpt, gX));
    sigma_a = 0.0547 * pow(fpt, 0.32);
    sigma_b = 0.0783 * pow(fpt, 0.16);
}

void function_j_f(double f)
{
    if (f < 0)
    {
        logfVar = log(-f) * -5.0 + logAlphaPrefactor;
        fFactor = -1;
    }
    else
    {
        logfVar = log(f) * -5.0 + logAlphaPrefactor;
        fFactor = 1;
    }
}

double function_j(double f, double fp)
{
    int eval = f > fp;
    double ratio = fp / f;
    double ratio2 = ratio * ratio;
    double logS = -1.25 * ratio2 * ratio2 + logfVar;
    double sigma = (!eval) * sigma_a + eval * sigma_b;
    double sigRatio = (f - fp) / (sigma * fp);
    logS += approxExp(-0.5 * sigRatio * sigRatio) * logGamma;
    return logS > -8 ? fFactor * exp(logS) : 0.0;
}

double function_j_old(double f, double fp, double fptilde) {

    double a = 0.0081;
    double b = 0.6;
    double g = 9.807;
    double pi = 4. * atan(1.);

    double fptildemin = (1.0 / 2.0 / pi) * pow((4.0 * b / 5.0), (1.0 / 4.0));

    double gC = 5.87;
    double aC = 0.0317;

    double aX = (log(a) - log(aC)) / log(fptildemin);
    double gX = -log(gC) / log(fptildemin);

    double saC = 0.0547;
    double saX = 0.32;

    double sbC = 0.0783;
    double sbX = 0.16;

    double fpt = MAX(fptilde, fptildemin);

    double alpha = aC * pow(fpt, aX);
    double gamma = gC * pow(fpt, gX);
    double sigma_a = saC * pow(fpt, saX);
    double sigma_b = sbC * pow(fpt, sbX);
    double exp1arg = -1.25 * pow((f / fp), -4);
    double sigma = (f <= fp) * sigma_a + (f > fp) * sigma_b;

    double exp2arg = -0.5 * pow((f - fp) / (sigma * fp), 2);

    double S = alpha * pow(g, 2) * pow((2 * pi), -4) * pow(f, -5) * exp(exp1arg) * pow(gamma, exp(exp2arg));

    return S;
}

int main()
{
    clock_t begin = clock();
    // initialization
    double a = 0.0081;
    double b = 0.6;
    double g = 9.807;
    pi = 4. * atan(1.);

    fptildemin = (1.0 / 2.0 / pi) * pow((4.0 * b / 5.0), (1.0 / 4.0));

    double gC = 5.87;
    double aC = 0.0317;

    aX = (log(a) - log(aC)) / log(fptildemin);
    gX = -log(gC) / log(fptildemin);

    double saC = 0.0547;
    double saX = 0.32;

    double sbC = 0.0783;
    double sbX = 0.16;
    prefactor = pow(g, 2) * pow((2 * pi), -4);

    // local variables for all loops
    double S, f, fp, fptilde;
    double firstFptilde = floor(fptildemin * 100.0) / 100.0;
    for (fptilde = firstFptilde; fptilde <= 1.; fptilde += 0.01) {

        function_j_tilde(fptilde);
        for (f = -5.; f <= 5.; f += 0.01) {
            if (f == 0.0)
                continue;

            function_j_f(f);
            for (fp = 0.01; fp <= 10.; fp += 0.01) {

                S = function_j(f, fp);
            }
        }
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent in updated algorithm: %lf\n", time_spent);

    begin = clock();
    for (f = -5.; f <= 5.; f += 0.01) {
        for (fp = 0.; fp <= 10.; fp += 0.01) {
            for (fptilde = 0.; fptilde <= 10.; fptilde += 0.01) {
                S = function_j_old(f, fp, fptilde);
            }
        }
    }
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent in old algorithm: %lf\n", time_spent);
}