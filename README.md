## Projects

1. Extensions of SAT
Download your favorite open-source SAT solver (which does not support the features listed below) and add the following features:
Support for Pseudo-Boolean Constraints (linear constraints over Boolean variables). For example, in addition to the CNF, the solver will be able to take as input constraints such as 3x + 2y <= 3, where x,y are Boolean variables appearing in the CNF formula. The satisfying solution must satisfy both the CNF constraints and the pseudo-Boolean constraints. Note: there is an exponential way to do this and a polynomial one - find the polynomial solution.
All-SAT. Add an option to the SAT solver of getting all satisfying assignments. Further, allow the user to specify a set of 'important variables', and find all the solutions that their projection to this set of variables is distinct.

2. Implement range allocation for equality logic with SAT
Equality logic can be decided by assigning an adequate finite discrete range of values to each variable (see Sect. 4.5). Implement a simple version of this procedure in which each variable has ceiling(log n) bits, where n is the number of variables, via a reduction to a propositional formula. Chapter 6 has relevant information about encoding such bitvector formulas. As a more advanced project, do the same for a nonuniform range, as explained in Sects. 4.5.2 - 4.5.3.

3. Implement a lazy range allocation for equality logic inside DPLL(T)
Equality logic can be decided by assigning an adequate finite discrete range of values to each variable. As in the previous project, encode this range with a bitvector of the appropriate size. In this project, however, implement a procedure that generates the clauses for the bitvector constraints incrementally within DPLL(T). Use union-find to decide which constraints need to be added. Compare the performance of your implementation with the eager encoding using suitable benchmarks.

4. On-demand Ackermann constraints for EUF
Equality logic with uninterpreted functions (EUF) can be reduced to equality logic by adding appropriate Ackermann constraints. The number of constraints can be large, and may be a burden for the procedure.
Implement a lazy procedure for adding the Ackermann constraints: If a satisfying assignment is obtained that violates functional consistency, add those constraints required to eliminate the assignment, and re-iterate.
Compare the performance of your implementation with the direct encoding using suitable benchmarks.

5. Implement Branch & Bound based on General Simplex.
Create an incremental version of Branch & Bound (Sec. 5.3) based on general simplex (which is also incremental). This means that given two similar problem instances, solving them consecutively while reusing information that was learned during the solving of the first instance, makes solving the second instance faster. For example, suppose that the second linear system is equivalent to the first one except that one of the variables is now required to be integral, and there was no such requirement in the first system. Obviously we can do better than just rerunning the solver from scratch.

6. Strict inequalities for the general simplex algorithm
Add support for constraints with strict inequalities over the reals to the general simplex algorithm (Sec. 5.2)!

7. Implement Fourier-Motzkin and compare it to Simplex
Fourier-Motzkin can be used to decide a conjunction of linear constraints over the reals.
Implement Fourier-Motzkin for a conjunction of linear constraints.
Compare the performance of your implementation with a simplex implementation using suitable benchmarks.

8. Integrating the OMEGA test into DPLL(T)
The OMEGA test is a decision procedure for a conjunction of linear constraints over the integers, and is described in Sec. 5.5. Integrate the OMEGA test into DPLL(T) (Chap. 11) to obtain a decision procedure for linear arithmetic over the integers with arbitrary Boolean connectives.

9. Making OMEGA incremental
The OMEGA test, as described in Sec. 5.5, is not incremental.
Propose and implement an incremental variant, which should include the possibility to deactivate constraints.
Integrate this incremental variant into DPLL(T). Compare the performance of the non-incremental variant with your new version.

10. Using DPLL(T) for splitting of the grey shadow
The OMEGA test (Sec. 5.5) is a recursive procedure. Most of the recursive calls correspond to a conjunction, that is, if any of the sub-problems is unsatisfiable, so is the original one.
The exception is the grey shadow: it corresponds to a disjunction, i.e., only if all sub-problems are unsatisfiable, so is the original one. The original OMEGA test performs this case-splitting inside the algorithm.

Use the propositional part of DPLL(T) to implement this splitting.
Compare the performance of both variants using suitable benchmarks.
 
11. Compare various circuits for multipliers for bit-vector arithmetic
Bit-vector arithmetic is usually decided using a translation into a circuit-equivalent. There are many ways to encode multipliers, to be found in the literature on circuit construction. Implement and compare a number of variants of multipliers.

12. Build an SMT-frontend
Build an SMT-frontend for a theory of your choice. Test it using the benchmarks from the SMT repository.

13. Build a lazy bit-vector flattener
Implement the algorithm described in Section 6.3. Compare its performance with an eager version of the algorithm.

14. Implement the decision procedure for arrays
Implement the algorithm described in Chapter 7.

15. Branch & Bound + DPLL(T) for Pseudo-Boolean Constraints
Pseudo-Boolean optimization problems consist of a set of Boolean constraints (usually in CNF) and a cost function, which associates integer weights with a subset of the Boolean variables. The goal is to find the satisfying assignment with the lowest cost. Implement such a procedure on top of DPLL(T). Use Branch & Bound for the cost function.

16. Replacing SAT with BDDs
Replace the SAT solver in DPLL(T) with a BDD package, and compare the performance. One strength of BDDs is that they provide inexpensive means to compute projections. What part of DPLL(T) can leverage this capability?
