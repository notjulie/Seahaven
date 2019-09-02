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

Solution Solver::Solve(const SeahavenProblem& problem)
{
   // clear
   totalSteps = 0;

   // create the initial state
   stateStack[0] = SolverState(problem);

   // and make sure that it's a clean state with no free moves waiting to be
   // done
   switch (DoFreeMoves(0))
   {
   case Victory:
      return Solution();
   case Normal:
      break;
   default:
      throw SolverException("Solver::Solve: unexpected result from DoFreeMoves");
   }

   // start the process of recursively solving
   SolverStep(0);

   // if no solution was found...
   if (resultStack.empty())
      return Solution::Fail();

   // else put together a solution from the changing states
   Solution solution;
   for (int i = 1; i < resultStack.size(); ++i)
   {
      SolverMove move = resultStack[i].GetMoveThatWasPerformed();
      switch (move.type)
      {
      case SolverMoveType::FromColumnToColumn:
      case SolverMoveType::FromColumnToTower:
         solution.AddStep(resultStack[i - 1].GetBottomColumnCardDetails(move.column));
         break;

      default:
         throw SolverException("Solver::Solve: unrecognized move type");
      }
   }
   return solution;
}



void Solver::SolverStep(int currentStateIndex)
{
   SolverMove move;
   
   // increment our number of steps
   ++totalSteps;

   SolverState* state = &stateStack[currentStateIndex];

   bool columnsMoved[10] = { false, false, false, false, false, false, false, false, false, false };

   // try all the column-to-column moves
   for (int i = 0; i < 10; ++i)
   {
      if (state->CanMoveColumnToColumn(i)) {
         move.type = SolverMoveType::FromColumnToColumn;
         move.column = i;
         TryMove(currentStateIndex, move);
         columnsMoved[i] = true;
      }
   }

   // try all the column-to-tower moves
   for (int i = 0; i < 10; ++i)
   {
      if (columnsMoved[i])
         continue;

      if (state->CanMoveColumnToTower(i)) {
         move.type = SolverMoveType::FromColumnToTower;
         move.column = i;
         TryMove(currentStateIndex, move);
      }
   }

   // all we do is exit... if we found a solution it will have been saved,
   // and we will continue on looking to see if there's a shorter one; if
   // not we'll just keep looking to find one at all
}


/// <summary>
/// Performs the given move and recursively determines if it results in a
/// better solution than any we currently have.
/// </summary>
void Solver::TryMove(int currentStateIndex, SolverMove move)
{
   // if our new stack size after we push another state will be as large as a
   // previous solution, don't bother continuing... if we already have a solution
   // the only thing we would be interested in would be a shorter solution
   int currentSolutionSize = (int)resultStack.size();
   if (currentSolutionSize != 0)
   {
      int newSolutionMinimumSize = currentStateIndex + 2;
      if (newSolutionMinimumSize >= currentSolutionSize)
         return;
   }

   // push a new copy of the current state
   SolverState* state = &stateStack[currentStateIndex];
   SolverState* nextState = &stateStack[(size_t)currentStateIndex + 1];
   *nextState = *state;

   // do the move
   nextState->PerformMove(move);

   // do any resulting free moves
   switch (DoFreeMoves(currentStateIndex + 1))
   {
   case Normal:
      break;
   case Victory:
      return;
   case DeadEnd:
      return;
   default:
      throw SolverException("Solver::SolverStep: unexpected DoFreeMoves result");
   }

   // recursively analyze the result
   SolverStep(currentStateIndex + 1);
}

/// <summary>
/// Performs free moves to the aces and combinations of cards; this is
/// called after performing a move to clean up the state of the game.
/// On completion it returns:
///    Victory: the game is won
///    DeadEnd: we've already tried this position before; don't continue
///    Normal: no surprises, continue solving
/// </summary>
Solver::FreeMovesResult Solver::DoFreeMoves(int currentStateIndex)
{
   SolverState* state = &stateStack[currentStateIndex];
   state->DoFreeMoves();

   // if everything is on the aces, it's a victory... if so, move the
   // current stack to the result stack and be done
   if (state->IsVictory())
   {
      resultStack = stateStack;
      resultStack.resize((int)currentStateIndex + 1);
      return Victory;
   }

   // never mind if the current position is in the cache
   if (cache.TestAndSet(state->GetHashValue()))
      return DeadEnd;

   return Normal;
}
