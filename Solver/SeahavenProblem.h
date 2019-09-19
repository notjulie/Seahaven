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
   SeahavenProblem(const SolverState &state);
   SeahavenProblem(
            const ProblemCards &column0,
            const ProblemCards &column1,
            const ProblemCards &column2,
            const ProblemCards &column3,
            const ProblemCards &column4,
            const ProblemCards &column5,
            const ProblemCards &column6,
            const ProblemCards &column7,
            const ProblemCards &column8,
            const ProblemCards &column9,
            const ProblemCards &towers
            );
    virtual ~SeahavenProblem();

    void Dump(FILE *f);
    std::vector<CompactedColumnCard> GetCompactedColumn(uint8_t columnIndex) const;
    ProblemCard GetTower(uint8_t index) const { return towers.GetCard(index); }
    LinkID GetCardLinkID(Suit suit, uint8_t rank) const;

    static SeahavenProblem CreateRandom(void);

private:
   void AddCardToCollection(const SolverState &state, ProblemCards &cards, CardLocation cardLocation);

private:
    ProblemCards towers;
    std::vector<ProblemCards> columns;
};

#endif	/* SEAHAVENPROBLEM_H */

