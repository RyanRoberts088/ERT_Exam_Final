

# Define compilers
CC = gcc -g
FC = gfortran -g
# Define compiler flags
FCFLAGS = 
CCFLAGS = -DBUILDING_C_DLL
# Define libraries
USRLIBS = -Wl,--out-implib,ERT_IRIPlotting.a,--output-def,ERT_IRIPlotting.def
LFLAGS = -lgfortran

# Define FORTRAN files
FFILES = \
irisub.for\
irifun.for\
iritec.for\
iridreg.for\
igrf.for\
cira.for\
iriflip.for\
# Define C files
CFILES = \
ERT_IRIPlotting.c\
IRIPlot.c\

#Define FORTRAN objects
FOBJECTS = \
irisub.o\
irifun.o\
iritec.o\
iridreg.o\
igrf.o\
cira.o\
iriflip.o\
# Define C++ objects
COBJECTS = \
ERT_IRIPlotting.o\
IRIPlot.o\

# Targets
irisub.o: $(FFILES)
	$(FC) -c $(FFILES)
ERT_IRIPlotting.o: $(CFILES)
	$(CC) -c $(CCFLAGS) $(CFILES)
ERT_IRIPlotting.exe : $(COBJECTS) $(FOBJECTS)
	$(CC) -o ERT_IRIPlotting.exe $(COBJECTS) $(FOBJECTS) $(USRLIBS) $(LFLAGS)
	
	
# for generating a shared library
#g++ -g -shared -o ERT_IRIPlotting.dll ERT_IRIPlotting.o IRIPlot.o irisub.o irifun.o iritec.o iridreg.o igrf.o cira.o iriflip.o -Wl,--out-implib,ERT_IRIPlotting.a,--output-def,ERT_IRIPlotting.def -lgfortran