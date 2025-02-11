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
TMP_CARS = ./tmp/CARS
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

LEMONINCDIR = /usr/local/include/lemon

LEMONLIBDIR = /usr/local/lib/libemon.a

# Compiler
# CCC = g++-4.8
CCC = g++

# Compilation parameters (Add afterward: --coverage -pg -ftree-vectorize -mfpmath=sse -march=native)
CCOPT = -std=gnu++0x -O3 -ftree-vectorize -mfpmath=sse -march=native -march=native -flto -g -m64 -fPIC -fexceptions -DNDEBUG -DIL_STD
# CCOPT = -std=gnu++0x -O3 -ftree-vectorize -mfpmath=sse -march=native -march=native -g -m64 -fPIC -fexceptions -DNDEBUG -DIL_STD

# Cplex static libraries directory
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

# Concert static libraries directory
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

# Include libraries identifiers
CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -L$(LEMONLIBDIR) -lconcert -lm -pthread -lemon

# Cplex header's directory
CPLEXINCDIR   = $(CPLEXDIR)/include

# Concert header's directory
CONCERTINCDIR = $(CONCERTDIR)/include

# Header's include path
CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) #-I$(LEMONINCDIR)
# CCFLAGS = $(CCOPT)

# Executable name
CPP_EX = SP-CaRS

# Compiling
all:
	mkdir -p $(TMP_CARS)
	# mkdir -p $(TMP_CARS)/heuristic
	# mkdir -p $(TMP_CARS)/exact
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
	/bin/rm -rf cutset.txt
	/bin/rm -rf *.lp


########################## GENERATING OBJECT's ######################################################

# CONFIGURATION - INSTANCES
$(TMP_CARS)/instance.o: $(SRC)/instance.cpp $(INCLUDE)/instance.h
	$(CCC) -c $(CCFLAGS) $(SRC)/instance.cpp -o $(TMP_CARS)/instance.o

# STRUCTURE - SOLUTION
$(TMP_CARS)/solution.o: $(SRC)/solution.cpp $(INCLUDE)/solution.h
	$(CCC) -c $(CCFLAGS) $(SRC)/solution.cpp -o $(TMP_CARS)/solution.o

# STRUCTURE - TIMER
$(TMP_CARS)/FWChrono.o: $(SRC)/FWChrono.cpp $(INCLUDE)/FWChrono.h
	$(CCC) -c $(CCFLAGS) $(SRC)/FWChrono.cpp -o $(TMP_CARS)/FWChrono.o

# STRUCTURE - RANDOM GEN
$(TMP_CARS)/mt19937ar.o: $(SRC)/mt19937ar.c $(INCLUDE)/mt19937ar.h
	$(CCC) -c $(CCFLAGS) $(SRC)/mt19937ar.c -o $(TMP_CARS)/mt19937ar.o

# STRUCTURE - LOGGER
$(TMP_CARS)/logger.o: $(SRC)/heuristic/logger.cpp $(INCLUDE)/heuristic/logger.h
	$(CCC) -c $(CCFLAGS) $(SRC)/heuristic/logger.cpp -o $(TMP_CARS)/logger.o

# STRUCTURE - BITARRAY
$(TMP_CARS)/bit_array.o: $(SRC)/bit_array.cpp $(INCLUDE)/bit_array.h
	$(CCC) -c $(CCFLAGS) $(SRC)/bit_array.cpp -o $(TMP_CARS)/bit_array.o

# STRUCTURE - ARC
$(TMP_CARS)/arc.o: $(SRC)/arc.cpp $(INCLUDE)/arc.h
	$(CCC) -c $(CCFLAGS) $(SRC)/arc.cpp -o $(TMP_CARS)/arc.o

# STRUCTURE - ARCLIST
$(TMP_CARS)/arc_list.o: $(SRC)/arc_list.cpp $(INCLUDE)/arc_list.h
	$(CCC) -c $(CCFLAGS) $(SRC)/arc_list.cpp -o $(TMP_CARS)/arc_list.o


# ILS
$(TMP_CARS)/constructor.o: $(SRC)/heuristic/constructor.cpp $(INCLUDE)/heuristic/constructor.h
	$(CCC) -c $(CCFLAGS) $(SRC)/heuristic/constructor.cpp -o $(TMP_CARS)/constructor.o
