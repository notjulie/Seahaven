
#ifndef SOLVERRESULT_H
#define SOLVERRESULT_H

#include "Solution.h"
#include "SolverStack.h"

class SolverResult
{
public:
   Solution CreateSolution(void) const;

   void DumpToFile(FILE* file) const;
   int GetSize(void) const { return resultStack.GetSize(); }

   void SetResultStack(const SolverStack& stack) { resultStack = stack; }

private:
   SolverStack resultStack;
};


#endif
