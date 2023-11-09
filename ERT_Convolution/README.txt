**** Description ********************************************
Computes the discrete 2-D convolution h(x,y) given values for two 
functions f(x,y) and g(x,y) along a grid. 
https://en.wikipedia.org/wiki/Multidimensional_discrete_convolution
Grid min and max values along x and y for the operation are defined
by taking the minimum and maximum values of x and y that are read
from input files for f(x,y) and g(x,y). The sets of (x,y) need not 
be the same for each function f and g, but the algorithm assumes 
homogeneous boundary conditions. This means that if a value for 
either f or g has not been provided by the user, we assume a value 
of zero for the function at that point.
*************************************************************

**** Usage **************************************************
C++ scripts
There are two input files that should be placed in the working
directory: convolve_raw_0.dat and convolve_raw_1.dat

Both files contain three columns of numbers where
the first two columns are (x,y) and the third column is
f(x,y). The length of the three columns should be equal.
Values should be tab-delimited along a row.

Outputs
convolvedX.dat - grid values in x used in convolution calculation
convolvedY.dat - grid values in y used in convolution calculation
convolved.dat  - three tab-delimited columns of equal length
			   - first two columns contain 0-based indices corresponding
				 to i-th value listed in convolvedX.dat (first column)
				 or convolvedY.dat (second column)
			   - third column is convolved value for a given (x,y)

Python script
The .dat files outputted from C++ are the inputs for ConvVisual.py, 
which generates a contour plot (conv.pdf) within the working directory.
Contour levels indicate the log_10 value of h(x,y) (the convolution)
where the levels are calculated by bilinear interpolation.
*************************************************************

**** Compilation instructions *******************************
Python
1. Install Python 3.10. Recommend installing using the PyCharm
   IDE (version 2021.3.1 at the time of writing this README)
2. Necessary python packages. Can be installed through PyCharm or
   pip (https://pip.pypa.io/en/stable/installation/)
   - matplotlib (version 3.8.1 or later)
   - numpy (version 1.26.1 or later)
   - scipy (version 1.11.3 or later)

C++ scripts were compiled using Visual Studio 2022 community edition
(version 17.7.5)
*************************************************************