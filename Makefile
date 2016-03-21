FC=gfortran
CC=g++
CXX=g++
FFLAGS=
CCFLAGS=-fPIC -g -Wall
EXEC=transformTTree

ROOTCONFIG   := ${ROOTSYS}/bin/root-config

ROOTCFLAGS   := $(shell $(ROOTCONFIG) --cflags)
ROOTLIBS     := $(shell $(ROOTCONFIG) --libs)

all: $(EXEC)

transformTTree : transformTTree.o
	$(CC) $^ $(ROOTCFLAGS) $(ROOTLIBS)  -o $@ 

transformTTree.o : transformTTree.C
	$(CC)  -c $(CCFLAGS) $(ROOTCFLAGS) $< -o $@



#####################################

clean :
	-rm -f *.o $(EXEC)
