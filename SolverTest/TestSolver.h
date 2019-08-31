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
   void TestCache(void);
   void TestRandomCase(void);
   void testSingleColumnToColumnProblem() { doSolverTestCase(singleColumnToColumnProblem, singleColumnToColumnSolution); }
   void testSingleColumnToTowerProblem() { doSolverTestCase(singleColumnToTowerProblem, singleColumnToTowerSolution); }
   void TestScottsProblem(void) { doSolverTestCase(scottsProblem, Solution::Fail()); }
   void testZeroMoveProblem() { doSolverTestCase(zeroMoveProblem, zeroMoveSolution); }

private:
   void doSolverTestCase(const SeahavenProblem &problem, const Solution &expectedResult);
};

#endif	/* TESTSOLVER_H */

