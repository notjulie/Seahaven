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
    ProblemCards(
            const ProblemCard &card0 = ProblemCard(),
            const ProblemCard &card1 = ProblemCard(),
            const ProblemCard &card2 = ProblemCard(),
            const ProblemCard &card3 = ProblemCard(),
            const ProblemCard &card4 = ProblemCard(),
            const ProblemCard &card5 = ProblemCard(),
            const ProblemCard &card6 = ProblemCard(),
            const ProblemCard &card7 = ProblemCard(),
            const ProblemCard &card8 = ProblemCard()
            );

    void Append(const ProblemCard& card) { cards.push_back(card); }
    ProblemCard   GetCard(uint8_t index) const;
    uint8_t       GetCount(void) const {
       return (uint8_t)cards.size();
    }
    
private:
    std::vector<ProblemCard> cards;
};

#endif	/* PROBLEMCARDS_H */

