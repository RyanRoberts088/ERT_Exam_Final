

# Define compilers
CC = gcc -g
# Define compiler flags
CCFLAGS = -DBUILDING_C_DLL
# Define libraries

# Define C files
CFILES = \
ERT_CcodeOptimization.c\

# Define C++ objects
COBJECTS = \
ERT_CcodeOptimization.o\

# Targets
ERT_CcodeOptimization.o: $(CFILES)
	$(CC) -c $(CCFLAGS) $(CFILES)
ERT_CcodeOptimization.exe : $(COBJECTS)
	$(CC) -o ERT_CcodeOptimization.exe $(COBJECTS)