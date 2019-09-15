/* 
 * File:   SeahavenProblem.cpp
 * Author: Randy
 * 
 * Created on April 2, 2014, 1:51 PM
 */

#include "SolverIncludes.h"

#include "ProblemCards.h"
#include "SolverException.h"

#include "SeahavenProblem.h"

SeahavenProblem::SeahavenProblem() {
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


void SeahavenProblem::Dump(void)
{
   for (int i = 0; i < columns.size(); ++i)
      columns[i].Dump();
   towers.Dump();
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
