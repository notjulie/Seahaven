
#include "SolverIncludes.h"
#include "SolverStack.h"


/// <summary>
/// Pushes the current state on to the stack and performs the given move
/// </summary>
void StackPointer::PushCurrentStateAndPerformMove(SolverMove move)
{
   // push
   stack[index + 1] = stack[index];
   ++index;
   stack.IncrementPushCount();

   // perform the move
   stack[index].PerformMove(move);
}


SolverStack SolverStack::GetRange(int first, int size)
{
   SolverStack result;
   result.SetSize(size);
   for (int i = 0; i < size; ++i)
      result[i] = (*this)[first + i];
   return result;
}
