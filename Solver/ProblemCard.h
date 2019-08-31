/* 
 * File:   ProblemCard.h
 * Author: Randy
 *
 * Created on April 2, 2014, 2:12 PM
 */

#ifndef PROBLEMCARD_H
#define	PROBLEMCARD_H

#include "Suit.h"

class ProblemCard {
public:
    ProblemCard(const char *_s = NULL);
    ProblemCard(Suit _suit, int _rank);

    void       DumpToConsole(void) const;
    bool       IsNull(void) const { return rank==0; }
    Suit       GetSuit(void) const { return suit; }
    uint8_t    GetRank(void) const { return rank; }

    bool operator==(const ProblemCard &that) const;
    
private:
    uint8_t     rank;
    Suit        suit;
};

#endif	/* PROBLEMCARD_H */

