**** Description ********************************************
This C code base creates an executable called ERT_FluxCorrected
that integrates the 1-D continuity and momentum equations with 
no forcing terms across a structured grid. The operators are 
split and the continuity and momentum equations integrated
separately. The flux-corrected method is first used to integrate
the density profile. 
 *    Boris, J. P., & Book, D. L. (1976). Solution of continuity equations
 *    by the method of flux-corrected transport. Controlled Fusion, 85-129.
 Next, a Runga-Kutta 4th-order method is used to integrate the velocity
 profile by exploiting continuity to reduce the momentum equation
 to the Inviscid Burgers equation. 
    v_t + v * v_x = 0
The specifics of the Runga-Kutta method can be found in
 *    Seydaoglu, M., Erdogan, U., and Ozis, T. (2015). Numerical solution of Burgers’ 
 *    equation with high order splitting methods. Journal of Computational and
 *    applied mathematics Vol. 291 410-421
 
 To use this method, the user must define two files called simOptions.dat
 and simInitial.dat.
 simOptions.dat and simInitial.dat should be placed within the same 
working directory as the C executable.
 
simOptions should contain the following data in the format specified below.
Comments (text with //) that explain the options should not be included.
NumCells 1000     // number of grid cells
dx 1.0            // grid cell size in meters
dt 0.0001		  // integration time step in seconds
Tfinal 10.0       // final integration time in seconds
Toutrate 0.1      // rate of output in seconds
MaxSteps 10000000 // maximum number of integration steps (maximum value is 10000000)

simInitial should contain the initial density and velocity profile
written to two equal length columns with density as the first column and
velocity as the second 
rho1 vel1
rho2 vel2
...

Once defined, the executable will output a file called simResults.dat,
which contains snapshots of the simulation with the earliest snapshots
at the top of the file. Density and velocity are organized into columns
as with simInitial.dat, and snapshots are separated by a single 
empty line.

To visualize the evolution of the data in simResults, utilize the 
Python script FluxCorrected.py, which reads in the files simInitial.dat
and simResults.dat if both files are placed in the same directory
as the script. The output is an animation called wave.gif.
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
6. Enter the command "make Makefile ERT_FluxCorrected.exe"
*************************************************************