#####################################################################
# File   : Makefile
# Purpose: Framework for Exercise 5 of Formal Verification course 06/07
# Author : Georg Weissenbacher, georg.weissenbacher@inf.ethz.ch
#####################################################################

CPP	= g++

#tested with version 2.5.31
FLEX    = flex

# make sure bison is version 2.1
BISON   = bison

# make this point to MiniSat
MINISAT = MiniSat-p_v1.14

INCLUDE = -I $(MINISAT)
LIBS    = $(MINISAT)/Solver.o $(MINISAT)/Proof.o $(MINISAT)/File.o

CFLAGS  =
LDFLAGS =
OBJS    = graph/graph.o \
          bigint/bigint.o \
          parsing/string_container.o parsing/dstring.o \
          parsing/scanner.o parsing/parser.o parsing/ast.o \
          parsing/parsing_context.o \
          sat/cnf.o sat/dimacs_cnf.o sat/satcheck_minisat.o \
          sat/prop.o sat/cnf_clause_list.o sat/threeval.o \
          sat/skeleton.o \
          lazy/lazy_base.o lazy/lazy_dpll.o lazy/dpll_t.o \
          simplex/rational.o simplex/gcd.o \
          simplex/integer.o simplex/linear_constraint.o \
          simplex/general_simplex_tableau.o \
          simplex/general_simplex.o \
          equality/equality_logic.o equality/union_find.o

DEBUG   = -g

.SUFFIXES: .cpp .o

all: DP.lib

DP.lib: $(OBJS) $(MINISAT)/Solver.o
	$(LD) $(OBJS) $(LIBS) -r -o $@

$(MINISAT)/Solver.o:
	(cd $(MINISAT); $(MAKE))

parsing/ast.o: parsing/ast.cpp parsing/ast.h parsing/parser.hpp

parsing/parser.o: parsing/parser.cpp parsing/parser.hpp parsing/ast.h

parsing/scanner.o: parsing/scanner.cpp parsing/parser.hpp parsing/ast.h 

parsing/scanner.cpp: parsing/scanner.l
	$(FLEX) -o$@ $<

parsing/parser.hpp: parsing/parser.cpp

parsing/parsing_context.h: parsing/ast.h

parsing/ast.h: parsing/parser.hpp

parsing/parser.cpp: parsing/parser.y
	$(BISON) -v -d $< --output-file=$@

.cpp.o:
	$(CPP) $(DEBUG) -Wall -c $(INCLUDE) $< -o $@

clean:
	rm -f $(OBJS) *~ DP.lib
	rm -f parsing/parser.cpp parsing/parser.hpp
	rm -f parsing/scanner.cpp
	rm -f *.hh semantic.cache
	(cd $(MINISAT); $(MAKE) clean)
