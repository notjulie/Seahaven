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
}

Solution Solver::Solve(const SeahavenProblem& problem)
{
   // clear
   totalSteps = 0;

   // allocate the stack
   stateStack.SetSize(100);

   // create the initial state
   StackPointer stackPointer = StackPointer(stateStack);
   *stackPointer = SolverState(problem);

   // and make sure that it's a clean state with no free moves waiting to be
   // done
   switch (DoFreeMoves(stackPointer))
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
   SolverStep(stackPointer);

   // if no solution was found...
   if (resultStack.IsEmpty())
      return Solution::Fail();

   // else put together a solution from the changing states
   Solution solution;
   for (int i = 1; i < resultStack.GetSize(); ++i)
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
void Solver::SolverStep(StackPointer stackPointer)
{
   for (int column = 0; column < 10; ++column)
      TryColumnMoves(stackPointer, column);
}


/// <summary>
/// Tries solving the problem assuming that the next move will be from the given column
/// </summary>
void Solver::TryColumnMoves(StackPointer stackPointer, int column)
{
   SolverMove move;

   // Our goal is to find a move that has a purpose.  We move cards off the column until
   // we expose a card that serves some particular purpose.  Details as we go along...

   // right away we know we should exit if the column is empty
   if (stackPointer->GetColumnCardCount(column) == 0)
      return;

   // move cards off the column until we do something that seems to have a purpose
   for (;;)
   {
      // if the next card on the column can move to a column, that counts as a
      // move that has a purpose, so we try that and exit
      if (stackPointer->CanMoveColumnToColumn(column))
      {
         // push
         stackPointer.PushCurrentState();

         // move
         move.type = SolverMoveType::FromColumnToColumn;
         move.column = column;
         stackPointer->PerformMove(move);
         DoFreeMovesAndSolve(stackPointer);
         return;
      }

      // if the next card can't move to a tower, then we are done here
      if (!stackPointer->CanMoveColumnToTower(column))
         return;

      // push
      stackPointer.PushCurrentState();

      // move
      move.type = SolverMoveType::FromColumnToTower;
      move.column = column;
      stackPointer->PerformMove(move);

      // if that empties out the column, the only purpose that can serve is if we
      // move a king to an empty column
      if (stackPointer->GetColumnCardCount(column) == 0)
      {
         TryMoveAnyKingToColumn(stackPointer);
         return;
      }

      // see if we can do any free moves
      switch (DoFreeMoves(stackPointer))
      {
      case DidFreeMoves:
         // if we did free moves that means that this move served a purpose so
         // continue our recursive search
         SolverStep(stackPointer);
         return;

      case DidNothing:
         // try the branch where we move a card to this column
         TryMovingACardToColumn(stackPointer, column);

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
void Solver::TryMove(StackPointer stackPointer, SolverMove move)
{
   // if our new stack size after we push another state will be as large as a
   // previous solution, don't bother continuing... if we already have a solution
   // the only thing we would be interested in would be a shorter solution
   int currentSolutionSize = (int)resultStack.GetSize();
   if (currentSolutionSize != 0)
   {
      int newSolutionMinimumSize = stackPointer.GetIndex() + 2;
      if (newSolutionMinimumSize >= currentSolutionSize)
         return;
   }

   // push a new copy of the current state
   stackPointer.PushCurrentState();

   // do the move
   stackPointer->PerformMove(move);

   // do any resulting free moves
   switch (DoFreeMoves(stackPointer))
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
   SolverStep(stackPointer);
}

/// <summary>
/// Performs free moves to the aces and combinations of cards; this is
/// called after performing a move to clean up the state of the game.
/// On completion it returns:
///    Victory: the game is won
///    DeadEnd: we've already tried this position before; don't continue
///    Normal: no surprises, continue solving
/// </summary>
Solver::FreeMovesResult Solver::DoFreeMoves(StackPointer stackPointer)
{
   bool didFreeMoves = stackPointer->DoFreeMoves();

   // if everything is on the aces, it's a victory... if so, move the
   // current stack to the result stack and be done
   if (stackPointer->IsVictory())
   {
      resultStack = stateStack;
      resultStack.SetSize((size_t)stackPointer.GetIndex() + 1);
      return Victory;
   }

   // never mind if the current position is in the cache
   if (cache.TestAndSet(stackPointer->GetHashValue()))
      return DeadEnd;

   return didFreeMoves ? DidFreeMoves : DidNothing;
}


void Solver::DoFreeMovesAndSolve(StackPointer stackPointer)
{
   switch (DoFreeMoves(stackPointer))
   {
   case DidFreeMoves:
   case DidNothing:
      SolverStep(stackPointer);
      break;

   case Victory:
   case DeadEnd:
      break;
   }
}


void Solver::TryMoveAnyKingToColumn(StackPointer stackPointer)
{
   throw SolverException("Solver::TryMoveAnyKingToColumn not implemented");
}

void Solver::TryMovingACardToColumn(StackPointer stackPointer, int targetColumn)
{
   // find the source column
   LinkedCard targetCard = stackPointer->GetColumnBottomCard(targetColumn);
   int sourceColumn = targetCard.toLower.GetColumnIndex();
   if (sourceColumn < 0)
      return;
   int sourceRow = targetCard.toLower.GetRowIndex();

   // move cards off the column until we move the source card or something
   // we disapprove of happens
   for (;;)
   {
      // if the card we are looking for is at the bottom of the column we just do
      // the column to column move that we intended to do
      if (stackPointer->GetColumnCardCount(sourceColumn) == sourceRow + 1)
      {
         // push
         stackPointer.PushCurrentState();

         // move
         SolverMove move;
         move.type = SolverMoveType::FromColumnToColumn;
         move.column = sourceColumn;
         stackPointer->PerformMove(move);

         // walk the tree from that point on and be done
         DoFreeMovesAndSolve(stackPointer);
         return;
      }

      // else we just blast the card to a tower... if we can't then we're done
      if (!stackPointer->CanMoveColumnToTower(sourceColumn))
         return;

      // push
      stackPointer.PushCurrentState();

      // move
      SolverMove move;
      move.type = SolverMoveType::FromColumnToTower;
      move.column = sourceColumn;
      stackPointer->PerformMove(move);

      // continue until we get to the card we wanted
   }
}

