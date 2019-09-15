/* 
 * File:   Solution.h
 * Author: Randy
 *
 * Created on April 2, 2014, 2:19 PM
 */

#ifndef SOLUTION_H
#define	SOLUTION_H

#include "SolutionStep.h"

class Solution {
public:
    Solution(
       const SolutionStep &step1 = SolutionStep()
       );

    void AddStep(const SolutionStep &step);
    void DumpToConsole(void) const;

    int GetTotalBranchesTested(void) const { return totalBranchesTested; }
    void SetTotalBranchesTested(int _totalBranchesTested) { totalBranchesTested = _totalBranchesTested; }

public:
   bool operator==(const Solution&) const;
   bool operator!=(const Solution&s) const { return !operator==(s); }

public:
   static Solution Fail(void);
   
private:
   bool  success;
   int   totalBranchesTested = 0;
   std::vector<SolutionStep>  steps;
};

#endif	/* SOLUTION_H */

