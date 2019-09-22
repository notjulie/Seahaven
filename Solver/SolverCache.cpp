/* 
 * File:   SolverCache.cpp
 * Author: Randy
 * 
 * Created on April 9, 2014, 8:11 PM
 */

#include "SolverIncludes.h"

#include "SolverException.h"
#include "SolverHashCode.h"

#include "SolverCache.h"


// each block is about 35KBytes (unless I go and change the format of the
// hash code)... 200 would be 7MB, and that's probably a good place
// to stop
#define MAX_BLOCK_COUNT 2000



SolverCache::SolverCache() {
   Clear();
}

void SolverCache::Clear(void)
{
   // clearing nextBlockIndex is easy
   nextBlockIndex = 0;
   
   // clear the block indexes to -1
   blockIndexes.resize(SolverHashCode::BlockNumberRange);
   for (int i=0; i<blockIndexes.size(); ++i)
      blockIndexes[i] = -1;

   // and we don't have to clear the blocks... just make sure the
   // array of (possibly) empty blocks is allocated
   blocks.resize(MAX_BLOCK_COUNT);
}

bool SolverCache::TestAndSet(const SolverHashCode &hashCode)
{
   if (disabled)
      return false;

   // the major part is the index of the block index
   int   blockIndex = blockIndexes[hashCode.GetBlockNumber()];
   
   // if we haven't yet allocated a block for this value do so now
   if (blockIndex < 0)
   {
      // the best thing we can do if we fill up the cache is clear it
      // and commence refilling it
      if (nextBlockIndex >= MAX_BLOCK_COUNT)
         Clear();
      
      blockIndex = nextBlockIndex++;
      blockIndexes[hashCode.GetBlockNumber()] = blockIndex;
      blocks[blockIndex].resize(SolverHashCode::ByteOffsetRange);
      memset(&blocks[blockIndex][0], 0, blocks[blockIndex].size());
   }
   
   // test and set
   uint8_t *bp = &blocks[blockIndex][hashCode.GetByteOffset()];
   uint8_t mask = 1<<hashCode.GetBitNumber();
   bool result = ((*bp) & mask) != 0;
   *bp |= mask;
   return result;
}


