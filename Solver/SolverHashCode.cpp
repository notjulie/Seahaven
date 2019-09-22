/* 
 * File:   SolverHashCode.cpp
 * Author: Randy
 * 
 * Created on April 16, 2014, 12:13 PM
 */

#include "SolverIncludes.h"

#include "SolverException.h"

#include "SolverHashCode.h"

SolverHashCode::SolverHashCode(const uint8_t *columnCounts, uint8_t throneHashValue) {
   // calculate a block number from the throne hash and the count of cards
   // on columns 7, 8 and 9
   blockNumber = throneHashValue;
   for (int i=7; i<10; ++i)
      blockNumber = blockNumber*6 + columnCounts[i];
   
   // calculate a bit number from the count of cards on columns 0-6
   uint32_t fullMinorPart = 0;
   for (int i=0; i<7; ++i)
      fullMinorPart = fullMinorPart*6 + columnCounts[i];
   
   // convert to a byte offset and bit number
   byteOffset = fullMinorPart>>3;
   bitNumber = fullMinorPart & 7;
}


