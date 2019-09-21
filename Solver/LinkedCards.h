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

struct alignas(8) LinkedCard {
public:
   CardLocation toHigher;
   CardLocation toLower;
   uint8_t size;

public:
   static const LinkedCard Null;
};
static_assert(sizeof(LinkedCard) <= 8, "LinkedCard is getting chubby");


class LinkedCards {
public:
   LinkedCards(void);

   inline LinkedCard GetCard(CardLocation cardLocation) const { return links[(int)cardLocation.linkID]; }

   void Clear(void);
   int CountKingsOnTowers(void) const;
   int GetEmptyTowers(void) const;
   int GetThroneOccupationMask(void) const;
   void MoveToOpenTower(CardLocation cardLocation);
   void MoveToHigher(CardLocation link);
   void MoveToLower(CardLocation link);
   void SetAceSizes(void);
   void SetCard(CardLocation cardLocation, LinkedCard card);
   ProblemCard GetCardDetails(CardLocation link) const;
   
private:
   LinkedCard links[(uint8_t)LinkID::LINK_COUNT];
};

#endif	/* LINKEDCARDS_H */

