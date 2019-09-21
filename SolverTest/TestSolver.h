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
   SeahavenProblem TestRandomCase(void);
   void testSingleColumnToColumnProblem() { doSolverTestCase(singleColumnToColumnProblem, singleColumnToColumnSolution); }
   void testSingleColumnToTowerProblem() { doSolverTestCase(singleColumnToTowerProblem, singleColumnToTowerSolution); }
   void TestScottsProblem(void);
   void TestSlowProblem(void);
   void TestSlowProblem2(void);
   void testZeroMoveProblem() { doSolverTestCase(zeroMoveProblem, zeroMoveSolution); }
   void TestCombineTowersProblem(void) { doSolverTestCase(combineTowersProblem, Solution::Fail()); }

private:
   void doSolverTestCase(const SeahavenProblem &problem, const Solution &expectedResult);
};

#endif	/* TESTSOLVER_H */

