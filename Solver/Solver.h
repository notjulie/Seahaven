/*
 * File:   Solver.h
 * Author: Randy
 *
 * Created on April 2, 2014, 2:18 PM
 */

#ifndef SOLVER_H
#define	SOLVER_H

#include "Solution.h"
#include "SolverCache.h"
#include "SolverState.h"

class SeahavenProblem;


class Solver {
public:
   Solver(void);

   void DisableCacheing(void) { cache.Disable(); }
   uint32_t GetTotalSteps(void) const { return totalSteps; }
   Solution Solve(const SeahavenProblem& problem);

private:
   enum FreeMovesResult {
      DidFreeMoves,
      DidNothing,
      Victory,
      DeadEnd
   };

private:
   FreeMovesResult DoFreeMoves(int currentStateIndex);
   void DoFreeMovesAndSolve(int currentStateIndex);
   void  SolverStep(int currentStateIndex);
   void TryMove(int currentStateIndex, SolverMove move);
   void TryMoveAnyKingToColumn(int currentStateIndex);
   void TryMovingACardToColumn(int currentStateIndex, int column);
   void TryColumnMoves(int currentStateIndex, int column);

private:
   std::vector<SolverState>   stateStack;
   std::vector<SolverState>   resultStack;
   uint32_t   totalSteps;
   SolverCache cache;
};

#endif	/* SOLVER_H */

