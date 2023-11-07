

# Define compilers
CC = gcc -g
# Define compiler flags
CCFLAGS = -DBUILDING_C_DLL
# Define libraries
USRLIBS = -Wl,--out-implib,ERT_GISRadar.a,--output-def,ERT_GISRadar.def

# Define C files
CFILES = \
GISRadar.c\

# Define C++ objects
COBJECTS = \
GISRadar.o\

# Targets
GISRadar.o: $(CFILES)
	$(CC) -c $(CCFLAGS) $(CFILES)
GISRadar.dll : $(COBJECTS) $(FOBJECTS)
	$(CC) -shared -o GISRadar.dll $(COBJECTS) $(FOBJECTS) $(USRLIBS) $(LFLAGS)