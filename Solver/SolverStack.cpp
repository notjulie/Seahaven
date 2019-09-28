
#include "SolverIncludes.h"
#include "SolverStack.h"


StackPointer::StackPointer(SolverState* valueToPush, StackPointer* previousStackPointer)
{
   this->previousStackPointer = previousStackPointer;
   this->topValue = valueToPush;
}

const SolverState& StackPointer::GetStackEntry(int i) const
{
   if (GetIndex() == i)
      return *this->topValue;
   else
      return previousStackPointer->GetStackEntry(i);
}
