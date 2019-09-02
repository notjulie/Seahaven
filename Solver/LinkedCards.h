/* 
 * File:   LinkedCards.h
 * Author: Randy
 *
 * Created on April 3, 2014, 1:51 PM
 */

#ifndef LINKEDCARDS_H
#define	LINKEDCARDS_H

#include "Suit.h"

enum LinkID {
   FIRST_COLUMN_LINK = 0,
   FIRST_TOWER_LINK = 50,
   FIRST_ACE_LINK = 54,
   FIRST_THRONE_LINK = 58,
   LINK_COUNT = 62,
   NO_LINK = LINK_COUNT
};

class LinkedCard {
public:
   LinkedCard(LinkID toHigher = NO_LINK, LinkID toLower = NO_LINK, uint8_t size = 0) {
      this->toHigher = toHigher;
      this->toLower = toLower;
      this->size = size;
   }

   LinkedCard(uint16_t ui16) {
      toHigher = (LinkID)(ui16 >> 10);
      toLower = (LinkID)(0x3f & (ui16 >> 4));
      size = 0xF & ui16;
   }

   uint16_t AsUInt16(void) const {
      return
         size +
         (toLower << 4) +
         (toHigher << 10);
   }

public:
   LinkID toHigher;
   LinkID toLower;
   uint8_t size;
};

class LinkedCards {
public:
   LinkedCards();

   void Clear(void);
   LinkedCard GetAce(Suit suit) const;
   LinkedCard GetColumnCard(int column, int row) const;
   int GetEmptyTowers(void) const;
   LinkedCard GetThrone(int i) const;
   int GetThroneOccupationMask(void) const;
   LinkedCard GetTower(int i) const;
   bool IsKing(LinkedCard card) const;
   void MoveColumnCardToHigher(LinkID link);
   void MoveToOpenTower(LinkID link);
   void MoveToHigher(LinkID link);
   void MoveToLower(LinkID link);
   void SetAceSizes(void);
   void SetCard(LinkID card, LinkID toLow, LinkID toHigh, uint8_t size);
   ProblemCard GetCardDetails(LinkID link) const;
   
public:
   static inline LinkID GetAceLinkID(Suit suit) {
      return (LinkID)(FIRST_ACE_LINK + suit); }
   static inline LinkID GetColumnLinkID(uint8_t column, uint8_t index) {
      return (LinkID)(FIRST_COLUMN_LINK + 5*column + index); }
   static inline LinkID GetThroneLinkID(Suit suit) {
      return (LinkID)(FIRST_THRONE_LINK + suit); }
   static inline LinkID GetTowerLinkID(uint8_t tower) {
      return (LinkID)(FIRST_TOWER_LINK + tower); }
   static inline bool IsTower(LinkID link) {
      return link>=FIRST_TOWER_LINK && link-FIRST_TOWER_LINK<=3; }
   static inline bool IsThrone(LinkID link) {
      return link>=FIRST_TOWER_LINK && link-FIRST_TOWER_LINK<=3; }
   
private:
   uint16_t links[LINK_COUNT];
};

#endif	/* LINKEDCARDS_H */

