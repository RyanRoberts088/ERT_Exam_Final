**** Description ********************************************
This C code base creates ERT_CcodeOptimization.exe, an executable
that compares the runtimes of optimized and unoptimized versions
of the algorithm found in test_j.c. Runtimes for each of the versions
are given in seconds. The executable may be run from the command line.

General approach to reducing computational time
1. Store variables that are not dependent upon the looped variables 
   as global variables
2. Rearrange loop variables such that variables dependent upon only 
   a subset of (f, fp, and fptilde) are not needlessly recalculated
   in the innermost loop.
3. Reduce variable range within each loop if variable value is not
   used
4. Reduce the number of multiplications and power evaluations
5. Do not evaluate innermost calculations if variable values would
   result in undefined outputs
6. Accelerate exponential evaluations utilizing finite approximations of
   the limit form of e^x 
   https://galileo.phys.virginia.edu/classes/152.mf1i.spring02/Exponential_Function.htm
7. Convert final power evaluations and multiplications into log form, 
   yielding multiplications and additions of intermediately calculated
   functions.
8. Explicitly calculate finite powers as multiplications. i.e. (x^4 as x*x*x*x)

Code was profiled in Visual Studio 2022 community edition using the 
profiler tool quantifying CPU usage. 

Some details of the optimizations
1. The main loops were rearranged to the following order 
   fptilde -> f -> fp to take advantage of variable calculations
   that only required fptilde or f.
2. The variable fpt is being set to a min value (fptildemin) that is greater than 
   the smallest value of the fptilde loop, so the minimum value of this
   loop was reset to fptildemin and then incremented according to 
   the original loop scheme.
3. Calculations are converted to log form until evaluation of S, for which
   the final value is calculated by exponential.
4. x^-4 and y^2 terms in the innermost calculations were split into
   multiplications.
5. The finite approximate exponential utilized 12 terms, but this finite
   value was limited to values -20 < x < 10. Empirically, this limitation
   was found to not affect calculated values while balancing speedup. 
   Return values for inputs x < -20 were 0.0 and the standard library 
   exponential was used for x > 10. 
   
   
Achievements
Machine specs - AMD Ryzen 7 5800H with Radeon Graphics 3.20 GHz 64 bit with 13.9 GB usable RAM
Old algorithm performace - 178.859 seconds
New algorithm performace - 1.218 seconds
Insights
1. Unnecessary exponentiation can drastically slow computation as determined by
   profiling within Visual Studio
2. Conversion to a convenient mathematical space (log in this case) may allow for
   drastic speedup
*************************************************************


**** Compilation instructions *******************************
1. Install the GNU FORTRAN compiler and GNU GCC compiler
  - version 11.4.0 for both compilers
2. Install GNU Make for Makefile compilation (version 4.4.1)
3. For windows, recommend installing Cygwin64 (version 3.4.9-1.x86_64)
  - If using Cygwin64, the compilers and Make can be installed through the 
    Cygwin64 installer executable
4. Place all files within the same working directory
5. Navigate to the working folder by the command line
6. Enter the command "make Makefile ERT_CcodeOptimization.exe"
*************************************************************