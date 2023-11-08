**** Description ********************************************
This C code base creates GISRadar.dll, a dynamically loaded
library exposing four methods. The two immediately given 
below convert two GIS points in standard to bearing and range 
(GIS2Radar) and from one GIS point and bearing and range to a 
final GIS point (RtoG).

int GIS2Radar(double *range, 
	          double *bearing, 
              double glonInit, 
              double glatInit, 
              double glonFinal, 
              double glatFinal)

int RtoG (double range, 
          double bearing, 
          double  glonInit, 
          double glatInit, 
          double *glonFinal, 
          double *glatFinal)
		  
Additionally, there are two methods providing command line
dialog to enter either the necessary data to use either 
GIS2Radar (ConvertGIS2Radar) or (ConvertRtoG). 

In addition to the library GISRadar.dll, an example executable
is generated (ERT_GISRadar.exe) that first calls ConvertGIS2Radar
and then ConvertRtoG as a demonstration of the utilities.
The executable may be run from the command line.
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
6. Enter the command "make Makefile GISRadar.dll ERT_GISRadar.exe"
*************************************************************