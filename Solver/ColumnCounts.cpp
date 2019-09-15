
#include "SolverIncludes.h"
#include "SolverException.h"
#include "ColumnCounts.h"

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

int ColumnCounts::GetEmptyCount(void) const
{
   int emptyColumns = 0;

   for (int i = 0; i < 10; ++i)
      if (columnCounts[i] == 0)
         ++emptyColumns;

   return emptyColumns;
}

void ColumnCounts::Increment(uint8_t column)
{
   if (++columnCounts[column] >= 10)
      throw SolverException("ColumnCounts::Decrement: count overflow");
}

