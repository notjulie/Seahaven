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
#include "Suit.h"

struct LinkedCard {
public:
   CardLocation toHigher;
   CardLocation toLower;
   uint8_t size;
};


class alignas(2) CompressedLink {
public:
   inline CompressedLink& operator=(const LinkedCard & card) {
      link = CalculateLink(card.toLower.GetLinkID(), card.toHigher.GetLinkID(), card.size);
      return *this;
   }

   operator LinkedCard() {
      LinkedCard result;
      result.toHigher.SetLinkID((LinkID)(link >> 10));
      result.toLower.SetLinkID((LinkID)(0x3f & (link >> 4)));
      result.size = 0xF & link;
      return result;
   }

   static CompressedLink Create(LinkID toLower, LinkID toHigher, uint8_t size) {
      CompressedLink result;
      result.link = CalculateLink(toLower, toHigher, size);
      return result;
   }

public:
   static const CompressedLink Null;

private:
   inline static uint16_t CalculateLink(LinkID toLower, LinkID toHigher, uint8_t size) {
      return
         size +
         ((uint8_t)toLower << 4) +
         ((uint8_t)toHigher << 10);
   }

private:
   uint16_t link;
};
static_assert(sizeof(CompressedLink) == 2, "CompressedLink length is not 2");


class LinksArray {
public:
   void Clear(void);

   inline CompressedLink& operator[](LinkID linkID) { return links[(uint8_t)linkID]; }
   inline CompressedLink operator[](LinkID linkID) const { return links[(uint8_t)linkID]; }

private:
   CompressedLink links[(uint8_t)LinkID::LINK_COUNT];
};

class LinkedCards {
public:
   LinkedCards();

   void Clear(void);
   LinkedCard GetAce(Suit suit) const;
   LinkedCard GetColumnCard(int column, int row) const;
   int GetEmptyTowers(void) const;
   LinkedCard GetThrone(Suit suit) const;
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
      return (LinkID)((uint8_t)LinkID::FIRST_ACE_LINK + suit.GetIndex()); }
   static inline LinkID GetColumnLinkID(uint8_t column, uint8_t index) {
      return (LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 5*column + index); }
   static inline LinkID GetThroneLinkID(Suit suit) {
      return (LinkID)((uint8_t)LinkID::FIRST_THRONE_LINK + suit.GetIndex()); }
   static inline LinkID GetTowerLinkID(uint8_t tower) {
      return (LinkID)((uint8_t)LinkID::FIRST_TOWER_LINK + tower); }

private:
   LinksArray links;
};

#endif	/* LINKEDCARDS_H */

