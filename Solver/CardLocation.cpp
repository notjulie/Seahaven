
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
