# System architecture
SYSTEM     = x86-64_linux

# Static library format for Cplex
LIBFORMAT  = static_pic

# Source code folder
SRC	= src
INCLUDE = include

# Machine hostname
MACHINE = $(shell hostname)

# Library type(STATIC or DYNAMIC)
MERGE = DYNAMIC

##### Folders
# Temp folders
TMP_ILS = ./tmp/ILS
TMP_STATIC = ./tmp/lib/static
# Perm folders
DAT_DOXYFILE = ./dat/doxyfile
DAT_INSTANCES = ./dat/instances
DAT_LP_MODELS = ./dat/lp_models
DAT_RESULTS = ./dat/results


# Cplex directory
CPLEXDIR	  = /opt/ibm/ILOG/CPLEX_Studio1261/cplex

# Concert directory
CONCERTDIR	  = /opt/ibm/ILOG/CPLEX_Studio1261/concert

# Compiler
CCC = g++-4.8
# CCC = g++

# Compilation parameters (Add afterward: --coverage -pg -ftree-vectorize -mfpmath=sse -march=native)
# CCOPT = -std=gnu++0x -O3 -ftree-vectorize -mfpmath=sse -march=native -march=native -flto -g -m64 -fPIC -fexceptions -DNDEBUG -DIL_STD
CCOPT = -std=gnu++0x -O3 -ftree-vectorize -mfpmath=sse -march=native -march=native -g -m64 -fPIC -fexceptions -DNDEBUG -DIL_STD

# Cplex static libraries directory
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

# Concert static libraries directory
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

# Include libraries identifiers
CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -pthread

# Cplex header's directory
CPLEXINCDIR   = $(CPLEXDIR)/include

# Concert header's directory
CONCERTINCDIR = $(CONCERTDIR)/include

# Header's include path
# CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR)
CCFLAGS = $(CCOPT)

# Executable name
CPP_EX = CaRS-ILS

# Compiling
all:
	mkdir -p $(TMP_ILS)
	mkdir -p $(TMP_ILS)/heuristic
	mkdir -p $(TMP_STATIC)
	mkdir -p $(DAT_DOXYFILE)
	mkdir -p $(DAT_INSTANCES)
	mkdir -p $(DAT_LP_MODELS)
	mkdir -p $(DAT_RESULTS)
	make -j8 $(CPP_EX);

# Executing
execute: $(CPP_EX)
	./$(CPP_EX)

# Cleaning
clean:
	/bin/rm -rf $(CPP_EX)
	/bin/rm -rf ./tmp
	/bin/rm -rf ./dat
	/bin/rm -rf *.tex


########################## GENERATING OBJECT's ######################################################

# CONFIGURATION - INSTANCES
$(TMP_ILS)/instance.o: $(SRC)/instance.cpp $(INCLUDE)/instance.h
	$(CCC) -c $(CCFLAGS) $(SRC)/instance.cpp -o $(TMP_ILS)/instance.o

# STRUCTURE - SOLUTION
$(TMP_ILS)/solution.o: $(SRC)/solution.cpp $(INCLUDE)/solution.h
	$(CCC) -c $(CCFLAGS) $(SRC)/solution.cpp -o $(TMP_ILS)/solution.o

# STRUCTURE - TIMER
$(TMP_ILS)/FWChrono.o: $(SRC)/FWChrono.cpp $(INCLUDE)/FWChrono.h
	$(CCC) -c $(CCFLAGS) $(SRC)/FWChrono.cpp -o $(TMP_ILS)/FWChrono.o

# STRUCTURE - RANDOM GEN
$(TMP_ILS)/mt19937ar.o: $(SRC)/mt19937ar.c $(INCLUDE)/mt19937ar.h
	$(CCC) -c $(CCFLAGS) $(SRC)/mt19937ar.c -o $(TMP_ILS)/mt19937ar.o

# STRUCTURE - LOGGER
$(TMP_ILS)/heuristic/logger.o: $(SRC)/heuristic/logger.cpp $(INCLUDE)/heuristic/logger.h
	$(CCC) -c $(CCFLAGS) $(SRC)/heuristic/logger.cpp -o $(TMP_ILS)/heuristic/logger.o

# ILS
$(TMP_ILS)/heuristic/constructor.o: $(SRC)/heuristic/constructor.cpp $(INCLUDE)/heuristic/constructor.h
	$(CCC) -c $(CCFLAGS) $(SRC)/heuristic/constructor.cpp -o $(TMP_ILS)/heuristic/constructor.o
$(TMP_ILS)/heuristic/neighborhoods.o: $(SRC)/heuristic/neighborhoods.cpp $(INCLUDE)/heuristic/neighborhoods.h
	$(CCC) -c $(CCFLAGS) $(SRC)/heuristic/neighborhoods.cpp -o $(TMP_ILS)/heuristic/neighborhoods.o
$(TMP_ILS)/heuristic/perturbation.o: $(SRC)/heuristic/perturbation.cpp $(INCLUDE)/heuristic/perturbation.h
	$(CCC) -c $(CCFLAGS) $(SRC)/heuristic/perturbation.cpp -o $(TMP_ILS)/heuristic/perturbation.o
$(TMP_ILS)/heuristic/ils.o: $(SRC)/heuristic/ils.cpp $(INCLUDE)/heuristic/ils.h
	$(CCC) -c $(CCFLAGS) $(SRC)/heuristic/ils.cpp -o $(TMP_ILS)/heuristic/ils.o

# MAIN
$(TMP_ILS)/main.o: $(SRC)/main.cpp
	$(CCC) -c $(CCFLAGS) $(SRC)/main.cpp -o $(TMP_ILS)/main.o

########################## OBJECT's LIBRARIES #######################################################
# CONFIGURATION
$(TMP_ILS)/Configuration.o: $(TMP_ILS)/FWChrono.o $(TMP_ILS)/mt19937ar.o
	gcc -Wl,-r $(TMP_ILS)/FWChrono.o $(TMP_ILS)/mt19937ar.o -o $(TMP_ILS)/Configuration.o -nostdlib

# STRUCTURE & TIMER
$(TMP_ILS)/Structure.o:  $(TMP_ILS)/instance.o $(TMP_ILS)/solution.o $(TMP_ILS)/heuristic/logger.o
	gcc -Wl,-r $(TMP_ILS)/instance.o $(TMP_ILS)/solution.o $(TMP_ILS)/heuristic/logger.o -o $(TMP_ILS)/Structure.o -nostdlib

# ILS
$(TMP_ILS)/ILS.o: $(TMP_ILS)/heuristic/constructor.o $(TMP_ILS)/heuristic/neighborhoods.o $(TMP_ILS)/heuristic/perturbation.o $(TMP_ILS)/heuristic/ils.o
	gcc -Wl,-r $(TMP_ILS)/heuristic/constructor.o $(TMP_ILS)/heuristic/neighborhoods.o $(TMP_ILS)/heuristic/perturbation.o $(TMP_ILS)/heuristic/ils.o -o $(TMP_ILS)/ILS.o -nostdlib

########################## LINKANDO TUDO ########################################################

$(CPP_EX): $(TMP_ILS)/Configuration.o $(TMP_ILS)/Structure.o $(TMP_ILS)/ILS.o $(TMP_ILS)/main.o
	$(CCC)  $(CCFLAGS) $(TMP_ILS)/Configuration.o $(TMP_ILS)/Structure.o $(TMP_ILS)/ILS.o $(TMP_ILS)/main.o -L$(TMP_STATIC) -o $(CPP_EX)
#endif
