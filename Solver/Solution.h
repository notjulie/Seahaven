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
    void Dump(FILE *f) const;
    SolutionStep GetStep(int i) const { return steps[i]; }
    int GetStepCount(void) const { return (int)steps.size(); }

public:
   bool operator==(const Solution&) const;
   bool operator!=(const Solution&s) const { return !operator==(s); }

public:
   static Solution Fail(void);
   
private:
   bool  success;
   std::vector<SolutionStep>  steps;
};

#endif	/* SOLUTION_H */

