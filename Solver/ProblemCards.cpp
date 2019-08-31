/* 
 * File:   ProblemCards.cpp
 * Author: Randy
 * 
 * Created on April 2, 2014, 2:08 PM
 */

#include "SolverIncludes.h"
#include <iostream>

#include "SolverException.h"

#include "ProblemCards.h"

ProblemCards::ProblemCards(
            const ProblemCard &card0,
            const ProblemCard &card1,
            const ProblemCard &card2,
            const ProblemCard &card3,
            const ProblemCard &card4,
            const ProblemCard &card5,
            const ProblemCard &card6,
            const ProblemCard &card7,
            const ProblemCard &card8
            )
{
    if (!card0.IsNull())
        cards.push_back(card0);
    if (!card1.IsNull())
        cards.push_back(card1);
    if (!card2.IsNull())
        cards.push_back(card2);
    if (!card3.IsNull())
        cards.push_back(card3);
    if (!card4.IsNull())
        cards.push_back(card4);
    if (!card5.IsNull())
        cards.push_back(card5);
    if (!card6.IsNull())
        cards.push_back(card6);
    if (!card7.IsNull())
        cards.push_back(card7);
    if (!card8.IsNull())
        cards.push_back(card8);
}

ProblemCard ProblemCards::GetCard(uint8_t index) const
{
   if (index >= cards.size())
      return ProblemCard();
   else
      return cards[index];
}

void ProblemCards::Dump(void)
{
   for (int i = 0; i < cards.size(); ++i)
      cards[i].DumpToConsole();
   std::cout << std::endl;
}

