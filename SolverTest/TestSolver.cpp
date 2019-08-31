/*
 * File:   TestSolver.cpp
 * Author: Randy
 *
 * Created on Apr 7, 2014, 7:07:16 AM
 */

#include "SolverIncludes.h"

#include "SeahavenProblem.h"
#include "Solver.h"
#include "SolverException.h"

#include "TestSolver.h"


TestSolver::TestSolver() {
}

TestSolver::~TestSolver() {
}

void TestSolver::doSolverTestCase(const SeahavenProblem &problem, const Solution &expectedResult)
{
   Solver   solver;
   Solution solution = solver.Solve(problem);
   if (solution != expectedResult)
      throw SolverException("doSolverTestCase: result does not match expectation");
}


