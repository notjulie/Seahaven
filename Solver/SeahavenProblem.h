/*
 * File:   SeahavenProblem.h
 * Author: Randy
 *
 * Created on April 2, 2014, 1:51 PM
 */

#ifndef SEAHAVENPROBLEM_H
#define	SEAHAVENPROBLEM_H

#include "CardLocation.h"
#include "ProblemCards.h"

struct CompactedColumnCard {
   ProblemCard topCard;
   uint8_t  cardCount;
};

class SolverState;

struct ProblemDefinition {
   std::initializer_list<std::initializer_list<const char*>> columns;
   std::initializer_list<const char*> towers;
};

class SeahavenProblem {
public:
   SeahavenProblem(void);
   SeahavenProblem(const ProblemDefinition& problemDefinition);
   SeahavenProblem(const SolverState& state);
   virtual ~SeahavenProblem();

   void Dump(FILE* f);
   std::vector<CompactedColumnCard> GetCompactedColumn(uint8_t columnIndex) const;
   ProblemCard GetTower(uint8_t index) const { return towers.GetCard(index); }
   CardLocation GetCardLocation(Suit suit, uint8_t rank) const;
   void SetColumnCard(int column, int row, const ProblemCard &card);

   static SeahavenProblem CreateRandom(void);
   static SeahavenProblem CreateFromJSON(const std::string &jsonCardLocations);

private:
   void AddCardToCollection(const SolverState& state, ProblemCards& cards, CardLocation cardLocation);

private:
   ProblemCards towers;
   std::vector<ProblemCards> columns;
};

#endif	/* SEAHAVENPROBLEM_H */

