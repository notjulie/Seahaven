/* 
 * File:   SolverState.h
 * Author: Randy
 *
 * Created on April 2, 2014, 10:34 PM
 */

#ifndef SOLVERSTATE_H
#define	SOLVERSTATE_H

#include "ColumnCounts.h"
#include "LinkedCards.h"
#include "SolverMove.h"
#include "Suit.h"

class SeahavenProblem;
class SolverHashCode;


class SolverState {
public:
   SolverState(void);
   SolverState(const SeahavenProblem &problem);

   bool  CanMoveColumnToTower(int columnIndex) const;
   int CountKingsOnTowers(void) const { return cards.CountKingsOnTowers(); }
   bool  DoFreeMoves(void);
   uint8_t GetColumnCardCount(uint8_t column) const { return columnCounts.Get(column); }
   LinkedCard GetCard(CardLocation cardLocation) const { return cards.GetCard(cardLocation); }
   LinkedCard GetColumnBottomCard(int column) const { return cards.GetColumnCard(column, columnCounts.Get(column)); }
   ProblemCard GetBottomColumnCardDetails(int column) const;
   int GetEmptyColumnCount(void) const;
   int GetFreeTowers(void) const { return cards.GetEmptyTowers(); }
   SolverMove   GetMoveThatWasPerformed(void) const { return movePerformed; }
   SolverHashCode GetHashValue(void) const;
   LinkedCard GetThrone(Suit suit) const { return cards.GetThrone(suit); }
   bool  IsBottomColumnCard(CardLocation card) const;
   bool  IsOnlyCardOnColumn(LinkID link) const;
   bool  IsVictory(void) const;
   void PerformMove(SolverMove move);
   
private:
   void Clear(void);
   void MoveColumnToColumnOrThrone(int columnIndex);
   void MoveColumnToTower(int columnIndex);
   void MoveTowerToThrone(Suit suit);

private:
   LinkedCards    cards;
   SolverMove     movePerformed;
   ColumnCounts   columnCounts;
};

#endif	/* SOLVERSTATE_H */

