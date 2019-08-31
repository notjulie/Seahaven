// SolverTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "TestSolver.h"

int main()
{
   TestSolver tester;

   tester.TestCache();
   tester.testSingleColumnToColumnProblem();
   tester.testSingleColumnToTowerProblem();
   tester.testZeroMoveProblem();

   tester.TestScottsProblem();
}

