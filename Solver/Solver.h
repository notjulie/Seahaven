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


class SolverStack {
public:
   size_t GetSize(void) const { return stack.size(); }
   int GetTotalPushCount(void) const { return totalPushes; }
   bool IsEmpty(void) const { return stack.empty(); }
   void SetSize(int size) { stack.resize(size); }
   inline void IncrementPushCount(void) { ++totalPushes; }

   inline SolverState& operator[](int i) { return stack[i]; }

private:
   std::vector<SolverState>   stack;
   int totalPushes = 0;
};

class StackPointer {
public:
   StackPointer(SolverStack &_stack)
      : stack(_stack)
   {
   }

   int GetIndex(void) const { return index; }
   void PushCurrentState(void) { stack[index + 1] = stack[index]; ++index; stack.IncrementPushCount(); }

   inline SolverState& operator*(void) { return stack[index]; }
   inline SolverState* operator->(void) { return &stack[index]; }

private:
   SolverStack& stack;
   int index = 0;
};

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
   void  SolverStep(StackPointer stackPointer);
   void TryMove(StackPointer stackPointer, SolverMove move);
   void TryMoveAnyKingToColumn(StackPointer stackPointer);
   void TryMovingACardToColumn(StackPointer stackPointer, int column);
   void TryColumnMoves(StackPointer stackPointer, int column);

private:
   SolverStack   stateStack;
   SolverStack   resultStack;
   SolverCache cache;
};

#endif	/* SOLVER_H */

