// SolverTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <iostream>
#include "TestCardOverlap.h"
#include "TestSolver.h"

int main()
{
   TestSolver tester;

   TestCardOverlap();


   tester.TestCombineTowersProblem();
   tester.TestCache();
   tester.testZeroMoveProblem();
   tester.testSingleColumnToTowerProblem();
   tester.testSingleColumnToColumnProblem();
   tester.TestScottsProblem();
   std::cout << "Starting slow problem... ";
   tester.TestSlowProblem();
   std::cout << "done" << std::endl;

   for (int i = 0; i < 1000; ++i)
   {
      std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
      tester.TestRandomCase();
      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

      std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
      std::cout << time_span.count() << std::endl;
   }
}

