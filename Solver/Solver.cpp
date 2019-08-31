/* 
 * File:   Solver.cpp
 * Author: Randy
 * 
 * Created on April 2, 2014, 2:18 PM
 */

#include "SolverIncludes.h"

#include "Solution.h"
#include "SolverException.h"
#include "SolverHashCode.h"

#include "Solver.h"

Solver::Solver() {
   // clear
   totalSteps = 0;
   
   // allocate the stack
   stateStack.resize(100);
}

Solution Solver::Solve(const SeahavenProblem &problem)
{
    // create the initial state
    stateStack[0] = SolverState(problem);
    
    // start the process of recursively solving
    totalSteps = 0;
    SolverStep(0);
    
    // if no solution was found...
    if (resultStack.empty())
       return Solution::Fail();
    
    // else put together a solution from the changing states
    Solution solution;
    for (int i=1; i<resultStack.size(); ++i)
    {
       solution.AddStep(resultStack[i-1].GetBottomColumnCardDetails(resultStack[i].GetColumnThatWasMoved()));
    }
    return solution;
}



void Solver::SolverStep(int currentStateIndex)
{
   // if we have a solution that is only one longer than the current state
   // then we know we can't do better; this macro is what checks that
   #define RETURN_IF_TOO_MANY_MOVES \
   if (!resultStack.empty()) \
   { \
      if (currentStateIndex + 2 >= resultStack.size()) \
         return; \
   }

   // increment our number of steps
   ++totalSteps;

   // on entry, currentStateIndex points to our starting point; however,
   // it is not expected necessarily to be clean; so we need to clean it
   // first before we decide what the next step might be
   SolverState *state = &stateStack[currentStateIndex];
   state->DoFreeMoves();
   
   // if everything is on the aces, it's a victory... if so, move the
   // current stack to the result stack and be done
   if (state->IsVictory())
   {
      resultStack = stateStack;
      resultStack.resize(currentStateIndex + 1);
      return;
   }

   // never mind if the current position is in the cache
   if (cache.TestAndSet(state->GetHashValue()))
      return;
   
   SolverState *nextState = &stateStack[currentStateIndex + 1];

   bool columnsMoved[10] = {false, false, false, false, false, false, false, false, false, false};
   
   // try all the column-to-column moves
   for (int i=0; i<10; ++i)
   {
      if (state->CanMoveColumnToColumnOrThrone(i)) {
         *nextState = *state;
         nextState->MoveColumnToColumnOrThrone(i);
         columnsMoved[i] = true;

         SolverStep(currentStateIndex + 1);
         RETURN_IF_TOO_MANY_MOVES;
      }
   }

   // try all the column-to-tower moves
   for (int i=0; i<10; ++i)
   {
      if (columnsMoved[i])
         continue;
      
      if (state->CanMoveColumnToTower(i)) {
         *nextState = *state;
         nextState->MoveColumnToTower(i);
         columnsMoved[i] = true;

         SolverStep(currentStateIndex + 1);
         RETURN_IF_TOO_MANY_MOVES;
      }
   }
   
   // all we do is exit... if we found a solution it will have been saved,
   // and we will continue on looking to see if there's a shorter one; if
   // not we'll just keep looking to find one at all
}
