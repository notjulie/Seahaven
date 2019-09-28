//
// Seahaven Solver project
//
//    Author: Randy Rasmussen
//    Copyright: None
//    Warranty: At your own risk
//


#include "SolverIncludes.h"
#include "StackPointer.h"


namespace solver {

   /// <summary>
   /// Initializes a new instance of class StackPointer.
   /// </summary>
   /// <param name="valueToPush">the item that is to become the new top of the stack</param>
   /// <param name="_previousStackPointer">link to the previous top</param>
   StackPointer::StackPointer(SolverState* valueToPush, StackPointer* _previousStackPointer)
      :
         previousStackPointer(_previousStackPointer),
         topValue(valueToPush),
         index(_previousStackPointer==nullptr ? 0 : _previousStackPointer->index + 1)
   {
   }


   /// <summary>
   /// Gets an item from the stack at the given index (index 0 -> bottom of the stack)
   /// </summary>
   const SolverState& StackPointer::GetStackEntry(int i) const
   {
      if (i == index)
         return *this->topValue;
      else
         return previousStackPointer->GetStackEntry(i);
   }

}

