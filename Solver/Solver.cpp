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


/// <summary>
/// Initializes a new instance of class Solver.
/// </summary>
Solver::Solver(void)
{
}


/// <summary>
/// Solve... it's what we do.
/// </summary>
Solution Solver::Solve(const SeahavenProblem& problem)
{
   // create the initial state
   SolverState initialState(problem);

   // create a stack from it
   StackPointer stackPointer(&initialState, nullptr);

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
   TestAllMoves(stackPointer);

   // return the solution
   return result.CreateSolution();
}


/// <summary>
/// Tries to solve (recursively) the problem at the given stack index.  On entry it is expected that
/// auto moves have been performed.
/// </summary>
void Solver::TestAllMoves(StackPointer stackPointer)
{
   // If we have kings on towers and empty columns to put them in, all we can do
   // on this go around is to rotate through the kings and see which ones should get
   // pulled down.  Note that if there were enough empty columns, DoFreeMoves will already
   // have covered that case.  Our job is to take care of arbitration when the empty
   // columns are a scarce resource.
   if (stackPointer->CountKingsOnTowers() != 0 && stackPointer->GetEmptyColumnCount() != 0)
   {
      for (Suit suit: Suit::All)
         TryTowerToThroneAndSolve(stackPointer, suit);

      // that's all we can do on this branch... we aren't allowed to proceed until
      // our towers are clean
      return;
   }

   // go through all columns and try to find moves that have some evident purpose
   for (int column = 0; column < 10; ++column)
      TryColumnMoves(stackPointer, column);

   // try moving kings from columns to towers
   if (isThroneToTowerAllowed)
   {
      for (Suit suit : Suit::All)
         TryMoveFromThroneToTowerAndSolve(stackPointer, suit);
   }
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

   // if the card can't move to a tower, then we are done here
   if (!stackPointer->CanMoveColumnToTower(column))
      return;

   // OK, move it to tower and test that branch
   SolverMove move;
   move.type = SolverMoveType::FromColumnToTower;
   move.column = column;
   TestMove(stackPointer, move, [this, column](StackPointer stackPointer) {
         // see if we can do any free moves
         switch (DoFreeMoves(stackPointer))
         {
         case DidFreeMoves:
            // if we did free moves that means that this move served a purpose so
            // we test all directions from that point
            TestAllMoves(stackPointer);
            return;

         case DidNothing:
            // try the branch where we move a card to this column
            TryMovingACardToColumn(stackPointer, column);

            // and after checking that branch, continue trying to move cards off this column
            TryColumnMoves(stackPointer, column);
            break;

         case Victory:
         case DeadEnd:
            // in either of these cases we are done
            return;
         }
      });
}


void Solver::TryMoveFromThroneToTowerAndSolve(StackPointer stackPointer, Suit suit)
{
   // get the throne
   LinkedCard throne = stackPointer->GetCard(CardLocation::Thrones[suit.GetIndex()]);
   int vacatingKingSize = throne.size;

   // a throne is a special column reserved for a specific king; if it's empty we don't
   // have a king to move
   if (vacatingKingSize == 0)
      return;

   // We're not allowed to do this if the throne is locked.  This is a mechanism that
   // I use to prevent the infinite loop of moving the KC to the tower so that the
   // KD can be moved to its throne, reverse, repeat.  After such a move I lock the
   // throne until it is involved in some move that required it to be on the throne.
   if (stackPointer->IsThroneLocked(suit))
      return;

   // and of course we need enough tower space
   if (stackPointer->GetFreeTowers() < throne.size)
      return;

   // OK, we can do this
   SolverMove move;
   move.type = SolverMoveType::FromThroneToTower;
   move.suit = suit;
   TestMove(stackPointer, move, [this, suit, vacatingKingSize](StackPointer stackPointer) {
         // OK, the only reason to move the king we just moved from its throne to a
         // tower is to make room for another king.  So loop through the kings and see
         // if we can do that.
         for (Suit followUpMoveSuit : Suit::All)
         {
            // never mind if this is the king we just moved
            if (followUpMoveSuit == suit)
               continue;

            // grab the king's throne
            LinkedCard throne = stackPointer->GetCard(CardLocation::Thrones[followUpMoveSuit.GetIndex()]);

            // we want to move a king to the throne... never mind if it's already there
            if (throne.size != 0)
               continue;

            // if the king is on a column then we already have a function for handling that
            if (throne.toLower.onColumn)
            {
               TryMoveKingToColumnAndSolve(stackPointer, followUpMoveSuit);
               continue;
            }

            // if the king is on a tower then call this function, which deals with all the trickery
            // involved with that
            if (throne.toLower.isTower)
            {
               TryMoveKingFromTowerToColumnVacatedByADifferentKingAndSolve(stackPointer, followUpMoveSuit, vacatingKingSize);
               continue;
            }
         }
      });
}


