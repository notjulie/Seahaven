/* 
 * File:   SolverHashCode.h
 * Author: Randy
 *
 * Created on April 16, 2014, 12:13 PM
 */

#ifndef SOLVERHASHCODE_H
#define	SOLVERHASHCODE_H


#define HASH_CODE_BLOCK_NUMBER_RANGE (16*6*6*6)
#define HASH_CODE_BYTE_OFFSET_RANGE (6*6*6*6*6*6*6/8)

class SolverHashCode {
public:
   SolverHashCode(const uint8_t *columnCounts, uint8_t kingsOnColumnsMask);

   uint16_t GetBitNumber(void) const { return bitNumber; }
   uint16_t GetBlockNumber(void) const { return blockNumber; }
   uint16_t GetByteOffset(void) const { return byteOffset; }
   
private:
   uint16_t blockNumber;
   uint16_t byteOffset;
   uint8_t  bitNumber;
};

#endif	/* SOLVERHASHCODE_H */

