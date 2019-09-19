/* 
 * File:   SeahavenProblem.cpp
 * Author: Randy
 * 
 * Created on April 2, 2014, 1:51 PM
 */

#include "SolverIncludes.h"

#include "LinkedCards.h"
#include "ProblemCards.h"
#include "SolverException.h"
#include "SolverState.h"

#include "SeahavenProblem.h"

SeahavenProblem::SeahavenProblem() {
}

SeahavenProblem::SeahavenProblem(const SolverState& state)
{
   for (int i = 0; i < 10; ++i)
   {
      ProblemCards column;
      for (int row = 0; row < state.GetColumnCardCount(i); ++row)
         AddCardToCollection(state, column, CardLocation::GetColumnCard(i, row));
      columns.push_back(column);
   }

   for (int i = 0; i < 4; ++i)
      AddCardToCollection(state, towers, CardLocation::GetTower(i));
}

SeahavenProblem::SeahavenProblem(
        const ProblemCards &column0,
        const ProblemCards &column1,
        const ProblemCards &column2,
        const ProblemCards &column3,
        const ProblemCards &column4,
        const ProblemCards &column5,
        const ProblemCards &column6,
        const ProblemCards &column7,
        const ProblemCards &column8,
        const ProblemCards &column9,
        const ProblemCards &_towers
        )
{
    towers = _towers;
    columns.push_back(column0);
    columns.push_back(column1);
    columns.push_back(column2);
    columns.push_back(column3);
    columns.push_back(column4);
    columns.push_back(column5);
    columns.push_back(column6);
    columns.push_back(column7);
    columns.push_back(column8);
    columns.push_back(column9);
}

SeahavenProblem::~SeahavenProblem() {
}


void SeahavenProblem::Dump(FILE *f)
{
   for (int i = 0; i < columns.size(); ++i)
      columns[i].Dump(f);
   towers.Dump(f);
}

SeahavenProblem SeahavenProblem::CreateRandom(void)
{
   SeahavenProblem result;

   // create a deck
   std::vector<ProblemCard>  deck;
   for (Suit suit = Suit::First; suit <= Suit::Last; ++suit)
   {
      for (int rank = 1; rank <= 13; ++rank)
         deck.push_back(ProblemCard(suit, rank));
   }

   // push random cards into the result
   result.columns.resize(10);
   for (int column = 0; column < 10; ++column)
   {
      for (int row = 0; row < 5; ++row)
      {
         int i = rand() % deck.size();
         result.columns[column].Append(deck[i]);
         deck[i] = deck[deck.size() - 1];
         deck.resize(deck.size() - 1);
      }
   }

   result.towers.Append(deck[0]);
   result.towers.Append(deck[1]);

   return result;
}


void SeahavenProblem::AddCardToCollection(const SolverState& state, ProblemCards& cards, CardLocation cardLocation)
{
   // get the card
   LinkedCard card = state.GetCard(cardLocation);
   if (card.size == 0)
      return;

   // get the info about the top card of the group
   ProblemCard cardInfo = state.GetCardDetails(cardLocation);
   
   // add however many cards we need to add
   for (int i = 0; i < card.size; ++i)
   {
      cards.Append(cardInfo);
      cardInfo = ProblemCard(cardInfo.GetSuit(), cardInfo.GetRank() - 1);
   }
}


/// <summary>
/// Takes the cards in a column and compacts it so that consecutive cards of
/// the same suit are merged into a single card with a size >1
/// </summary>
std::vector<CompactedColumnCard> SeahavenProblem::GetCompactedColumn(uint8_t columnIndex) const
{
   // declare the result
   std::vector<CompactedColumnCard> result;

   // get the cards in the column
   ProblemCards cards = columns[columnIndex];
   
   // process the column
   int i=0;
   while (i < cards.GetCount())
   {
      // get the next card
      CompactedColumnCard compactedCard;
      compactedCard.topCard = cards.GetCard(i++);
      compactedCard.cardCount = 1;
      
      // add any following cards to it if they are consecutive
      for (;;)
      {
         if (i >= cards.GetCount())
            break;
         ProblemCard nextCard = cards.GetCard(i);
         if (compactedCard.topCard.GetSuit() != nextCard.GetSuit())
            break;
         if (nextCard.GetRank() != compactedCard.topCard.GetRank() - compactedCard.cardCount)
            break;
         
         // we have a card to add
         i++;
         compactedCard.cardCount++;
      }
      
      // add the combined card to the collection
      result.push_back(compactedCard);
   }
   
   // return the result
   return result;
}


/// <summary>
/// Locates the given card in the problem object and returns its location
/// as a link.
/// </summary>
LinkID SeahavenProblem::GetCardLinkID(Suit suit, uint8_t rank) const
{
   // a rank of zero would mean the location that an ace would link down to
   if (rank == 0)
      return suit.GetAceLinkID();

   // a rank of 14 would be the location that a king would link up to
   if (rank == 14)
      return suit.GetThroneLinkID();

   // see if it's on a column
   for (int columnIndex = 0; columnIndex < 10; ++columnIndex)
   {
      std::vector<CompactedColumnCard> compactedCards = GetCompactedColumn(columnIndex);

      for (int i = 0; i < compactedCards.size(); ++i)
      {
         if (suit != compactedCards[i].topCard.GetSuit())
            continue;
         int   topRank = compactedCards[i].topCard.GetRank();
         int   bottomRank = topRank + 1 - compactedCards[i].cardCount;
         if (rank >= bottomRank && rank <= topRank)
            return LinkedCards::GetColumnLinkID(columnIndex, i);
      }
   }

   // see if it's on a tower
   for (int towerIndex = 0; towerIndex < 4; ++towerIndex)
   {
      ProblemCard card = GetTower(towerIndex);
      if (card.GetSuit() == suit && card.GetRank() == rank)
         return LinkedCards::GetTowerLinkID(towerIndex);
   }

   // else we assume it must be sitting on top of the ace pile... there isn't
   // anywhere else that it could be
   return suit.GetAceLinkID();
}

