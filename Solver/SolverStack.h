
#ifndef SOLVERSTACK_H
#define SOLVERSTACK_H

#include "SolverState.h"

class SolverStack {
public:
   int GetSize(void) const { return (int)stack.size(); }
   int GetTotalPushCount(void) const { return totalPushes; }
   bool IsEmpty(void) const { return stack.empty(); }
   void SetSize(int size) { stack.resize(size); }
   inline void IncrementPushCount(void) { ++totalPushes; }

   inline SolverState& operator[](int i) { return stack[i]; }
   inline const SolverState& operator[](int i) const { return stack[i]; }

private:
   std::vector<SolverState>   stack;
   int totalPushes = 0;
};

class StackPointer {
public:
   StackPointer(SolverStack& _stack)
      : stack(_stack)
   {
   }

   inline bool CanPush(void) const { return index < stack.GetSize() - 1; }
   int GetIndex(void) const { return index; }
   void PushCurrentState(void);

   inline SolverState& operator*(void) { return stack[index]; }
   inline SolverState* operator->(void) { return &stack[index]; }

private:
   SolverStack& stack;
   int index = 0;
};


#endif
