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
   case DidFreeMoves:
   case DidNothing:
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


/// <summary>
/// Tries to solve (recursively) the problem at the given stack index.  On entry it is expected that
/// auto moves have been performed.
/// </summary>
void Solver::SolverStep(int currentStateIndex)
{
   for (int column = 0; column < 10; ++column)
      TryColumnMoves(currentStateIndex, column);
}


/// <summary>
/// Tries solving the problem assuming that the next move will be from the given column
/// </summary>
void Solver::TryColumnMoves(int currentStateIndex, int column)
{
   SolverMove move;

   // Our goal is to find a move that has a purpose.  We move cards off the column until
   // we expose a card that serves some particular purpose.  Details as we go along...

   // point to the top of the stack
   SolverState *currentState = &stateStack[currentStateIndex];

   // right away we know we should exit if the column is empty
   if (currentState->GetColumnCardCount(column) == 0)
      return;

   // move cards off the column until we do something that seems to have a purpose
   for (;;)
   {
      // if the next card on the column can move to a column, that counts as a
      // move that has a purpose, so we try that and exit
      if (currentState->CanMoveColumnToColumn(column))
      {
         // push
         currentState[1] = currentState[0];
         ++currentState;
         ++currentStateIndex;

         // move
         move.type = SolverMoveType::FromColumnToColumn;
         move.column = column;
         currentState->PerformMove(move);
         DoFreeMovesAndSolve(currentStateIndex);
         return;
      }

      // if the next card can't move to a tower, then we are done here
      if (!currentState->CanMoveColumnToTower(column))
         return;

      // push
      currentState[1] = currentState[0];
      ++currentState;
      ++currentStateIndex;

      // move
      move.type = SolverMoveType::FromColumnToTower;
      move.column = column;
      currentState->PerformMove(move);

      // if that empties out the column, the only purpose that can serve is if we
      // move a king to an empty column
      if (currentState->GetColumnCardCount(column) == 0)
      {
         TryMoveAnyKingToColumn(currentStateIndex);
         return;
      }

      // see if we can do any free moves
      switch (DoFreeMoves(currentStateIndex))
      {
      case DidFreeMoves:
         // if we did free moves that means that this move served a purpose so
         // continue our recursive search
         SolverStep(currentStateIndex);
         return;

      case DidNothing:
         // try the branch where we move a card to this column
         TryMovingACardToColumn(currentStateIndex, column);

         // and after checking that branch, continue trying to move cards off this column
         break;

      case Victory:
      case DeadEnd:
         // in either of these cases we are done
         return;
      }
   }
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
   case DidFreeMoves:
   case DidNothing:
      break;
   case Victory:
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
   bool didFreeMoves = state->DoFreeMoves();

   // if everything is on the aces, it's a victory... if so, move the
   // current stack to the result stack and be done
   if (state->IsVictory())
   {
      resultStack = stateStack;
      resultStack.resize((size_t)currentStateIndex + 1);
      return Victory;
   }

   // never mind if the current position is in the cache
   if (cache.TestAndSet(state->GetHashValue()))
      return DeadEnd;

   return didFreeMoves ? DidFreeMoves : DidNothing;
}


void Solver::DoFreeMovesAndSolve(int currentStateIndex)
{
   switch (DoFreeMoves(currentStateIndex))
   {
   case DidFreeMoves:
   case DidNothing:
      SolverStep(currentStateIndex);
      break;

   case Victory:
   case DeadEnd:
      break;
   }
}


void Solver::TryMoveAnyKingToColumn(int currentStateIndex)
{
   throw SolverException("Solver::TryMoveAnyKingToColumn not implemented");
}

void Solver::TryMovingACardToColumn(int currentStateIndex, int targetColumn)
{
   // find the source column
   SolverState* currentState = &stateStack[currentStateIndex];
   LinkedCard targetCard = currentState->GetColumnBottomCard(targetColumn);
   int sourceColumn = LinkedCards::GetColumnIndex(targetCard.toLower);
   if (sourceColumn < 0)
      return;
   int sourceRow = LinkedCards::GetRowIndex(targetCard.toLower);

   // move cards off the column until we move the source card or something
   // we disapprove of happens
   for (;;)
   {
      // if the card we are looking for is at the bottom of the column we just do
      // the column to column move that we intended to do
      if (currentState->GetColumnCardCount(sourceColumn) == sourceRow + 1)
      {
         // push
         currentState[1] = currentState[0];
         ++currentState;
         ++currentStateIndex;

         // move
         SolverMove move;
         move.type = SolverMoveType::FromColumnToColumn;
         move.column = sourceColumn;
         currentState->PerformMove(move);

         // walk the tree from that point on and be done
         DoFreeMovesAndSolve(currentStateIndex);
         return;
      }

      // else we just blast the card to a tower... if we can't then we're done
      if (!currentState->CanMoveColumnToTower(sourceColumn))
         return;

      // push
      currentState[1] = currentState[0];
      ++currentState;
      ++currentStateIndex;

      // move
      SolverMove move;
      move.type = SolverMoveType::FromColumnToTower;
      move.column = sourceColumn;
      currentState->PerformMove(move);

      // continue until we get to the card we wanted
   }
}

