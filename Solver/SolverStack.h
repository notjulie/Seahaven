
#ifndef SOLVERSTACK_H
#define SOLVERSTACK_H

#include "SolverState.h"

class SolverStack {
public:
   int GetSize(void) const { return (int)stack.size(); }
   bool IsEmpty(void) const { return stack.empty(); }
   void SetSize(int size) { stack.resize(size); }

   inline SolverState& operator[](int i) { return stack[i]; }
   inline const SolverState& operator[](int i) const { return stack[i]; }

private:
   std::vector<SolverState>   stack;
};

class StackPointer {
public:
   StackPointer(SolverState *valueToPush, StackPointer * previousStackPointer);

   int GetIndex(void) const { return previousStackPointer != nullptr ? 1 + previousStackPointer->GetIndex() : 0; }

   inline SolverState& operator*(void) { return *topValue; }
   inline SolverState* operator->(void) { return topValue; }

   const SolverState& GetStackEntry(int i) const;

private:
   const StackPointer *previousStackPointer;
   SolverState *topValue;
};


#endif
