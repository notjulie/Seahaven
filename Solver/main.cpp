/* 
 * File:   main.cpp
 * Author: Randy
 *
 * Created on April 2, 2014, 10:32 AM
 */

#include "SolverIncludes.h"

#include "ProblemCards.h"
#include "SeahavenProblem.h"
#include "Solution.h"
#include "Solver.h"
#include "SolverException.h"
#include "TestCases.h"

using namespace std;

static void SolveProblem(const SeahavenProblem &problem);

/*
 * 
 */
int main(int argc, char** argv) {
    try {
        SolveProblem(scottsProblem);
    }
    catch (const SolverException &x) {
        printf("Error caught: %s\r\n", x.GetMessage().c_str());
    }
    catch (...) {
        printf("Unhandled exception caught");
    }        

    return 0;
}

static void SolveProblem(const SeahavenProblem &problem)
{
    // make up the problem we're trying to solve

    Solver solver;
    Solution solution = solver.Solve(problem);
    printf("Total steps: %d\r\n", solver.GetTotalSteps());
    solution.DumpToConsole();
}

