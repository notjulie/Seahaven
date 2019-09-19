
#ifndef CARDLOCATION_H
#define CARDLOCATION_H

#include "LinkID.h"
#include "Suit.h"

class CardLocation {
public:
   inline int GetRowIndex(void) const { return ((uint8_t)linkID - (uint8_t)LinkID::FIRST_COLUMN_LINK) % 5; }
   inline LinkID GetLinkID(void) const { return linkID; }
   inline bool IsThrone(void) const { return (uint8_t)linkID >= (uint8_t)LinkID::FIRST_THRONE_LINK && (uint8_t)linkID - (uint8_t)LinkID::FIRST_THRONE_LINK <= 3; }
   inline bool IsTower(void) const { return (uint8_t)linkID >= (uint8_t)LinkID::FIRST_TOWER_LINK && (uint8_t)linkID - (uint8_t)LinkID::FIRST_TOWER_LINK <= 3; }
   inline void SetLinkID(LinkID linkID) { this->linkID = linkID; }

   int GetColumnIndex(void) const;
   Suit GetSuit(void) const;

   inline static CardLocation GetColumnCard(int column, int row) {
      CardLocation result;
      result.linkID = (LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 5 * column + row);
      return result;
   }
   inline static CardLocation GetTower(int tower) {
      CardLocation result;
      result.linkID = (LinkID)((uint8_t)LinkID::FIRST_TOWER_LINK + tower);
      return result;
   }

private:
   LinkID linkID;
};

#endif
