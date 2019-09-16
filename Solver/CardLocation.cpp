
#include "SolverIncludes.h"
#include "CardLocation.h"


int CardLocation::GetColumnIndex(void) const
{
   int column = ((uint8_t)linkID - (uint8_t)LinkID::FIRST_COLUMN_LINK) / 5;
   if (column >= 10)
      return -1;
   else
      return column;
}


Suit CardLocation::GetSuit(void) const
{
   int suitIndex = (uint8_t)linkID - (uint8_t)LinkID::FIRST_ACE_LINK;
   if (suitIndex<0 || suitIndex>=4)
      suitIndex = (uint8_t)linkID - (uint8_t)LinkID::FIRST_THRONE_LINK;
   return Suit::FromIndex(suitIndex);
}

