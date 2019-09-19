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
#include "SolverResult.h"
#include "SolverStack.h"

class SeahavenProblem;


class Solver {
public:
   Solver(void);

   void DisableCacheing(void) { cache.Disable(); }
   const SolverResult& GetResult(void) const { return result; }
   int GetTotalBranchesTested(void) { return stateStack.GetTotalPushCount(); }

   Solution Solve(const SeahavenProblem& problem);

private:
   enum FreeMovesResult {
      DidFreeMoves,
      DidNothing,
      Victory,
      DeadEnd
   };

private:
   FreeMovesResult DoFreeMoves(StackPointer stackPointer);
   void DoFreeMovesAndSolve(StackPointer stackPointer);
   bool PushCurrentStateAndPerformMove(StackPointer stackPointer, SolverMove move);
   void SolverStep(StackPointer stackPointer);
   void TryMoveKingToColumn(StackPointer stackPointer, Suit suit);
   void TryMoveLastCardFromColumn(StackPointer stackPointer, int column);
   void TryMovingACardToColumn(StackPointer stackPointer, int column);
   void TryColumnMoves(StackPointer stackPointer, int column);
   bool TryPushColumnToTowerMove(StackPointer& stackPointer, int column);
   bool TryPushColumnToHigherAndSolve(StackPointer& stackPointer, int column);
   void TryPushTowerToThroneAndSolve(StackPointer stackPointer, int tower);

private:
   SolverStack   stateStack;
   SolverCache cache;
   SolverResult result;
};

#endif	/* SOLVER_H */

