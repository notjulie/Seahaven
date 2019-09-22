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
   LinkedCard linkedCard;

   // neutral state
   Clear();

   // go through all the columns
   for (int columnIndex = 0; columnIndex < 10; ++columnIndex) {
      std::vector<CompactedColumnCard> compactedCards = problem.GetCompactedColumn(columnIndex);
      
      for (int j=0; j<compactedCards.size(); ++j)
      {
         // get the links
         CardLocation highLink = problem.GetCardLocation(
            compactedCards[j].topCard.GetSuit(),
            compactedCards[j].topCard.GetRank() + 1
            );
         CardLocation lowLink = problem.GetCardLocation(
            compactedCards[j].topCard.GetSuit(),
            compactedCards[j].topCard.GetRank() - compactedCards[j].cardCount
            );
         
         // add it
         linkedCard.toLower = lowLink;
         linkedCard.toHigher = highLink;
         linkedCard.size = compactedCards[j].cardCount;
         cards.SetCard(CardLocation::Columns[columnIndex][j], linkedCard);
         columnCounts.Increment(columnIndex);
      }
   }
   
   // and the towers
   for (int i=0; i<4; ++i)
   {
      ProblemCard tower = problem.GetTower(i);
      if (!tower.IsNull())
      {
         CardLocation highLink = problem.GetCardLocation(
            tower.GetSuit(),
            tower.GetRank() + 1
            );
         CardLocation lowLink = problem.GetCardLocation(
            tower.GetSuit(),
            tower.GetRank() - 1
            );
         
         // add it
         linkedCard.toLower = lowLink;
         linkedCard.toHigher = highLink;
         linkedCard.size = 1;
         cards.SetCard(CardLocation::Towers[i], linkedCard);
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
   LinkedCard  card = cards.GetCard(CardLocation::Columns[columnIndex][columnCounts.Get(columnIndex) - 1]);
   
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

   case SolverMoveType::FromThroneToTower:
      cards.MoveToOpenTower(CardLocation::Thrones[move.suit.GetIndex()]);
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
   cards.MoveToHigher(CardLocation::Columns[columnIndex][columnCounts.Get(columnIndex)]);
}


void SolverState::MoveColumnToTower(int columnIndex)
{
   // move it and decrement the column count
   columnCounts.Decrement(columnIndex);
   cards.MoveToOpenTower(CardLocation::Columns[columnIndex][columnCounts.Get(columnIndex)]);
}

void SolverState::MoveTowerToThrone(Suit suit)
{
   // just move the tower's lower to it
   cards.MoveToHigher(cards.GetCard(CardLocation::Thrones[suit.GetIndex()]).toLower);
}


bool SolverState::DoFreeMoves(void)
{
   bool didFreeMoves = false;

   // try moving to the aces repeatedly until nothing happens
   for (;;)
   {
      bool acesMoved = false;
      for (Suit suit : Suit::All)
      {
         LinkedCard ace = cards.GetCard(CardLocation::Aces[suit.GetIndex()]);
         CardLocation nextAceCard = ace.toHigher;
         if (nextAceCard.isTower || nextAceCard.isThrone)
         {
            cards.MoveToLower(nextAceCard);
            acesMoved = true;
            continue;
         }
         
         if (nextAceCard.onColumn)
         {
            int row = nextAceCard.row;
            if (row+1 == columnCounts.Get(nextAceCard.column))
            {
               cards.MoveToLower(nextAceCard);
               columnCounts.Decrement(nextAceCard.column);
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
      LinkedCard tower = cards.GetCard(CardLocation::Towers[i]);
      if (tower.size == 0)
         continue;

      // if its higher link is the bottom card on a column we can combine them
      bool canMoveToHigher = false;
      if (IsBottomColumnCard(tower.toHigher))
         canMoveToHigher = true;

      // if its higher link is a non-empty throne (i.e. a king that has been put on
      // an empty column) we can likewise move it there
      if (tower.toHigher.isThrone)
         if (GetCard(tower.toHigher).size != 0)
            canMoveToHigher = true;

      // and if its higher is on a tower we can also combine the tower cards
      if (tower.toHigher.isTower)
         canMoveToHigher = true;

      // combine with the next higher card if we can
      if (canMoveToHigher)
      {
         cards.MoveToHigher(CardLocation::Towers[i]);

         // Note that our "didFreeMoves" result is an indication of whether or not
         // we did something that helped the situation.  Combining two towers is just
         // a cleanup for our accounting... it doesn't really improve anything.
         if (!tower.toHigher.isTower)
            didFreeMoves = true;
      }
   }

   // any kings that are the only card on a column get moved to their throne, since
   // the thrones are invented for exactly that purpose
   for (Suit suit : Suit::All)
   {
      // get the throne, make sure that it's empty
      LinkedCard throne = cards.GetCard(CardLocation::Thrones[suit.GetIndex()]);
      if (throne.size != 0)
         continue;

      // get the king, make sure that it's on a column
      CardLocation kingLocation = throne.toLower;
      if (!kingLocation.onColumn)
         continue;

      // if it is the only card on the column, move it to the throne
      if (columnCounts.Get(kingLocation.column) == 1)
      {
         cards.MoveToHigher(kingLocation);
         columnCounts.Decrement(kingLocation.column);
         didFreeMoves = true;
      }
   }

   // if we have kings on the towers and enough empty columns to hold them all,
   // then they all get moved to their thrones
   int kingsOnTowers = cards.CountKingsOnTowers();
   if (kingsOnTowers > 0 && GetEmptyColumnCount() >= kingsOnTowers)
   {
      for (Suit suit : Suit::All)
      {
         LinkedCard throne = cards.GetCard(CardLocation::Thrones[suit.GetIndex()]);
         if (throne.size == 0)
         {
            if (throne.toLower.isTower)
            {
               cards.MoveToHigher(throne.toLower);
               didFreeMoves = true;
            }
         }
      }
   }

   return didFreeMoves;
}

bool SolverState::IsBottomColumnCard(CardLocation cardLocation) const
{
   if (!cardLocation.onColumn)
      return false;
   int row = cardLocation.row;
   return (row+1 == columnCounts.Get(cardLocation.column));
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
   for (Suit suit : Suit::All)
      if (cards.GetCard(CardLocation::Thrones[suit.GetIndex()]).size != 0)
         --emptyColumns;

   // done
   return emptyColumns;
}


SolverHashCode SolverState::GetHashValue(void) const
{
   return SolverHashCode(
      columnCounts.GetPointer(),
      cards.GetThroneHashValue()
      );
}


