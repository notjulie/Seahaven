/* 
 * File:   LinkedCards.h
 * Author: Randy
 *
 * Created on April 3, 2014, 1:51 PM
 */

#ifndef LINKEDCARDS_H
#define	LINKEDCARDS_H

#include "CardLocation.h"
#include "ProblemCard.h"
#include "SolverException.h"
#include "Suit.h"

struct LinkedCard {
public:
   CardLocation toHigher;
   CardLocation toLower;
   uint8_t size;

public:
   static const LinkedCard Null;
};


class LinkedCards {
public:
   LinkedCards();

   inline LinkedCard GetCard(CardLocation cardLocation) const { return links[(int)cardLocation.GetLinkID()]; }

   void Clear(void);
   int CountKingsOnTowers(void) const;
   int GetEmptyTowers(void) const;
   int GetThroneOccupationMask(void) const;
   bool IsKing(LinkedCard card) const;
   void MoveToOpenTower(CardLocation cardLocation);
   void MoveToHigher(CardLocation link);
   void MoveToLower(LinkID link);
   void SetAceSizes(void);
   void SetCard(CardLocation cardLocation, LinkedCard card);
   ProblemCard GetCardDetails(LinkID link) const;
   
private:
   LinkedCard links[(uint8_t)LinkID::LINK_COUNT];
};

#endif	/* LINKEDCARDS_H */

