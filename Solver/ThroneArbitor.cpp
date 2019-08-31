/* 
 * File:   ThroneArbitor.cpp
 * Author: Randy
 * 
 * Created on April 13, 2014, 9:36 PM
 */

#include "SolverIncludes.h"

#include "SolverException.h"

#include "ThroneArbitor.h"

ThroneArbitor::ThroneArbitor() {
   // clear
   towersUsed = 0;
   emptyColumns = 0;
   throneStates[0] = UNSPECIFIED;
   throneStates[1] = UNSPECIFIED;
   throneStates[2] = UNSPECIFIED;
   throneStates[3] = UNSPECIFIED;
}

void ThroneArbitor::Arbitrate(uint8_t _towersUsed, uint8_t _emptyColumns, const uint8_t *throneSizes)
{
   // simple case #1: no empty columns
   if (_emptyColumns == 0)
   {
      HandleAllOnTowers(_towersUsed, throneSizes);
      return;
   }
   
   // count kings
   int kingCount = 0;
   for (int i=0; i<4; ++i)
      if (throneSizes[i] > 0)
         ++kingCount;

   // simple case #2: enough empty columns to handle all kings
   if (_emptyColumns >= kingCount)
   {
      HandleAllOnColumns(_towersUsed, _emptyColumns - kingCount);
      return;
   }
   
   throw SolverException("ThroneArbitor::Arbitrate: not implemented");
}

void ThroneArbitor::HandleAllOnColumns(uint8_t _towersUsed, uint8_t _emptyColumns)
{
   throneStates[0] = ON_COLUMN;
   throneStates[1] = ON_COLUMN;
   throneStates[2] = ON_COLUMN;
   throneStates[3] = ON_COLUMN;
   towersUsed = _towersUsed;
   emptyColumns = _emptyColumns;
}

void ThroneArbitor::HandleAllOnTowers(uint8_t _towersUsed, const uint8_t *throneSizes)
{
   throneStates[0] = ON_TOWER;
   throneStates[1] = ON_TOWER;
   throneStates[2] = ON_TOWER;
   throneStates[3] = ON_TOWER;
   towersUsed = _towersUsed + throneSizes[0] + throneSizes[1] + throneSizes[2] + throneSizes[3];
}


bool ThroneArbitor::CanMoveToThrone(uint8_t size) const
{
   // if we know we can shove it on the towers the answer is a
   // quick and easy yes
   if (towersUsed + size <= 4)
      return true;
   
   // if we can't shove everything except the king on the towers then
   // we know we *can't* move it
   if (towersUsed + size > 5)
      return false;
   
   // and in this case it boils down to whether or not we have an empty
   // column
   return emptyColumns > 0;
}
