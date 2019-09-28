
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
   int GetNumberOfThroneToTowerMoves(void) const;

   void CopyFromStack(StackPointer stackPointer);

private:
   SolverStack resultStack;
};


#endif
