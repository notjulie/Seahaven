/* 
 * File:   SolverState.cpp
 * Author: Randy
 * 
 * Created on April 2, 2014, 10:34 PM
 */

#include "SolverIncludes.h"

#include "SeahavenProblem.h"
#include "SolverException.h"
#include "SolverHashCode.h"

#include "SolverState.h"

SolverState::SolverState(void)
{
   Clear();
}


SolverState::SolverState(const SeahavenProblem &problem) {
   // neutral state
   Clear();

   // go through all the columns
   for (int columnIndex = 0; columnIndex < 10; ++columnIndex) {
      std::vector<CompactedColumnCard> compactedCards = problem.GetCompactedColumn(columnIndex);
      
      for (int j=0; j<compactedCards.size(); ++j)
      {
         // get the links
         LinkID highLink = problem.GetCardLinkID(
            compactedCards[j].topCard.GetSuit(),
            compactedCards[j].topCard.GetRank() + 1
            );
         LinkID lowLink = problem.GetCardLinkID(
            compactedCards[j].topCard.GetSuit(),
            compactedCards[j].topCard.GetRank() - compactedCards[j].cardCount
            );
         
         // add it
         cards.SetCard(
            cards.GetColumnLinkID(columnIndex, j),
            lowLink,
            highLink,
            compactedCards[j].cardCount
         );
         columnCounts.Increment(columnIndex);
      }
   }
   
   // and the towers
   for (int i=0; i<4; ++i)
   {
      ProblemCard tower = problem.GetTower(i);
      if (!tower.IsNull())
      {
         LinkID highLink = problem.GetCardLinkID(
            tower.GetSuit(),
            tower.GetRank() + 1
            );
         LinkID lowLink = problem.GetCardLinkID(
            tower.GetSuit(),
            tower.GetRank() - 1
            );
         
         // add it
         cards.SetCard(
            cards.GetTowerLinkID(i),
            lowLink,
            highLink,
            1
         );
      }
   }

   // the above doesn't account for the cards on the aces, although it
   // links to them; we just need to adjust the sizes
   cards.SetAceSizes();
}


bool SolverState::CanMoveColumnToTower(int columnIndex) const
{
   // never mind if the column is empty
   if (columnCounts.Get(columnIndex) == 0)
      return false;
   
   // get the card on the bottom of the column
   LinkedCard  card = cards.GetColumnCard(columnIndex, columnCounts.Get(columnIndex) - 1);
   
   // all good as long as we have the tower space
   return card.size <= cards.GetEmptyTowers();
}


/// <summary>
/// Carries out the move defined by the SolverMove object
/// </summary>
void SolverState::PerformMove(SolverMove move)
{
   switch (move.type)
   {
   case SolverMoveType::FromColumnToHigherCard:
      MoveColumnToColumnOrThrone(move.column);
      break;

   case SolverMoveType::FromColumnToTower:
      MoveColumnToTower(move.column);
      break;

   case SolverMoveType::FromTowerToEmptyThrone:
      MoveTowerToThrone(move.suit);
      break;

   default:
      throw SolverException("SolverState:PerformMove: unrecognized move type");
   }

   movePerformed = move;
}


void SolverState::MoveColumnToColumnOrThrone(int columnIndex)
{
   // move it and decrement the column count
   columnCounts.Decrement(columnIndex);
   LinkID   link = cards.GetColumnLinkID(columnIndex, columnCounts.Get(columnIndex));
   cards.MoveToHigher(link);
}


void SolverState::MoveColumnToTower(int columnIndex)
{
   // move it and decrement the column count
   columnCounts.Decrement(columnIndex);
   LinkID   link = cards.GetColumnLinkID(columnIndex, columnCounts.Get(columnIndex));
   cards.MoveToOpenTower(link);
}

void SolverState::MoveTowerToThrone(Suit suit)
{
   // get the throne
   LinkedCard throne = cards.GetThrone(suit);

   // and just move its lower to it
   cards.MoveToHigher(throne.toLower.GetLinkID());
}

ProblemCard SolverState::GetBottomColumnCardDetails(int columnIndex) const
{
   LinkID   link = cards.GetColumnLinkID(columnIndex, columnCounts.Get(columnIndex)-1);
   return cards.GetCardDetails(link);
}


