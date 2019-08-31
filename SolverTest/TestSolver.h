/*
 * File:   TestSolver.h
 * Author: Randy
 *
 * Created on Apr 7, 2014, 7:07:15 AM
 */

#ifndef TESTSOLVER_H
#define	TESTSOLVER_H

#include "TestCases.h"

class TestSolver {
public:
   TestSolver();
   virtual ~TestSolver();

public:
   void testZeroMoveProblem() { doSolverTestCase(zeroMoveProblem, zeroMoveSolution); }
   void testSingleColumnToColumnProblem() { doSolverTestCase(singleColumnToColumnProblem, singleColumnToColumnSolution); }
   void testSingleColumnToTowerProblem() { doSolverTestCase(singleColumnToTowerProblem, singleColumnToTowerSolution); }

private:
   void doSolverTestCase(const SeahavenProblem &problem, const Solution &expectedResult);
};

#endif	/* TESTSOLVER_H */

