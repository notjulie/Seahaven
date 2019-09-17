
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

