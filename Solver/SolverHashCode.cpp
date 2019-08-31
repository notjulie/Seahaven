/* 
 * File:   SolverHashCode.cpp
 * Author: Randy
 * 
 * Created on April 16, 2014, 12:13 PM
 */

#include "SolverIncludes.h"

#include "SolverException.h"

#include "SolverHashCode.h"

SolverHashCode::SolverHashCode(const uint8_t *columnCounts, const ThroneState *throneStates) {
   blockNumber = 0;
   for (int i=0; i<3; ++i)
      blockNumber = blockNumber*3 + throneStates[i];
   for (int i=7; i<10; ++i)
      blockNumber = blockNumber*6 + columnCounts[i];
   
   uint32_t fullMinorPart = 0;
   for (int i=0; i<7; ++i)
      fullMinorPart = fullMinorPart*6 + columnCounts[i];
   
   byteOffset = fullMinorPart>>3;
   bitNumber = fullMinorPart & 7;
}


