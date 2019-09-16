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
#include "SolverStack.h"

class SeahavenProblem;


class Solver {
public:
   Solver(void);

   void DisableCacheing(void) { cache.Disable(); }
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
   void SolverStep(StackPointer stackPointer);
   void TryMove(StackPointer stackPointer, SolverMove move);
   void TryMoveKingToColumn(StackPointer stackPointer, Suit suit);
   void TryMoveLastCardFromColumn(StackPointer stackPointer, int column);
   void TryMovingACardToColumn(StackPointer stackPointer, int column);
   void TryColumnMoves(StackPointer stackPointer, int column);
   bool TryPushColumnToTowerMove(StackPointer& stackPointer, int column);
   bool TryPushColumnToHigherAndSolve(StackPointer& stackPointer, int column);

private:
   SolverStack   stateStack;
   SolverStack   resultStack;
   SolverCache cache;
};

#endif	/* SOLVER_H */

