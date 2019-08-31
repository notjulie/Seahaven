/* 
 * File:   SolverCache.h
 * Author: Randy
 *
 * Created on April 9, 2014, 8:11 PM
 */

#ifndef SOLVERCACHE_H
#define	SOLVERCACHE_H

class SolverHashCode;

class SolverCache {
public:
   SolverCache();

   void  Clear(void);
   bool  TestAndSet(const SolverHashCode &hashCode);
   
private:
   typedef std::vector<uint8_t> Block;
   
private:
   std::vector<int16_t>  blockIndexes;
   std::vector<Block> blocks;
   int16_t nextBlockIndex;
};

#endif	/* SOLVERCACHE_H */

