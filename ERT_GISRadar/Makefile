

# Define compilers
CC = gcc -g
# Define compiler flags
CCFLAGS = -DBUILDING_C_DLL
# Define libraries
USRLIBS = -Wl,--out-implib,GISRadar.a
LLIBS = -L. -lGISRadar

# Define C files
CFILES = \
GISRadar.c\

MAINCFILES = \
ERT_GISRadar.c\

# Define C++ objects
COBJECTS = \
GISRadar.o\

# Targets
GISRadar.o: $(CFILES)
	$(CC) -c $(CCFLAGS) $(CFILES)
GISRadar.dll : $(COBJECTS)
	$(CC) -shared -o GISRadar.dll $(COBJECTS) $(USRLIBS)
ERT_GISRadar.exe: $(MAINCFILES)
	$(CC) -o ERT_GISRadar.exe $(MAINCFILES) $(LLIBS)