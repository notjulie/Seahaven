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
         columnCounts[columnIndex]++;
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


bool SolverState::CanMoveColumnToColumnOrThrone(int columnIndex) const
{
   // never mind if the column is empty
   if (columnCounts[columnIndex] == 0)
      return false;
   
   // get the card on the bottom of the column
   LinkedCard  card = cards.GetColumnCard(columnIndex, columnCounts[columnIndex] - 1);

   // if the higher is a throne, check that
   if (LinkedCards::IsThrone(card.toHigher))
      return throneArbitor.CanMoveToThrone(card.size);
   
   // never mind if we don't have the tower space
   if (card.size > throneArbitor.GetEmptyTowers() + 1)
      return false;
   
   // easy from there
   return this->IsBottomColumnCard(card.toHigher);
}


bool SolverState::CanMoveColumnToTower(int columnIndex) const
{
   // never mind if the column is empty
   if (columnCounts[columnIndex] == 0)
      return false;
   
   // get the card on the bottom of the column
   LinkedCard  card = cards.GetColumnCard(columnIndex, columnCounts[columnIndex] - 1);
   
   // all good as long as we don't have the tower space
   return card.size <= throneArbitor.GetEmptyTowers();
}


void SolverState::PerformMove(SolverMove move)
{
   switch (move.type)
   {
   case SolverMove::MoveFromColumn:
      if (CanMoveColumnToColumnOrThrone(move.column))
         MoveColumnToColumnOrThrone(move.column);
      else
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
   LinkID   link = cards.GetColumnLinkID(columnIndex, --columnCounts[columnIndex]);
   cards.MoveColumnCardToHigher(link);
}


void SolverState::MoveColumnToTower(int columnIndex)
{
   // move it and decrement the column count
   LinkID   link = cards.GetColumnLinkID(columnIndex, --columnCounts[columnIndex]);
   cards.MoveToOpenTower(link);
}


ProblemCard SolverState::GetBottomColumnCardDetails(int columnIndex) const
{
   LinkID   link = cards.GetColumnLinkID(columnIndex, columnCounts[columnIndex]-1);
   return cards.GetCardDetails(link);
}


void SolverState::DoFreeMoves(void)
{
   // first we need to update the state of the thrones
   ArbitrateThrones();
   
   // try moving to the aces repeatedly until nothing happens
   for (;;)
   {
      bool acesMoved = false;
      for (int i=0; i<4; ++i)
      {
         LinkedCard ace = cards.GetAce((Suit)i);
         LinkID nextAceCard = ace.toHigher;
         if (LinkedCards::IsTower(nextAceCard) || LinkedCards::IsThrone(nextAceCard))
         {
            cards.MoveToLower(nextAceCard);
            acesMoved = true;
            continue;
         }
         
         int column = (nextAceCard - FIRST_COLUMN_LINK) / 5;
         if (column>=0 && column<=9)
         {
            int row = (nextAceCard - FIRST_COLUMN_LINK) % 5;
            if (row+1 == columnCounts[column])
            {
               cards.MoveToLower(nextAceCard);
               columnCounts[column]--;
               acesMoved = true;
            }
         }
      }
      
      if (!acesMoved)
         break;
   }
   
   // the next thing to do is to combine cards on towers with columns or thrones
   for (int i=0; i<4; ++i)
   {
      LinkedCard tower = cards.GetTower(i);
      if (tower.size == 0)
         continue;
      if (LinkedCards::IsThrone(tower.toHigher) || IsBottomColumnCard(tower.toHigher))
         cards.MoveToHigher(LinkedCards::GetTowerLinkID(i));
   }

   // any kings sitting on a tower or as the only card on a column get moved
   // to their throne
   for (int i=0; i<4; ++i)
   {
      LinkedCard throne = cards.GetThrone(i);
      if (LinkedCards::IsTower(throne.toLower) || IsOnlyCardOnColumn(throne.toLower))
         cards.MoveToHigher(LinkedCards::GetThroneLinkID((Suit)i));
   }
   
   // and we need to update the state of the thrones again
   ArbitrateThrones();
}

void SolverState::ArbitrateThrones(void)
{
   // figure out how many towers are used by non-kings
   int towersUsed = 0;
   for (int i=0; i<4; ++i)
   {
      LinkedCard tower = cards.GetTower(i);
      towersUsed += tower.size;
   }
   
   // figure out how many empty columns we have
   uint8_t emptyColumns = 0;
   for (int i=0; i<10; ++i)
      if (columnCounts[i] == 0)
         ++emptyColumns;
   
   // get the throne sizes
   uint8_t throneSizes[4];
   for (int i=0; i<4; ++i)
   {
      LinkedCard throne = cards.GetThrone(i);
      throneSizes[i] = throne.size;
   }
   
   throneArbitor.Arbitrate(
      towersUsed,
      emptyColumns,
      throneSizes
      );
}

bool SolverState::IsBottomColumnCard(LinkID link) const
{
   int column = (link - FIRST_COLUMN_LINK) / 5;
   if (column<0 || column>9)
      return false;
   int row = (link - FIRST_COLUMN_LINK) % 5;
   return (row+1 == columnCounts[column]);
}

bool SolverState::IsOnlyCardOnColumn(LinkID link) const
{
   int column = (link - FIRST_COLUMN_LINK) / 5;
   if (column<0 || column>9)
      return false;
   return columnCounts[column] == 0;
}

bool SolverState::IsVictory(void) const
{
   for (int i=0; i<10; ++i)
      if (columnCounts[i] != 0)
         return false;
   
   return true;
}


void SolverState::Clear(void)
{
   for (int i=0; i<10; ++i)
      columnCounts[i] = 0;
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
      columnCounts,
      throneArbitor.GetThroneStates()
      );
}
