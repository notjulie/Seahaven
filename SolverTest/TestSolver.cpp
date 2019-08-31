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


void TestSolver::TestCache(void)
{
   // run once normally
   Solver solverWithCacheing;
   Solution solutionWithCacheing = solverWithCacheing.Solve(cacheTestProblem);

   // run once witch cacheing disabled
   Solver solverWithoutCacheing;
   solverWithoutCacheing.DisableCacheing();
   Solution solutionWithoutCacheing = solverWithoutCacheing.Solve(cacheTestProblem);

   // we should get the same result
   if (solutionWithCacheing != solutionWithoutCacheing)
      throw SolverException("TestSolver::TestCache: results do not match");

   // but faster
   int stepsWithCacheing = solverWithCacheing.GetTotalSteps();
   int stepsWithoutCacheing = solverWithoutCacheing.GetTotalSteps();
   if (stepsWithoutCacheing <= stepsWithCacheing)
      throw SolverException("TestSolver::TestCache: cacheing didn't help");
}

void TestSolver::TestRandomCase(void)
{
   Solver solver;
   SeahavenProblem problem = SeahavenProblem::CreateRandom();
   problem.Dump();
   solver.Solve(problem);
}
