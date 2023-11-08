**** Description ********************************************
This C code base creates an executable that will generate
the electron density profile (EDP) in Megahertz (MHz) 
versus vertical height in kilometers from the Earth's 
surface. Calculation of the profile is performed using
the IRI 2016 model http://irimodel.org/. IRI data, options,
and instrutions can be found at the link above. Solar and
Magnetic indices found in the repository can be substituted
for the indices at the link. All rights go to IRI.

The dialog for user entry of the calculation
options is almost word-for-word the same as that found in
the file iritest.for. However, unlike the outputs found in
iritest.for, the output will always be an EDP profile vs. 
height found in the file EDP.dat, with the first column 
being height and second EDP. EDP is calculated as plasma 
frequency, which is converted from raw electron density 
(num electrons / m-3) through the formula found in 
https://doi.org/10.1016/S0964-2749(02)80180-9.

EDP can be visualized through GNU plot using an optional 
file plot.gnu, which is run if present in the working
directory. An example plotting file for editing is included.
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
6. Enter the command "make Makefile ERT_IRIPlotting.exe"
*************************************************************


**** Lessons learned ****************************************
I had not made an interface between C to FORTRAN before. As
such, I needed to understand that FORTRAN stores array data 
column-first whereas C stores data row-first. Secondly, the
IRI subroutines did not specify the type of the real number
data, and I found that the default real data type is real(4),
which corresponds to float in C. Finally, logical data must 
be passed as int from C to FORTRAN. 

A particulary useful resource was the guide found at this
link https://faculty.sites.iastate.edu/keinert/files/inline-files/calling.pdf.
Here, I learned the necessary syntax to expose the FORTRAN
subroutine methods.
*************************************************************