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
         LinkID highLink = GetLinkID(
            problem,
            compactedCards[j].topCard.GetSuit(),
            compactedCards[j].topCard.GetRank() + 1
            );
         LinkID lowLink = GetLinkID(
            problem,
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
         LinkID highLink = GetLinkID(
            problem,
            tower.GetSuit(),
            tower.GetRank() + 1
            );
         LinkID lowLink = GetLinkID(
            problem,
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


/// <summary>
/// Determines if the bottom card on the column can be moved to the next
/// higher card (on another column) or an empty column if it's a king
/// </summary>
bool SolverState::CanMoveColumnToColumn(int columnIndex) const
{
   // never mind if the column is empty
   if (columnCounts.Get(columnIndex) == 0)
      return false;
   
   // get the card on the bottom of the column
   LinkedCard  card = cards.GetColumnCard(columnIndex, columnCounts.Get(columnIndex) - 1);

   // in order to move it we need enough tower space
   if (card.size > cards.GetEmptyTowers() + 1)
      return false;

   // if it's a king it needs to go to an empty column, or by proxy, to its throne
   if (cards.IsKing(card))
   {
      throw SolverException("SolverState::CanMoveColumnToColumn: Move king to column not implemented");
   }
   else
   {
      // else we just need to know if the next higher card is on the bottom of
      // a column
      return this->IsBottomColumnCard(card.toHigher);
   }
}


bool SolverState::CanMoveColumnToTower(int columnIndex) const
{
   // never mind if the column is empty
   if (columnCounts.Get(columnIndex) == 0)
      return false;
   
   // get the card on the bottom of the column
   LinkedCard  card = cards.GetColumnCard(columnIndex, columnCounts.Get(columnIndex) - 1);
   
   // all good as long as we don't have the tower space
   return card.size <= cards.GetEmptyTowers();
}


/// <summary>
/// Carries out the move defined by the SolverMove object
/// </summary>
void SolverState::PerformMove(SolverMove move)
{
   switch (move.type)
   {
   case SolverMoveType::FromColumnToColumn:
      MoveColumnToColumnOrThrone(move.column);
      break;

   case SolverMoveType::FromColumnToTower:
      MoveColumnToTower(move.column);
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
   cards.MoveColumnCardToHigher(link);
}


void SolverState::MoveColumnToTower(int columnIndex)
{
   // move it and decrement the column count
   columnCounts.Decrement(columnIndex);
   LinkID   link = cards.GetColumnLinkID(columnIndex, columnCounts.Get(columnIndex));
   cards.MoveToOpenTower(link);
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
   
   // the next thing to do is to combine cards on towers with columns or thrones
   for (int i=0; i<4; ++i)
   {
      LinkedCard tower = cards.GetTower(i);
      if (tower.size == 0)
         continue;
      if (tower.toHigher.IsThrone() || IsBottomColumnCard(tower.toHigher))
      {
         cards.MoveToHigher(LinkedCards::GetTowerLinkID(i));
         didFreeMoves = true;
      }
   }

   // any kings sitting on a tower or as the only card on a column get moved
   // to their throne
   for (Suit suit=Suit::First; suit<=Suit::Last; ++suit)
   {
      LinkedCard throne = cards.GetThrone(suit);
      if (throne.toLower.IsTower() || IsOnlyCardOnColumn(throne.toLower.GetLinkID()))
      {
         cards.MoveToHigher(LinkedCards::GetThroneLinkID(suit));
         didFreeMoves = true;
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

bool SolverState::IsOnlyCardOnColumn(LinkID link) const
{
   int column = ((uint8_t)link - (uint8_t)LinkID::FIRST_COLUMN_LINK) / 5;
   if (column<0 || column>9)
      return false;
   return columnCounts.Get(column) == 0;
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

/// <summary>
/// Locates the given card in the problem object and returns its location
/// as a link.
/// </summary>
LinkID SolverState::GetLinkID(const SeahavenProblem &problem, Suit suit, uint8_t rank)
{
   // a rank of zero would mean the location that an ace would link down to
   if (rank == 0)
      return LinkedCards::GetAceLinkID(suit);
   
   // a rank of 14 would be the location that a king would link up to
   if (rank == 14)
      return LinkedCards::GetThroneLinkID(suit);
   
   // see if it's on a column
   for (int columnIndex=0; columnIndex<10; ++columnIndex)
   {
      std::vector<CompactedColumnCard> compactedCards = problem.GetCompactedColumn(columnIndex);

      for (int i=0; i<compactedCards.size(); ++i)
      {
         if (suit != compactedCards[i].topCard.GetSuit())
            continue;
         int   topRank = compactedCards[i].topCard.GetRank();
         int   bottomRank = topRank + 1 - compactedCards[i].cardCount;
         if (rank>=bottomRank && rank<=topRank)
            return LinkedCards::GetColumnLinkID(columnIndex, i);
      }
   }
   
   // see if it's on a tower
   for (int towerIndex=0; towerIndex<4; ++towerIndex)
   {
      ProblemCard card = problem.GetTower(towerIndex);
      if (card.GetSuit()==suit && card.GetRank()==rank)
         return LinkedCards::GetTowerLinkID(towerIndex);
   }
   
   // else we assume it must be sitting on top of the ace pile... there isn't
   // anywhere else that it could be
   return LinkedCards::GetAceLinkID(suit);
}

SolverHashCode SolverState::GetHashValue(void) const
{
   return SolverHashCode(
      columnCounts.GetPointer(),
      cards.GetThroneOccupationMask()
      );
}



ColumnCounts::ColumnCounts(void)
{
   Clear();
}

void ColumnCounts::Clear(void)
{
   for (int i = 0; i < 10; ++i)
      columnCounts[i] = 0;
}

void ColumnCounts::Decrement(uint8_t column)
{
   if (--columnCounts[column] >= 10)
      throw SolverException("ColumnCounts::Decrement: count overflow");
}

uint8_t ColumnCounts::Get(uint8_t column) const
{
   return columnCounts[column];
}

void ColumnCounts::Increment(uint8_t column)
{
   if (++columnCounts[column] >= 10)
      throw SolverException("ColumnCounts::Decrement: count overflow");
}

