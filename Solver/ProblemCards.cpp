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

ProblemCards::ProblemCards(void)
{
}

ProblemCards::ProblemCards(const std::initializer_list<const char*> _cards)
{
   for (auto card : _cards)
      cards.push_back(ProblemCard(card));
}

ProblemCard ProblemCards::GetCard(uint8_t index) const
{
   if (index >= cards.size())
      return ProblemCard();
   else
      return cards[index];
}

void ProblemCards::Dump(FILE *f)
{
   for (int i = 0; i < cards.size(); ++i)
   {
      if (i != 0)
         fprintf(f, " ");
      cards[i].Dump(f);
   }
   fprintf(f, "\r\n");
}

void ProblemCards::SetCard(int index, const ProblemCard& card)
{
   if (cards.size() <= index)
      cards.resize(index + 1);
   cards[index] = card;
}