bool SolverState::DoFreeMoves(void)
{
   bool didFreeMoves = false;

   // try moving to the aces repeatedly until nothing happens
   for (;;)
   {
      bool acesMoved = false;
      for (Suit suit=Suit::First; suit<=Suit::Last; ++suit)
      {
         LinkedCard ace = cards.GetAce(suit);
         CardLocation nextAceCard = ace.toHigher;
         if (nextAceCard.IsTower() || nextAceCard.IsThrone())
         {
            cards.MoveToLower(nextAceCard.GetLinkID());
            acesMoved = true;
            continue;
         }
         
         int column = nextAceCard.GetColumnIndex();
         if (column>=0 && column<=9)
         {
            int row = nextAceCard.GetRowIndex();
            if (row+1 == columnCounts.Get(column))
            {
               cards.MoveToLower(nextAceCard.GetLinkID());
               columnCounts.Decrement(column);
               acesMoved = true;
            }
         }
      }
      
      if (!acesMoved)
         break;
      didFreeMoves = true;
   }
   
   // the next thing to do is to combine cards on towers with columns or thrones or towers
   for (int i=0; i<4; ++i)
   {
      // grab the tower
      LinkedCard tower = cards.GetTower(i);
      if (tower.size == 0)
         continue;

      // if its higher link is the bottom card on a column we can combine them
      bool canMoveToHigher = false;
      if (IsBottomColumnCard(tower.toHigher))
         canMoveToHigher = true;

      // if its higher link is a non-empty throne (i.e. a king that has been put on
      // an empty column) we can likewise move it there
      if (tower.toHigher.IsThrone())
         if (GetCard(tower.toHigher).size != 0)
            canMoveToHigher = true;

      // and if its higher is on a tower we can also combine the tower cards
      if (tower.toHigher.IsTower())
         canMoveToHigher = true;

      // combine with the next higher card if we can
      if (canMoveToHigher)
      {
         cards.MoveToHigher(LinkedCards::GetTowerLinkID(i));
         didFreeMoves = true;
      }
   }

   // any kings that are the only card on a column get moved to their throne, since
   // the thrones are invented for exactly that purpose
   for (Suit suit = Suit::First; suit <= Suit::Last; ++suit)
   {
      // get the throne, make sure that it's empty
      LinkedCard throne = cards.GetThrone(suit);
      if (throne.size != 0)
         continue;

      // get the king, make sure that it's on a column
      CardLocation kingLocation = throne.toLower;
      int column = kingLocation.GetColumnIndex();
      if (column < 0)
         continue;

      // if it is the only card on the column, move it to the throne
      if (columnCounts.Get(column) == 1)
      {
         cards.MoveToHigher(kingLocation.GetLinkID());
         columnCounts.Decrement(column);
         didFreeMoves = true;
      }
   }

   // if we have kings on the towers and enough empty columns to hold them all,
   // then they all get moved to their thrones
   int kingsOnTowers = cards.CountKingsOnTowers();
   if (kingsOnTowers > 0 && GetEmptyColumnCount() >= kingsOnTowers)
   {
      for (Suit suit = Suit::First; suit <= Suit::Last; ++suit)
      {
         LinkedCard throne = cards.GetThrone(suit);
         if (throne.size == 0)
         {
            if (throne.toLower.IsTower())
            {
               cards.MoveToHigher(throne.toLower.GetLinkID());
               didFreeMoves = true;
            }
         }
      }
   }

   return didFreeMoves;
}

bool SolverState::IsBottomColumnCard(CardLocation cardLocation) const
{
   int column = cardLocation.GetColumnIndex();
   if (column<0 || column>9)
      return false;
   int row = cardLocation.GetRowIndex();
   return (row+1 == columnCounts.Get(column));
}

bool SolverState::IsVictory(void) const
{
   for (int i=0; i<10; ++i)
      if (columnCounts.Get(i) != 0)
         return false;
   
   return true;
}


void SolverState::Clear(void)
{
   columnCounts.Clear();
   cards.Clear();
}


int SolverState::GetEmptyColumnCount(void) const
{
   // first we check the columns that are empty
   int emptyColumns = columnCounts.GetEmptyCount();

   // Thrones are basically places that kings sit when there are empty columns, since
   // there's no effective difference between a king on column 1 or column 7.  So
   // subtract one from the count for any non-empty throne.
   for (Suit suit = Suit::First; suit <= Suit::Last; ++suit)
      if (cards.GetThrone(suit).size != 0)
         --emptyColumns;

   // done
   return emptyColumns;
}


SolverHashCode SolverState::GetHashValue(void) const
{
   return SolverHashCode(
      columnCounts.GetPointer(),
      cards.GetThroneOccupationMask()
      );
}


void SolverState::LockThrone(Suit suit)
{
   throw SolverException("SolverState::LockThrone: not implemented");
}


LinkedCard SolverState::GetColumnBottomCard(int column) const
{
   int row = columnCounts.Get(column) - 1;
   if (row >= 0)
      return cards.GetColumnCard(column, row);

   LinkedCard result;
   result.size = 0;
   result.toHigher.SetLinkID(LinkID::NO_LINK);
   result.toLower.SetLinkID(LinkID::NO_LINK);
   return result;
}

