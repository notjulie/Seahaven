//
// Seahaven Solver project
//
//    Author: Randy Rasmussen
//    Copyright: None
//    Warranty: At your own risk
//


#ifndef SOLVERSTACK_H
#define SOLVERSTACK_H

#include "SolverState.h"

namespace solver {

   /// <summary>
   /// Encapsulation of the solver's stack.  The solver basically uses recursion, so this is
   /// actually just a linked list that points to objects on the system stack.  Its
   /// purpose is so that when a solution is found we can walk the stack backwards to
   /// find all the steps in the solution.
   ///
   /// NOTE: implemented as immutable
   /// </summary>
   class StackPointer {
   public:
      StackPointer(SolverState* valueToPush, StackPointer* previousStackPointer);

      inline int GetIndex(void) const { return index; }
      inline SolverState& operator*(void) { return *topValue; }
      inline SolverState* operator->(void) { return topValue; }

      const SolverState& GetStackEntry(int i) const;

   private:
      const StackPointer* const previousStackPointer;
      SolverState* const topValue;
      const int index;
   };

}

#endif
