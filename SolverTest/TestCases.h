/* 
 * File:   TestCases.h
 * Author: Randy
 *
 * Created on April 7, 2014, 8:59 PM
 */

#ifndef TESTCASES_H
#define	TESTCASES_H

#include "SeahavenProblem.h"
#include "Solution.h"

using solver::ProblemDefinition;
using solver::Solution;

extern const ProblemDefinition cacheTestProblem;
extern const ProblemDefinition slowTestProblem;
extern const ProblemDefinition slowTestProblem2;
extern const ProblemDefinition scottsProblem;
extern const ProblemDefinition combineTowersProblem;

extern const ProblemDefinition zeroMoveProblem;
extern const Solution zeroMoveSolution;

extern const ProblemDefinition singleColumnToColumnProblem;
extern const Solution singleColumnToColumnSolution;

extern const ProblemDefinition singleColumnToTowerProblem;
extern const Solution singleColumnToTowerSolution;

extern const ProblemDefinition problemIRanInto;

#endif	/* TESTCASES_H */

