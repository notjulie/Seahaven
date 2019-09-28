
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
   StackPointer(SolverStack& _stack)
      : stack(_stack)
   {
   }

   int GetIndex(void) const { return index; }
   void PushCurrentState(void);

   inline SolverState& operator*(void) { return stack[index]; }
   inline SolverState* operator->(void) { return &stack[index]; }

   const SolverState& GetStackEntry(int i) const { return stack[i]; }

private:
   SolverStack& stack;
   int index = 0;
};


#endif
