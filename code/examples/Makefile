#####################################################################
# File   : Makefile
# Purpose: Framework for Exercise 5 of Formal Verification course 06/07
# Author : Georg Weissenbacher, georg.weissenbacher@inf.ethz.ch
#####################################################################

CPP	= g++

INCLUDE = -I ../lib
LIBS    = ../lib/DP.lib

PROGRAMS = main_lazy bool2cnf simplex equality_logic

CFLAGS  =
LDFLAGS =

DEBUG   = -g

.SUFFIXES: .cpp .o

all: $(PROGRAMS)

main_lazy: main_lazy.o
	$(CPP) main_lazy.o $(LIBS) -o $@

simplex: simplex.o
	$(CPP) simplex.o $(LIBS) -o $@

bool2cnf: bool2cnf.o
	$(CPP) bool2cnf.o $(LIBS) -o $@

equality_logic: equality_logic.o
	$(CPP) equality_logic.o $(LIBS) -o $@

.cpp.o:
	$(CPP) $(DEBUG) -Wall -c $(INCLUDE) $< -o $@

clean:
	rm -f *.o *~ $(PROGRAMS)
	rm -f *.hh semantic.cache

