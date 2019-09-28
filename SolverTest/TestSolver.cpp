/*
 * File:   TestSolver.cpp
 * Author: Randy
 *
 * Created on Apr 7, 2014, 7:07:16 AM
 */

#include "SolverIncludes.h"
#include <iostream>

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
   solver::Solver   solver;
   Solution solution = solver.Solve(problem);
   if (solution != expectedResult)
   {
      // open up a file and dump the result to it
      FILE* resultFile = std::fopen("solveFailure.txt", "wt");
      if (resultFile == nullptr)
         throw SolverException("doSolverTestCase: error opening dump file");
      solver.GetResult().DumpToFile(resultFile);
      fclose(resultFile);

      throw SolverException("doSolverTestCase: result does not match expectation");
   }
}


void TestSolver::TestScottsProblem(void)
{
   solver::Solver   solver;
   Solution solution = solver.Solve(scottsProblem);
   if (solution.IsFailure())
      throw SolverException("TestSolver::TestScottsProblem: expected success");
}

void TestSolver::TestCache(void)
{
   // run once normally
   solver::Solver solverWithCacheing;
   Solution solutionWithCacheing = solverWithCacheing.Solve(cacheTestProblem);

   // run once witch cacheing disabled
   solver::Solver solverWithoutCacheing;
   solverWithoutCacheing.DisableCacheing();
   Solution solutionWithoutCacheing = solverWithoutCacheing.Solve(cacheTestProblem);

   // we should get the same result
   if (solutionWithCacheing != solutionWithoutCacheing)
      throw SolverException("TestSolver::TestCache: results do not match");

   // but faster
   int stepsWithCacheing = solverWithCacheing.GetTotalBranchesTested();
   int stepsWithoutCacheing = solverWithoutCacheing.GetTotalBranchesTested();
   if (stepsWithoutCacheing <= stepsWithCacheing)
      throw SolverException("TestSolver::TestCache: cacheing didn't help");
}

SeahavenProblem TestSolver::TestRandomCase(void)
{
   solver::Solver solver;
   SeahavenProblem problem = SeahavenProblem::CreateRandom();
   //problem.Dump();
   Solution solution = solver.Solve(problem);
   if (solution.IsFailure())
      std::cout << "Lose: ";
   else
      std::cout << "Win: ";

   //std::cout << solver.GetResult().GetNumberOfThroneToTowerMoves() << " ";

   return problem;
}

void TestSolver::TestSlowProblem(void)
{
   solver::Solver   solver;
   Solution solution = solver.Solve(slowTestProblem);

   FILE* resultFile = std::fopen("TestSlowProblem.txt", "wt");
   if (resultFile == nullptr)
      throw SolverException("TestSlowProblem: error opening dump file");
   solver.GetResult().DumpToFile(resultFile);
   fclose(resultFile);
}


void TestSolver::TestSlowProblem2(void)
{
   solver::Solver   solver;
   Solution solution = solver.Solve(slowTestProblem2);
}
