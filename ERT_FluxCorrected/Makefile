

# Define compilers
CC = gcc -g
# Define compiler flags
CCFLAGS = -DBUILDING_C_DLL
# Define libraries

# Define C files
CFILES = \
ERT_FluxCorrected.c\
Simulate.c\
flux_corrected_transport.c\

# Define C objects
COBJECTS = \
ERT_FluxCorrected.o\
Simulate.o\
flux_corrected_transport.o\

# Targets
ERT_FluxCorrected.o: $(CFILES)
	$(CC) -c $(CCFLAGS) $(CFILES)
ERT_FluxCorrected.exe : $(COBJECTS) $(FOBJECTS)
	$(CC) -o ERT_FluxCorrected.exe $(COBJECTS) $(FOBJECTS) $(USRLIBS) $(LFLAGS)