$(TMP_CARS)/neighborhoods.o: $(SRC)/heuristic/neighborhoods.cpp $(INCLUDE)/heuristic/neighborhoods.h
	$(CCC) -c $(CCFLAGS) $(SRC)/heuristic/neighborhoods.cpp -o $(TMP_CARS)/neighborhoods.o
$(TMP_CARS)/perturbation.o: $(SRC)/heuristic/perturbation.cpp $(INCLUDE)/heuristic/perturbation.h
	$(CCC) -c $(CCFLAGS) $(SRC)/heuristic/perturbation.cpp -o $(TMP_CARS)/perturbation.o
$(TMP_CARS)/ils.o: $(SRC)/heuristic/ils.cpp $(INCLUDE)/heuristic/ils.h
	$(CCC) -c $(CCFLAGS) $(SRC)/heuristic/ils.cpp -o $(TMP_CARS)/ils.o

# EXACT
$(TMP_CARS)/MinCut.o: $(SRC)/exact/MinCut.cpp $(INCLUDE)/exact/MinCut.h
	$(CCC) -c $(CCFLAGS) $(SRC)/exact/MinCut.cpp -o $(TMP_CARS)/MinCut.o
$(TMP_CARS)/gmindcut.o: $(SRC)/exact/gmindcut.cpp $(INCLUDE)/exact/gmindcut.h
	$(CCC) -c $(CCFLAGS) $(SRC)/exact/gmindcut.cpp -o $(TMP_CARS)/gmindcut.o
$(TMP_CARS)/node_info.o: $(SRC)/exact/node_info.cpp $(INCLUDE)/exact/node_info.h
	$(CCC) -c $(CCFLAGS) $(SRC)/exact/node_info.cpp -o $(TMP_CARS)/node_info.o
$(TMP_CARS)/callbacks.o: $(SRC)/exact/callbacks.cpp $(INCLUDE)/exact/callbacks.h
	$(CCC) -c $(CCFLAGS) $(SRC)/exact/callbacks.cpp -o $(TMP_CARS)/callbacks.o
$(TMP_CARS)/model.o: $(SRC)/exact/model.cpp $(INCLUDE)/exact/model.h
	$(CCC) -c $(CCFLAGS) $(SRC)/exact/model.cpp -o $(TMP_CARS)/model.o
$(TMP_CARS)/solver.o: $(SRC)/exact/solver.cpp $(INCLUDE)/exact/solver.h
	$(CCC) -c $(CCFLAGS) $(SRC)/exact/solver.cpp -o $(TMP_CARS)/solver.o

# MAIN
$(TMP_CARS)/main.o: $(SRC)/main.cpp
	$(CCC) -c $(CCFLAGS) $(SRC)/main.cpp -o $(TMP_CARS)/main.o


########################## LINKANDO TUDO ########################################################

$(CPP_EX): $(TMP_CARS)/FWChrono.o $(TMP_CARS)/mt19937ar.o $(TMP_CARS)/instance.o $(TMP_CARS)/solution.o $(TMP_CARS)/logger.o $(TMP_CARS)/bit_array.o $(TMP_CARS)/arc.o $(TMP_CARS)/arc_list.o $(TMP_CARS)/constructor.o $(TMP_CARS)/neighborhoods.o $(TMP_CARS)/perturbation.o $(TMP_CARS)/ils.o $(TMP_CARS)/MinCut.o $(TMP_CARS)/gmindcut.o $(TMP_CARS)/node_info.o $(TMP_CARS)/callbacks.o $(TMP_CARS)/model.o $(TMP_CARS)/solver.o $(TMP_CARS)/main.o
	$(CCC) $(CCFLAGS) $(TMP_CARS)/FWChrono.o $(TMP_CARS)/mt19937ar.o $(TMP_CARS)/instance.o $(TMP_CARS)/solution.o $(TMP_CARS)/logger.o $(TMP_CARS)/bit_array.o $(TMP_CARS)/arc.o $(TMP_CARS)/arc_list.o $(TMP_CARS)/constructor.o $(TMP_CARS)/neighborhoods.o $(TMP_CARS)/perturbation.o $(TMP_CARS)/ils.o $(TMP_CARS)/MinCut.o $(TMP_CARS)/gmindcut.o $(TMP_CARS)/node_info.o $(TMP_CARS)/callbacks.o $(TMP_CARS)/model.o $(TMP_CARS)/solver.o $(TMP_CARS)/main.o -L$(TMP_STATIC) -o $(CPP_EX) $(CCLNFLAGS)


#endif