/// <summary>
/// Called after a king is moved from an empty column to a tower.
/// </summary>
void Solver::TryMoveKingFromTowerToColumnVacatedByADifferentKingAndSolve(StackPointer stackPointer, Suit suit, int otherKingSize)
{
   // request the move
   SolverMove move;
   move.type = SolverMoveType::FromTowerToEmptyThrone;
   move.suit = suit;
   TestMove(stackPointer, move, [this, suit, otherKingSize](StackPointer stackPointer) {
         // Following the move, we need to lock the throne if its size is less than or equal to
         // the size of the king that we just moved to the tower.  The reason for this is that
         // like always we are interested only in moves that serve a purpose.  Moving a king
         // to a tower to make room for a king and a queen serves a purpose.  Moving a king to
         // a tower to make room for a king only serves a purpose if we intend to put a card on
         // that king.
         LinkedCard targetThrone = stackPointer->GetCard(CardLocation::Thrones[suit.GetIndex()]);
         if (targetThrone.size <= otherKingSize)
            stackPointer->LockThrone(suit);

         // and from that point we can do any move we want to
         DoFreeMovesAndSolve(stackPointer);
      });
}


/// <summary>
/// Moves the given king from a tower (if that's where it is) to its throne (if the throne is empty).
/// </summary>
void Solver::TryTowerToThroneAndSolve(StackPointer stackPointer, Suit suit)
{
   static int towersToThrone = 0;
   ++towersToThrone;

   // grab the throne
   LinkedCard throne = stackPointer->GetCard(CardLocation::Thrones[suit.GetIndex()]);

   // never mind if it already has something on it
   if (throne.size != 0)
      return;

   // never mind if the king it points to is not on a tower
   if (!throne.toLower.isTower)
      return;

   // move it
   SolverMove move;
   move.type = SolverMoveType::FromTowerToEmptyThrone;
   move.suit = suit;
   TestMove(stackPointer, move, [this, suit](StackPointer stackPointer) {
         // If we still have empty columns then we need to continue down this path;
         // note that by starting with the next suit we make sure that we don't do
         // redundant steps (KC then KD, next time KD then KC).
         if (stackPointer->GetEmptyColumnCount() != 0)
         {
            Suit nextSuit = suit;
            while (++nextSuit <= Suit::Last)
               TryTowerToThroneAndSolve(stackPointer, nextSuit);
            return;
         }

         // else just solve as normal
         DoFreeMovesAndSolve(stackPointer);
      });
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

   // if we can't move the card to a tower then we're done
   if (!stackPointer->CanMoveColumnToTower(column))
      return;

   // move
   SolverMove move;
   move.type = SolverMoveType::FromColumnToTower;
   move.column = column;
   TestMove(stackPointer, move, [this](StackPointer stackPointer) {
         // try all possibilities for the next move
         for (Suit suit : Suit::All)
            TryMoveKingToColumnAndSolve(stackPointer, suit);
      });
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
      // Truncate the size of the stack and copy it to the result
      result.CopyFromStack(stackPointer);

      // and note how hard we had to work to get it
      totalPushesAtTimeOfResult = totalPushCount;

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
      TestAllMoves(stackPointer);
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
void Solver::TryMoveKingToColumnAndSolve(StackPointer stackPointer, Suit suit)
{
   // find the throne, which is the link that points downward to the king
   LinkedCard throne = stackPointer->GetCard(CardLocation::Thrones[suit.GetIndex()]);

   // a throne acts as a column that the king can be moved to, so if the throne's
   // size is not zero it means that this king is already on a column
   if (throne.size != 0)
      return;

   // else the throne just points to the king
   CardLocation kingLocation = throne.toLower;

   // handle the case where the king is on a column 
   if (kingLocation.onColumn)
   {
      // we are trying to move the king to an empty column; if it's already
      // the top card in the column that it's in there's no benefit to moving it
      // to a different column
      int rowIndex = kingLocation.row;
      if (rowIndex == 0)
         return;

      // if this is not the bottom card on the column we need to remove a card and
      // try again
      if (stackPointer->GetColumnCardCount(kingLocation.column) > rowIndex + 1)
      {
         // just blast the card to a tower... if we can't then we're done
         if (!stackPointer->CanMoveColumnToTower(kingLocation.column))
            return;

         // move
         SolverMove move;
         move.type = SolverMoveType::FromColumnToTower;
         move.column = kingLocation.column;
         TestMove(stackPointer, move, [this, suit](StackPointer stackPointer) {
               TryMoveKingToColumnAndSolve(stackPointer, suit);
            });
         return;
      }

      // try the move
      TryPushColumnToHigherAndSolve(stackPointer, kingLocation.column);
      return;
   }

   // if the king is currently on a tower then moving it to a column/throne is easy
   if (kingLocation.isTower)
   {
      SolverMove move;
      move.type = SolverMoveType::FromTowerToEmptyThrone;
      move.suit = suit;
      TestMove(stackPointer, move, [this](StackPointer stackPointer) {
            DoFreeMovesAndSolve(stackPointer);
         });
   }
}


void Solver::TryMovingACardToColumn(StackPointer stackPointer, int targetColumn)
{
   // find the source column
   LinkedCard targetCard = stackPointer->GetCard(stackPointer->EndOfColumn(targetColumn));
   if (!targetCard.toLower.onColumn)
      return;
   int sourceRow = targetCard.toLower.row;
   int sourceColumn = targetCard.toLower.column;

   // if the card we are looking for is at the bottom of the column we just do
   // the column to column move that we intended to do
   if (stackPointer->GetColumnCardCount(sourceColumn) == sourceRow + 1)
   {
      TryPushColumnToHigherAndSolve(stackPointer, sourceColumn);
      return;
   }

   // else we need to blast a card from the source column to a tower... 
   // if we can't then we're done
   if (!stackPointer->CanMoveColumnToTower(sourceColumn))
      return;

   // move from the source column to the tower and call ourself back
   // recursively to finish the sequence
   SolverMove move;
   move.type = SolverMoveType::FromColumnToTower;
   move.column = sourceColumn;
   TestMove(stackPointer, move, [this, targetColumn](StackPointer stackPointer) {
         TryMovingACardToColumn(stackPointer, targetColumn);
      });
}


bool Solver::TryPushColumnToHigherAndSolve(StackPointer stackPointer, int column)
{
   // get the card
   LinkedCard card = stackPointer->GetCard(stackPointer->EndOfColumn(column));

   // we need to have enough towers
   if (card.size > stackPointer->GetFreeTowers() + 1)
      return false;

   // check if we can do the move
   if (card.toHigher.isThrone)
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
   else if (card.toHigher.onColumn)
   {
      // else we can only do this if the higher card is the bottom card on a column
      if (stackPointer->GetColumnCardCount(card.toHigher.column) != card.toHigher.row + 1)
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
   TestMove(stackPointer, move, [this, column](StackPointer stackPointer) {
         DoFreeMovesAndSolve(stackPointer);
      });
   return true;
}


/// <summary>
/// Standard mechanism for testing a move and all branches that come from it.  The sequence is:
///    - push a new state onto the stack
///    - perform the move
///    - call the nextStep function with the new stack pointer
/// </summary>
/// <param name="stackPointer">current stack pointer</param>
/// <param name="move">the move to perform</param>
/// <param name="nextStep">the sequence to carry out after performing the move</param>
void Solver::TestMove(StackPointer stackPointer, SolverMove move, const std::function<void(StackPointer)> nextStep)
{
   // if we already have a solution...
   int currentResultSize = result.GetSize();
   if (currentResultSize > 0)
   {
      // Sometimes we will get a result that's good enough that we aren't likely
      // to get a better one.  Once that happens, we can end up spinning through the
      // problem with no hope of finding a better solution.  To prevent that, I set a
      // cap on the number of pushes we can do before deciding to just accept the result
      // we already have.
      uint32_t pushesSinceLastResult = this->totalPushCount - totalPushesAtTimeOfResult;
      if (pushesSinceLastResult > 20000)
         return;

      // If we don't have space on the stack just give up... this generally just means that
      // we already found a solution and are only interested in seeing if we can find one that's
      // shorter than the one we already found.
      int maxStackSize = currentResultSize - 1;
      int currentStackSize = stackPointer.GetIndex() + 1;
      if (currentStackSize + 1 > maxStackSize)
         return;
   }

   // perform the move
   SolverState newState = *stackPointer;
   newState.PerformMove(move);

   // push to the top of the stack
   StackPointer newStackPointer(&newState, &stackPointer);
   ++totalPushCount;

   // and perform the next step on the new top of the stack
   nextStep(newStackPointer);
}
