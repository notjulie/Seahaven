/* 
 * File:   SolverState.h
 * Author: Randy
 *
 * Created on April 2, 2014, 10:34 PM
 */

#ifndef SOLVERSTATE_H
#define	SOLVERSTATE_H

#include "LinkedCards.h"
#include "SolverMove.h"
#include "Suit.h"

class SeahavenProblem;
class SolverHashCode;

class ColumnCounts {
public:
   ColumnCounts(void);

   void Clear(void);
   void Decrement(uint8_t column);
   uint8_t Get(uint8_t column) const;
   const uint8_t* GetPointer(void) const { return columnCounts; }
   void Increment(uint8_t column);

private:
   uint8_t        columnCounts[10];
};

class SolverState {
public:
   SolverState(void);
   SolverState(const SeahavenProblem &problem);

   bool  CanMoveColumnToColumn(int columnIndex) const;
   bool  CanMoveColumnToTower(int columnIndex) const;
   bool  DoFreeMoves(void);
   uint8_t GetColumnCardCount(uint8_t column) const { return columnCounts.Get(column); }
   bool  IsBottomColumnCard(CardLocation card) const;
   bool  IsOnlyCardOnColumn(LinkID link) const;
   bool  IsVictory(void) const;
   LinkedCard GetColumnBottomCard(int column) const { return cards.GetColumnCard(column, columnCounts.Get(column)); }
   ProblemCard GetBottomColumnCardDetails(int column) const;
   SolverMove   GetMoveThatWasPerformed(void) const { return movePerformed; }
   SolverHashCode GetHashValue(void) const;
   void PerformMove(SolverMove move);
   
private:
   void Clear(void);
   void  MoveColumnToColumnOrThrone(int columnIndex);
   void  MoveColumnToTower(int columnIndex);

private:
   static LinkID GetLinkID(const SeahavenProblem &problem, Suit suit, uint8_t rank);

private:
   LinkedCards    cards;
   SolverMove     movePerformed;
   ColumnCounts   columnCounts;
};

#endif	/* SOLVERSTATE_H */

