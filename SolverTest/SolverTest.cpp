// SolverTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <iostream>
#include "SolverIncludes.h"
#include "Solver.h"
#include "TestCardOverlap.h"
#include "TestSolver.h"

int main()
{
   TestSolver tester;

   tester.testSingleColumnToTowerProblem();
   tester.testSingleColumnToColumnProblem();

   std::cout << "Starting slow problem... ";
   tester.TestSlowProblem2();
   std::cout << "done" << std::endl;

   tester.TestSlowProblem();
   TestCardOverlap();

   tester.TestCombineTowersProblem();
   tester.TestCache();
   tester.testZeroMoveProblem();
   tester.TestScottsProblem();

#if 0
   // try to find problems that require the ability to move thrones to towers
   int failureCount = 0;
   for (;;)
   {
      SeahavenProblem problem = SeahavenProblem::CreateRandom();

      Solver solverWithoutThroneMoves;
      solverWithoutThroneMoves.DisableMovingKingsFromEmptyColumnsToTowers();
      Solution solutionWithoutThroneMoves = solverWithoutThroneMoves.Solve(problem);

      if (solutionWithoutThroneMoves.IsFailure())
      {
         std::cout << ++failureCount;
         std::cout << ": Failure without throne moves... ";
         Solver solverWithThroneMoves;
         Solution solutionWithThroneMoves = solverWithThroneMoves.Solve(problem);
         if (solutionWithThroneMoves.IsFailure())
            std::cout << "Failure with throne moves.";
         else
            std::cout << "SUCCESS WITH THRONE MOVES!!!";
         std::cout << std::endl;
      }
   }
#endif

   double slowestProblemTime = 0;
   SeahavenProblem slowestProblem;
   for (int i = 0; i < 1000; ++i)
   {
      std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
      SeahavenProblem problem = tester.TestRandomCase();
      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

      std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
      std::cout << time_span.count() << std::endl;

      if (time_span.count() > slowestProblemTime)
      {
         slowestProblemTime = time_span.count();
         slowestProblem = problem;
      }
   }

   std::cout << "Slowest problem:" << std::endl;
   slowestProblem.Dump(stdout);
}


