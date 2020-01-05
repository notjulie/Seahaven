
#ifndef SOLVERRESULT_H
#define SOLVERRESULT_H

#include "Solution.h"
#include "StackPointer.h"

namespace solver {

   class SolverResult
   {
   public:
      Solution CreateSolution(void) const;

      void DumpToFile(FILE* file) const;
      int GetSize(void) const { return (int)resultStack.size(); }
      int GetNumberOfThroneToTowerMoves(void) const;

      void CopyFromStack(solver::StackPointer stackPointer);

   private:
      std::vector<SolverState> resultStack;
   };

}

#endif
