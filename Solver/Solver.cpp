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

   // solve recursively
   SolverStep(stackPointer);

   // return the solution
   return result.CreateSolution();

}


/// <summary>
/// Tries to solve (recursively) the problem at the given stack index.  On entry it is expected that
/// auto moves have been performed.
/// </summary>
void Solver::SolverStep(StackPointer stackPointer)
{
   // If we have kings on towers and empty columns to put them in, all we can do
   // on this go around is to rotate through the kings and see which ones should get
   // pulled down.  Note that if there were enough empty columns, DoFreeMoves will already
   // have covered that case.  Our job is to take care of arbitration when the empty
   // columns are a scarce resource.
   int kingsOnTowers = stackPointer->CountKingsOnTowers();
   if (kingsOnTowers > 0)
   {
      int emptyColumns = stackPointer->GetEmptyColumnCount();
      if (emptyColumns > 0)
      {
         for (int tower = 0; tower < 4; ++tower)
            TryPushTowerToThroneAndSolve(stackPointer, tower);
         return;
      }
   }

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
      result.SetResultStack(stateStack.GetRange(0, stackPointer.GetIndex() + 1));
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

      // try the move
      TryPushColumnToHigherAndSolve(stackPointer, columnIndex);
      return;
   }

   // if the king is currently on a tower then moving it to a column/throne is easy
   if (kingLocation.IsTower())
   {
      SolverMove move;
      move.type = SolverMoveType::FromTowerToEmptyThrone;
      move.suit = suit;
      if (PushCurrentStateAndPerformMove(stackPointer, move))
         DoFreeMovesAndSolve(stackPointer);
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

   // move
   SolverMove move;
   move.type = SolverMoveType::FromColumnToTower;
   move.column = sourceColumn;
   return PushCurrentStateAndPerformMove(stackPointer, move);
}


void Solver::TryMovingACardToColumn(StackPointer stackPointer, int targetColumn)
{
   // find the source column
   LinkedCard targetCard = stackPointer->GetCard(stackPointer->EndOfColumn(targetColumn));
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
   // get the card
   LinkedCard card = stackPointer->GetCard(stackPointer->EndOfColumn(column));

   // we need to have enough towers
   if (card.size > stackPointer->GetFreeTowers() + 1)
      return false;

   // check if we can do the move
   if (card.toHigher.IsThrone())
   {
      LinkedCard throne = stackPointer->GetCard(card.toHigher);

      // If the throne is not empty, that means we are basically pointing at a king
      // already on a column, making this a simple column to column move.  However, if
      // it is empty, we are moving a king to an empty column, in which case we need to
      // have an empty column available.
      if (throne.size == 0)
         if (stackPointer->GetEmptyColumnCount() <= 0)
            return false;
   }
   else if (card.toHigher.IsOnColumn())
   {
      // else we can only do this if the higher card is the bottom card on a column
      int column = card.toHigher.GetColumnIndex();
      if (stackPointer->GetColumnCardCount(column) != card.toHigher.GetRowIndex() + 1)
         return false;
   }
   else
   {
      return false;
   }

   // move
   SolverMove move;
   move.type = SolverMoveType::FromColumnToHigherCard;
   move.column = column;
   if (!PushCurrentStateAndPerformMove(stackPointer, move))
      return false;
   DoFreeMovesAndSolve(stackPointer);
   return true;
}


void Solver::TryPushTowerToThroneAndSolve(StackPointer stackPointer, int towerIndex)
{
   // get the tower
   LinkedCard towerCard = stackPointer->GetTower(towerIndex);

   // empty towers of course mean nothing
   if (towerCard.size == 0)
      return;

   // if it's not pointing up at a throne never mind
   if (!towerCard.toHigher.IsThrone())
      return;

   // move
   SolverMove move;
   move.type = SolverMoveType::FromTowerToEmptyThrone;
   move.suit = towerCard.toHigher.GetSuit();
   if (!PushCurrentStateAndPerformMove(stackPointer, move))
      return;

   // In order to make sure that this move serves a purpose, we need to lock the throne
   stackPointer->LockThrone(move.suit);

   // continue
   DoFreeMovesAndSolve(stackPointer);
}


bool Solver::PushCurrentStateAndPerformMove(StackPointer &stackPointer, SolverMove move)
{
   // if we already have a solution we aren't allowed to do any moves unless it
   // might still result in a solution shorter than the one we already have
   int currentResultSize = result.GetSize();
   if (currentResultSize > 0)
   {
      // just so I don't get into any one-off confusion, let's do this carefully...
      // the maximum result size that we will allow is one less than the one we already have
      int maxResultSize = currentResultSize - 1;

      // current stack size is...
      int currentStackSize = stackPointer.GetIndex() + 1;

      // after this move it will be
      int nextStackSize = currentStackSize + 1;

      // and we insist that it be less
      if (nextStackSize >= maxResultSize)
         return false;
   }

   // go ahead and do the move
   stackPointer.PushCurrentStateAndPerformMove(move);
   return true;
}

