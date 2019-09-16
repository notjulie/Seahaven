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
}

Solution Solver::Solve(const SeahavenProblem& problem)
{
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
   {
      Solution solution = Solution::Fail();
      solution.SetTotalBranchesTested(stateStack.GetTotalPushCount());
      return solution;
   }

   // else put together a solution from the changing states
   Solution solution;
   for (int i = 1; i < resultStack.GetSize(); ++i)
   {
      SolverMove move = resultStack[i].GetMoveThatWasPerformed();
      switch (move.type)
      {
      case SolverMoveType::FromColumnToHigherCard:
      case SolverMoveType::FromColumnToTower:
         solution.AddStep(resultStack[i - 1].GetBottomColumnCardDetails(move.column));
         break;

      default:
         throw SolverException("Solver::Solve: unrecognized move type");
      }
   }

   solution.SetTotalBranchesTested(stateStack.GetTotalPushCount());
   return solution;
}


/// <summary>
/// Tries to solve (recursively) the problem at the given stack index.  On entry it is expected that
/// auto moves have been performed.
/// </summary>
void Solver::SolverStep(StackPointer stackPointer)
{
   // go through all columns and try to find moves that have some evident purpose
   for (int column = 0; column < 10; ++column)
      TryColumnMoves(stackPointer, column);

   // TODO... try moving kings from columns to towers
}


/// <summary>
/// Tries solving the problem assuming that the next move will be from the given column
/// </summary>
void Solver::TryColumnMoves(StackPointer stackPointer, int column)
{
   // Our goal is to find a move that has a purpose.  We move cards off the column until
   // we expose a card that serves some particular purpose.  Details as we go along...

   // right away we know we should exit if the column is empty
   if (stackPointer->GetColumnCardCount(column) == 0)
      return;

   // move cards off the column until we do something that seems to have a purpose
   for (;;)
   {
      // if this is the last card on the column and we don't have any empty columns,
      // then the purpose of this move is to empty a column in order to move a king
      // to the column
      if (stackPointer->GetColumnCardCount(column) == 1)
      {
         TryMoveLastCardFromColumn(stackPointer, column);
         return;
      }

      // if the next card on the column can move to the card a rank above it, that counts as a
      // move that has a purpose, so we try that and exit
      if (TryPushColumnToHigherAndSolve(stackPointer, column))
         return;

      // if the next card can't move to a tower, then we are done here
      if (!TryPushColumnToTowerMove(stackPointer, column))
         return;

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
/// Moving the last card from a column would only be useful if we intend to
/// move a king to that column.  This pursues that branch of moves for the
/// given column.
/// </summary>
void Solver::TryMoveLastCardFromColumn(StackPointer stackPointer, int column)
{
   // if we already have empty columns then this would not be a productive move...
   // we've followed this sequence as far as we can
   if (stackPointer->GetEmptyColumnCount() != 0)
      return;

   // push the card to a tower; if we can't this is as far as we can go down this road
   if (!TryPushColumnToTowerMove(stackPointer, column))
      return;

   // try all possibilities for the next move
   for (Suit suit = Suit::First; suit <= Suit::Last; ++suit)
      TryMoveKingToColumn(stackPointer, suit);
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


/// <summary>
/// Called immediately after removing the last card from a column, our job is to
/// find the given king and move it to the column.
/// </summary>
void Solver::TryMoveKingToColumn(StackPointer stackPointer, Suit suit)
{
   // find the throne, which is the link that points downward to the king
   LinkedCard throne = stackPointer->GetThrone(suit);

   // a throne acts as a column that the king can be moved to, so if the throne's
   // size is not zero it means that this king is already on a column
   if (throne.size != 0)
      return;

   // else the throne just points to the king
   CardLocation kingLocation = throne.toLower;

   // handle the case where the king is on a column 
   int columnIndex = kingLocation.GetColumnIndex();
   if (columnIndex >= 0)
   {
      // we are trying to move the king to an empty column; if it's already
      // the top card in the column that it's in there's no benefit to moving it
      // to a different column
      int rowIndex = kingLocation.GetRowIndex();
      if (rowIndex == 0)
         return;

      // push any cards on top of the king to a tower
      while (stackPointer->GetColumnCardCount(columnIndex) > rowIndex + 1)
         if (!TryPushColumnToTowerMove(stackPointer, columnIndex))
            return;

      // If we have enough towers left we can move the king to its throne.  Note that
      // I'm intentionally not checking free moves yet.  If we need to do free moves to
      // pull this off then that means we have some other move that has to happen first.
      LinkedCard king = stackPointer->GetColumnBottomCard(columnIndex);
      if (king.size > stackPointer->GetFreeTowers() + 1)
         return;

      // go
      stackPointer.PushCurrentState();

      SolverMove move;
      move.type = SolverMoveType::ToThrone;
      move.suit = kingLocation.GetSuit();
      stackPointer->PerformMove(move);
      DoFreeMovesAndSolve(stackPointer);
      return;
   }

   // if the king is currently on a tower then moving it to a column/throne is easy
   if (kingLocation.IsTower())
   {
      throw SolverException("TryMoveKingToColumn not supported for tower king");
   }
}

/// <summary>
/// Attempts to move the bottom card on the given column to a tower; if it
/// can it will push a new state onto the stack
/// </summary>
bool Solver::TryPushColumnToTowerMove(StackPointer &stackPointer, int sourceColumn)
{
   // else we just blast the card to a tower... if we can't then we're done
   if (!stackPointer->CanMoveColumnToTower(sourceColumn))
      return false;

   // push
   stackPointer.PushCurrentState();

   // move
   SolverMove move;
   move.type = SolverMoveType::FromColumnToTower;
   move.column = sourceColumn;
   stackPointer->PerformMove(move);

   // done
   return true;
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
         TryPushColumnToHigherAndSolve(stackPointer, sourceColumn);
         return;
      }

      // else we just blast the card to a tower... if we can't then we're done
      if (!TryPushColumnToTowerMove(stackPointer, sourceColumn))
         return;

      // continue until we get to the card we wanted
   }
}


bool Solver::TryPushColumnToHigherAndSolve(StackPointer& stackPointer, int column)
{
   if (!stackPointer->CanMoveColumnToColumn(column))
      return false;

   // push
   stackPointer.PushCurrentState();

   // move
   SolverMove move;
   move.type = SolverMoveType::FromColumnToHigherCard;
   move.column = column;
   stackPointer->PerformMove(move);
   DoFreeMovesAndSolve(stackPointer);
   return true;
}
