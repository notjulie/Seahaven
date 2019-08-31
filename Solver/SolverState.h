/* 
 * File:   SolverState.h
 * Author: Randy
 *
 * Created on April 2, 2014, 10:34 PM
 */

#ifndef SOLVERSTATE_H
#define	SOLVERSTATE_H

#include "LinkedCards.h"
#include "Suit.h"
#include "ThroneArbitor.h"

class SeahavenProblem;
class SolverHashCode;

class SolverState {
public:
   SolverState(void);
   SolverState(const SeahavenProblem &problem);

   bool  CanMoveColumnToColumnOrThrone(int columnIndex) const;
   bool  CanMoveColumnToTower(int columnIndex) const;
   void  DoFreeMoves(void);
   bool  IsBottomColumnCard(LinkID link) const;
   bool  IsOnlyCardOnColumn(LinkID link) const;
   bool  IsVictory(void) const;
   void  MoveColumnToColumnOrThrone(int columnIndex);
   void  MoveColumnToTower(int columnIndex);
   ProblemCard GetBottomColumnCardDetails(int column) const;
   int   GetColumnThatWasMoved(void) const { return columnThatWasMoved; }
   SolverHashCode GetHashValue(void) const;
   
private:
   void ArbitrateThrones(void);
   void Clear(void);

private:
   static LinkID GetLinkID(const SeahavenProblem &problem, Suit suit, uint8_t rank);

private:
   LinkedCards    cards;
   ThroneArbitor  throneArbitor;
   uint8_t     columnCounts[10];
   uint8_t     columnThatWasMoved;
};

#endif	/* SOLVERSTATE_H */

