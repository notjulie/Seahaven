
#include "SolverIncludes.h"
#include "SolverStack.h"


/// <summary>
/// Pushes a copy of the current state on to the top of the stack
/// </summary>
void StackPointer::PushCurrentState(void)
{
   // push
   stack[index + 1] = stack[index];
   ++index;
}


