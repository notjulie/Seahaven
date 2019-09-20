/* 
 * File:   ProblemCards.h
 * Author: Randy
 *
 * Created on April 2, 2014, 2:08 PM
 */

#ifndef PROBLEMCARDS_H
#define	PROBLEMCARDS_H

#include <vector>
#include "ProblemCard.h"

class ProblemCards {
public:
   ProblemCards(void);
   ProblemCards(const std::initializer_list<const char*> cards);

    void Append(const ProblemCard& card) { cards.push_back(card); }
    void Dump(FILE *f);
    ProblemCard   GetCard(uint8_t index) const;
    uint8_t       GetCount(void) const {
       return (uint8_t)cards.size();
    }
    
private:
    std::vector<ProblemCard> cards;
};

#endif	/* PROBLEMCARDS_H */

