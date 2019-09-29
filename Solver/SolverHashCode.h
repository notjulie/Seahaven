/* 
 * File:   SolverHashCode.h
 * Author: Randy
 *
 * Created on April 16, 2014, 12:13 PM
 */

#ifndef SOLVERHASHCODE_H
#define	SOLVERHASHCODE_H

#include "LinkedCards.h"


class SolverHashCode {
public:
   SolverHashCode(const uint8_t *columnCounts, uint8_t kingsOnColumnsMask);

   uint16_t GetBitNumber(void) const { return bitNumber; }
   uint16_t GetBlockNumber(void) const { return blockNumber; }
   uint16_t GetByteOffset(void) const { return byteOffset; }
   
public:
   typedef uint16_t BlockNumber;

public:
   static const BlockNumber MaxBlockNumber = (LinkedCards::ThroneHashRange * 6 * 6 * 6) - 1;
   static const BlockNumber InvalidBlockNumber = MaxBlockNumber + 1;
   static const int ByteOffsetRange = (6 * 6 * 6 * 6 * 6 * 6 * 6 / 8);

   static_assert(std::numeric_limits<BlockNumber>::max() >= InvalidBlockNumber, "BlockNumber type not big enough");

private:
   BlockNumber blockNumber;
   uint16_t byteOffset;
   uint8_t  bitNumber;
};

#endif	/* SOLVERHASHCODE_H */

