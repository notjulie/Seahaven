/* 
 * File:   SolverCache.h
 * Author: Randy
 *
 * Created on April 9, 2014, 8:11 PM
 */

#ifndef SOLVERCACHE_H
#define	SOLVERCACHE_H

#include "SolverHashCode.h"


class SolverCache {
public:
   SolverCache(void);

   void  Clear(void);
   void  Disable(void) { disabled = true; }
   bool  TestAndSet(const SolverHashCode &hashCode);
   
private:
   struct Block {
      std::vector<uint8_t> bitSet;
      SolverHashCode::BlockNumber blockNumber = SolverHashCode::InvalidBlockNumber;
   };
   
private:
   std::vector<int16_t>  blockIndexes;
   std::vector<Block> blocks;
   int16_t nextBlockIndex;
   bool disabled = false;
};

#endif	/* SOLVERCACHE_H */

